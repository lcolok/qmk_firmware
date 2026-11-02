#!/usr/bin/env swift
/*
 * Q3 Pro auto-mode companion.
 *
 * Division of labour (mirrors the firmware side):
 *   DIP on  = auto mode. This program owns the default layer.
 *   DIP off = manual mode. This program refuses to send SET; the keyboard
 *             falls back to DIP-selected layers.
 *
 * State source: NSWorkspace.didActivateApplicationNotification (event-driven,
 * no polling). Reading the frontmost app's bundle id does not require
 * accessibility permission.
 *
 * Transport: VIA custom value, channel 0, value_id 0x10.
 *   SET payload: [layer, lease_seconds]
 *   GET payload: [mode, last_layer, lease_remaining_s, signal_present]
 *
 * Lease: refreshed on every SET. If this program dies, the firmware falls back
 * to AUTO_FALLBACK_LAYER after the lease expires.
 *
 * NOTE: this requires the firmware that implements AUTO_VALUE_ID. Against the
 * factory firmware the SET/GET returns id_unhandled (0xFF) — see README.
 */

import Foundation
import IOKit.hid
import AppKit

let VALUE_ID: UInt8 = 0x10
let CHANNEL: UInt8 = 0
let CMD_SET: UInt8 = 0x07
let CMD_GET: UInt8 = 0x08
let CMD_UNHANDLED: UInt8 = 0xFF
let PACKET = 32
let LEASE_SECONDS: UInt8 = 7
let HEARTBEAT_SECONDS: Double = 2.0

// bundle id -> target layer. Edit to taste; unknown apps use DEFAULT_LAYER.
let DEFAULT_LAYER: UInt8 = 0
let rules: [String: UInt8] = [
    "com.apple.Terminal": 2,
    "com.mitchellh.ghostty": 2,
    "com.google.Chrome": 3,
    "com.apple.Safari": 3,
]

var device: IOHIDDevice? = nil
var hidManager: IOHIDManager? = nil

// Raw HID replies arrive on the interrupt input endpoint, i.e. through the
// input report callback — IOHIDDeviceGetReport (control endpoint) does not see
// them. Verified against the device: SetReport succeeds, GetReport times out,
// the callback is the path hidapi uses.
let cbQueue = DispatchQueue(label: "hidcb")
let respSem = DispatchSemaphore(value: 0)
var cbBuf = [UInt8](repeating: 0, count: 64)
var lastResponse: [UInt8]? = nil

func packet(_ payload: [UInt8]) -> [UInt8] {
    var p = payload
    while p.count < PACKET { p.append(0) }
    return p
}

func registerCallback(_ dev: IOHIDDevice) {
    IOHIDDeviceSetDispatchQueue(dev, cbQueue)
    IOHIDDeviceRegisterInputReportCallback(dev, &cbBuf, cbBuf.count, { context, result, device, reportType, reportId, report, reportSize in
        if result == kIOReturnSuccess {
            lastResponse = Array(UnsafeBufferPointer(start: report, count: reportSize))
            respSem.signal()
        }
    }, nil)
    IOHIDDeviceActivate(dev)
}

func closeDevice() {
    guard let dev = device else { return }
    let cancelled = DispatchSemaphore(value: 0)
    IOHIDDeviceSetCancelHandler(dev) { cancelled.signal() }
    IOHIDDeviceCancel(dev)
    cancelled.wait()
    IOHIDDeviceClose(dev, IOOptionBits(kIOHIDOptionsTypeNone))
    device = nil
    if let mgr = hidManager {
        IOHIDManagerClose(mgr, IOOptionBits(kIOHIDOptionsTypeNone))
        hidManager = nil
    }
}

func send(_ payload: [UInt8]) -> [UInt8]? {
    guard let dev = device else { return nil }
    let p = packet(payload)
    lastResponse = nil
    // No report-id prefix: the firmware expects data[0] = command_id.
    let rc = IOHIDDeviceSetReport(dev, kIOHIDReportTypeOutput, 0, p, p.count)
    if rc != kIOReturnSuccess { return nil }
    if respSem.wait(timeout: .now() + 3) == .timedOut { return nil }
    return lastResponse
}

func openDevice() -> (IOHIDDevice?, String) {
    let mgr = IOHIDManagerCreate(kCFAllocatorDefault, IOOptionBits(kIOHIDOptionsTypeNone))

    // 保留实测成功的调用顺序。此前测试受设备占用干扰，
    // 尚未证明反向顺序必然失败，不应视为 IOKit 的通用要求。
    if IOHIDManagerOpen(mgr, IOOptionBits(kIOHIDOptionsTypeNone)) != kIOReturnSuccess {
        return (nil, "IOHIDManagerOpen failed")
    }

    // Classic matching dictionary: vendor 0x3434, usage page 0xFF60.
    // kIOHIDVendorIDKey is imported as "VendorID" (verified); the usage-page
    // key is not imported into Swift scope, so the documented string literal
    // is used here.
    let matching = [
        kIOHIDVendorIDKey: 0x3434,
        "UsagePage": 0xFF60,
    ] as CFDictionary
    IOHIDManagerSetDeviceMatching(mgr, matching)

    // The device exposes several matching collections; only one opens (the
    // others return 0xe00002e2). Try each and keep the one that opens.
    let devices = IOHIDManagerCopyDevices(mgr) as? Set<IOHIDDevice>
    if devices?.count == 0 {
        return (nil, "no 0x3434 raw HID collection (usage page 0xFF60)")
    }
    for d in devices ?? [] {
        if IOHIDDeviceOpen(d, IOOptionBits(kIOHIDOptionsTypeNone)) == kIOReturnSuccess {
            hidManager = mgr
            registerCallback(d)
            return (d, "")
        }
    }
    return (nil, "collection present but every open failed (0xe00002e2: already in use?)")
}

func currentLayer() -> UInt8 {
    let app = NSWorkspace.shared.frontmostApplication
    let id = app?.bundleIdentifier ?? ""
    return rules[id] ?? DEFAULT_LAYER
}

func main() {
    // Unbuffered: output must appear even when stdout is redirected to a file.
    setbuf(stdout, nil)

    let (dev, why) = openDevice()
    if dev == nil {
        print("[companion] \(why)")
        exit(1)
    }
    device = dev
    defer { closeDevice() }

    // Probe: read mode. If the firmware does not implement AUTO_VALUE_ID the
    // response command id is 0xFF, so we report honestly instead of assuming.
    if let r = send([CMD_GET, CHANNEL, VALUE_ID, 0]), r.count >= 7 {
        if r[0] == CMD_UNHANDLED {
            print("[companion] firmware does not implement AUTO_VALUE_ID 0x10 (id_unhandled)")
            print("[companion] flash the build that defines it before using auto mode")
            closeDevice()
            exit(2)
        }
        guard r[0] == CMD_GET, r[1] == CHANNEL, r[2] == VALUE_ID else {
            print("[companion] unexpected response to GET")
            closeDevice()
            exit(3)
        }
        print("[companion] mode=\(r[3]) last_layer=\(r[4]) lease_left=\(r[5])s present=\(r[6])")
    } else {
        print("[companion] no valid response to GET")
        closeDevice()
        exit(3)
    }

    if CommandLine.arguments.contains("--probe") { return }

    NSWorkspace.shared.notificationCenter.addObserver(
        forName: NSWorkspace.didActivateApplicationNotification,
        object: nil, queue: .main
    ) { note in
        guard let app = note.userInfo?["app"] as? NSRunningApplication else { return }
        let layer = rules[app.bundleIdentifier ?? ""] ?? DEFAULT_LAYER
        let r = send([CMD_SET, CHANNEL, VALUE_ID, layer, LEASE_SECONDS])
        if r == nil {
            print("[companion] SET failed (no response)")
        } else if r![0] == CMD_UNHANDLED {
            print("[companion] SET rejected: firmware is in manual mode (DIP off)")
        } else {
            print("[companion] \(app.localizedName ?? "?") -> layer \(layer)")
        }
    }

    // Heartbeat keeps the lease alive while the frontmost app does not change.
    Timer.scheduledTimer(withTimeInterval: HEARTBEAT_SECONDS, repeats: true) { _ in
        let layer = currentLayer()
        let r = send([CMD_SET, CHANNEL, VALUE_ID, layer, LEASE_SECONDS])
        if r != nil && r![0] != CMD_UNHANDLED {
            print("[companion] heartbeat -> layer \(layer)")
        }
    }

    RunLoop.main.run()
}

main()
