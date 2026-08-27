/* Copyright 2026
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "pixel_scroll_bridge.h"

#include <string.h>

#include "raw_hid.h"
#include "timer.h"

#define PSCR_PACKET_SIZE 32
#define PSCR_MAGIC0 0xA5
#define PSCR_MAGIC1 0x5A
#define PSCR_PROTOCOL_VERSION 0x03
#define PSCR_HEARTBEAT_TIMEOUT_MS 1000

/* Host -> device. Values deliberately live outside VIA's current command-ID
 * range (0x01..0x15, 0xFF) and are further protected by a two-byte magic. */
enum pixel_scroll_host_message {
    PSCR_MSG_OBSERVE   = 0x01,
    PSCR_MSG_CLAIM     = 0x02,
    PSCR_MSG_HEARTBEAT = 0x03,
    PSCR_MSG_RELEASE   = 0x04,
};

/* Device -> host. */
enum pixel_scroll_device_message {
    PSCR_MSG_ACK   = 0x81,
    PSCR_MSG_DELTA = 0x82,
};

enum pixel_scroll_bridge_flags {
    PSCR_FLAG_STREAMING = 1u << 0,
    PSCR_FLAG_TAKEOVER  = 1u << 1,
};

static bool     stream_enabled       = false;
static bool     takeover_enabled     = false;
static uint32_t last_host_message_ms = 0;
static uint16_t delta_sequence       = 0;

static void store_u16_le(uint8_t *dst, uint16_t value) {
    dst[0] = (uint8_t)(value & 0xFFu);
    dst[1] = (uint8_t)((value >> 8) & 0xFFu);
}

static void store_i16_le(uint8_t *dst, int16_t value) {
    store_u16_le(dst, (uint16_t)value);
}

static void store_u32_le(uint8_t *dst, uint32_t value) {
    dst[0] = (uint8_t)(value & 0xFFu);
    dst[1] = (uint8_t)((value >> 8) & 0xFFu);
    dst[2] = (uint8_t)((value >> 16) & 0xFFu);
    dst[3] = (uint8_t)((value >> 24) & 0xFFu);
}

static uint8_t bridge_state_flags(void) {
    uint8_t flags = 0;
    if (stream_enabled) {
        flags |= PSCR_FLAG_STREAMING;
    }
    if (takeover_enabled) {
        flags |= PSCR_FLAG_TAKEOVER;
    }
    return flags;
}

static void init_packet(uint8_t packet[PSCR_PACKET_SIZE], uint8_t message_type) {
    memset(packet, 0, PSCR_PACKET_SIZE);
    packet[0] = PSCR_MAGIC0;
    packet[1] = PSCR_MAGIC1;
    packet[2] = PSCR_PROTOCOL_VERSION;
    packet[3] = message_type;
}

static void send_ack(uint8_t request_type, uint8_t status) {
    uint8_t packet[PSCR_PACKET_SIZE];
    init_packet(packet, PSCR_MSG_ACK);
    packet[4] = bridge_state_flags();
    packet[5] = request_type;
    packet[6] = status;
    store_u32_le(&packet[8], timer_read32());
    raw_hid_send(packet, sizeof(packet));
}

/* VIA owns raw_hid_receive() in this keymap. Its documented keyboard-level
 * hook runs before VIA's normal command switch; returning false preserves
 * every ordinary VIA packet unchanged. For bridge packets we fully handle
 * the request and return true, including the response as required by VIA. */
bool via_command_kb(uint8_t *data, uint8_t length) {
    if (length != PSCR_PACKET_SIZE || data[0] != PSCR_MAGIC0 || data[1] != PSCR_MAGIC1) {
        return false;
    }

    const uint8_t request_type = data[3];
    if (data[2] != PSCR_PROTOCOL_VERSION) {
        send_ack(request_type, 1); /* unsupported protocol version */
        return true;
    }

    switch (request_type) {
        case PSCR_MSG_OBSERVE:
            stream_enabled       = true;
            takeover_enabled     = false;
            last_host_message_ms = timer_read32();
            send_ack(request_type, 0);
            return true;

        case PSCR_MSG_CLAIM:
            stream_enabled       = true;
            takeover_enabled     = true;
            last_host_message_ms = timer_read32();
            send_ack(request_type, 0);
            return true;

        case PSCR_MSG_HEARTBEAT:
            if (stream_enabled) {
                last_host_message_ms = timer_read32();
            }
            send_ack(request_type, 0);
            return true;

        case PSCR_MSG_RELEASE:
            stream_enabled   = false;
            takeover_enabled = false;
            send_ack(request_type, 0);
            return true;

        default:
            send_ack(request_type, 2); /* unknown bridge command */
            return true;
    }
}

void pixel_scroll_bridge_housekeeping(void) {
    if (stream_enabled && timer_elapsed32(last_host_message_ms) > PSCR_HEARTBEAT_TIMEOUT_MS) {
        /* Fail open to the factory wheel path. A crashed/disconnected
         companion must never strand the user without scrolling. */
        stream_enabled   = false;
        takeover_enabled = false;
    }
}

bool pixel_scroll_bridge_streaming(void) {
    pixel_scroll_bridge_housekeeping();
    return stream_enabled;
}

bool pixel_scroll_bridge_takeover(void) {
    pixel_scroll_bridge_housekeeping();
    return takeover_enabled;
}

uint8_t pixel_scroll_bridge_poll_interval_ms(void) {
    pixel_scroll_bridge_housekeeping();
    /* The stock code runs when elapsed > 10 ms (~90 Hz). In takeover mode,
       elapsed > 3 ms gives ~250 Hz samples for a 120 Hz display while leaving
       the no-companion path byte-for-byte equivalent in timing behaviour. */
    return takeover_enabled ? 3 : 10;
}

void pixel_scroll_bridge_send_sample(uint16_t left_angle, uint16_t right_angle,
                                     int16_t left_delta, int16_t right_delta,
                                     const tmag5273_debug_sample_t *left_diag,
                                     const tmag5273_debug_sample_t *right_diag,
                                     uint8_t mode_flags) {
    pixel_scroll_bridge_housekeeping();
    if (!stream_enabled || left_diag == NULL || right_diag == NULL) {
        return;
    }

    /* Protocol v3 keeps the v2 angle/delta fields and adds the underlying
       X/Y magnetic vector plus magnitude and conversion status. This makes
       the hardware-vs-CORDIC boundary directly observable on the host. */
    uint8_t packet[PSCR_PACKET_SIZE];
    init_packet(packet, PSCR_MSG_DELTA);
    packet[4] = bridge_state_flags() | mode_flags;
    store_u16_le(&packet[6], delta_sequence++);
    store_i16_le(&packet[8], left_delta);
    store_i16_le(&packet[10], right_delta);
    store_u16_le(&packet[12], left_angle);
    store_u16_le(&packet[14], right_angle);
    store_u32_le(&packet[16], timer_read32());
    store_i16_le(&packet[20], left_diag->x);
    store_i16_le(&packet[22], left_diag->y);
    store_i16_le(&packet[24], right_diag->x);
    store_i16_le(&packet[26], right_diag->y);
    packet[28] = left_diag->magnitude;
    packet[29] = right_diag->magnitude;
    packet[30] = left_diag->conv_status;
    packet[31] = right_diag->conv_status;
    raw_hid_send(packet, sizeof(packet));
}
