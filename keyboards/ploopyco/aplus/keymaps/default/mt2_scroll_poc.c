/* Copyright 2026
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Gate C v4b: send only the small, public Magic Trackpad 2 USB touch packet
 * format (12-byte prefix + two 9-byte finger records).  The packing follows
 * the Linux hid-magicmouse parser and the VoodooInput simulator.  This file
 * deliberately never emits the unrelated 0x44 / 1387-byte vendor report.
 */

#include "mt2_scroll_poc.h"

#include <string.h>

#include "host_driver.h"
#include "report.h"
#include "timer.h"

#define MT2_REPORT_ID 0x02
#define MT2_INNER_REPORT_ID 0x31
#define MT2_TOUCH_ACTIVE 0x03
#define MT2_TOUCH_INACTIVE 0x02

#define MT2_STATE_INACTIVE 0x00
#define MT2_STATE_START 0x03
#define MT2_STATE_ACTIVE 0x04
#define MT2_STATE_STOP 0x07

#define MT2_FINGER_INDEX 0x02
#define MT2_FINGER_MIDDLE 0x03
#define MT2_FINGER_ANGLE 0x04

#define MT2_FINGER1_X (-700)
#define MT2_FINGER2_X 700
#define MT2_START_Y 0
#define MT2_Y_LIMIT 1800

#define MT2_FILTER_WINDOW_MS 20
#define MT2_START_THRESHOLD 4
#define MT2_RELEASE_MS 80

static bool touch_active = false;
static int16_t virtual_y = MT2_START_Y;
static int32_t candidate_delta = 0;
static uint32_t candidate_started = 0;
static uint32_t last_motion_time_ms = 0;

static int16_t clamp_s13(int32_t value) {
    if (value < -4096) {
        return -4096;
    }
    if (value > 4095) {
        return 4095;
    }
    return (int16_t)value;
}

static void encode_timestamp(uint8_t *dst, uint32_t milliseconds) {
    dst[0] = (uint8_t)((milliseconds << 3) | 0x04);
    dst[1] = (uint8_t)((milliseconds >> 5) & 0xFF);
    dst[2] = (uint8_t)((milliseconds >> 13) & 0xFF);
}

/* A Magic Trackpad 2 finger is a packed 72-bit record:
 * X:S13, Y:S13, Finger:U3, State:U3, major, minor, size, pressure,
 * Identifier:U4, reserved:U1, Angle:U3. */
static void pack_finger(uint8_t *dst, int16_t x, int16_t y, uint8_t finger,
                        uint8_t state, uint8_t identifier, bool contact_metrics) {
    const uint16_t ux = (uint16_t)clamp_s13(x) & 0x1FFF;
    const uint16_t uy = (uint16_t)clamp_s13(y) & 0x1FFF;

    dst[0] = (uint8_t)(ux & 0xFF);
    dst[1] = (uint8_t)(((ux >> 8) & 0x1F) | ((uy & 0x07) << 5));
    dst[2] = (uint8_t)((uy >> 3) & 0xFF);
    dst[3] = (uint8_t)(((uy >> 11) & 0x03) | ((finger & 0x07) << 2) | ((state & 0x07) << 5));
    dst[4] = contact_metrics ? 20 : 0; /* Touch major */
    dst[5] = contact_metrics ? 20 : 0; /* Touch minor */
    dst[6] = contact_metrics ? 10 : 0; /* Size */
    dst[7] = contact_metrics ? 5 : 0;  /* Pressure */
    dst[8] = (uint8_t)((identifier & 0x0F) | ((MT2_FINGER_ANGLE & 0x07) << 5));
}

static void send_frame(uint8_t state, bool contact_metrics, bool active_header) {
    report_digitizer_t report;
    memset(&report, 0, sizeof(report));

    report.data[0] = MT2_REPORT_ID;
    report.data[1] = 0; /* physical button */
    /* bytes 2..6 intentionally remain zero */
    report.data[7] = active_header ? MT2_TOUCH_ACTIVE : MT2_TOUCH_INACTIVE;
    report.data[8] = MT2_INNER_REPORT_ID;
    encode_timestamp(&report.data[9], timer_read32());

    pack_finger(&report.data[12], MT2_FINGER1_X, virtual_y, MT2_FINGER_INDEX, state, 1, contact_metrics);
    pack_finger(&report.data[21], MT2_FINGER2_X, virtual_y, MT2_FINGER_MIDDLE, state, 2, contact_metrics);
    send_digitizer(&report);
}

static void release_contacts(void) {
    if (touch_active) {
        send_frame(MT2_STATE_STOP, false, false);
        send_frame(MT2_STATE_INACTIVE, false, false);
    }

    touch_active = false;
    virtual_y = MT2_START_Y;
    candidate_delta = 0;
    candidate_started = 0;
    last_motion_time_ms = 0;
}

static int16_t take_filtered_delta(uint32_t now, int16_t wheel_delta) {
    if (candidate_started == 0) {
        candidate_started = now;
    }
    candidate_delta += wheel_delta;

    if (candidate_delta >= MT2_START_THRESHOLD || candidate_delta <= -MT2_START_THRESHOLD) {
        const int32_t accepted = candidate_delta;
        candidate_delta = 0;
        candidate_started = now;
        if (accepted > INT16_MAX) {
            return INT16_MAX;
        }
        if (accepted < INT16_MIN) {
            return INT16_MIN;
        }
        return (int16_t)accepted;
    }

    if (timer_elapsed32(candidate_started) >= MT2_FILTER_WINDOW_MS) {
        candidate_delta = 0;
        candidate_started = now;
    }
    return 0;
}

void mt2_scroll_poc_process(int16_t wheel_delta, bool enabled) {
    const uint32_t now = timer_read32();

    if (!enabled) {
        release_contacts();
        return;
    }

    const int16_t accepted_delta = take_filtered_delta(now, wheel_delta);
    if (accepted_delta != 0) {
        if (!touch_active) {
            virtual_y = MT2_START_Y;
            touch_active = true;
            send_frame(MT2_STATE_START, true, true);
        }

        int32_t next_y = (int32_t)virtual_y + accepted_delta;
        if (next_y < -MT2_Y_LIMIT || next_y > MT2_Y_LIMIT) {
            release_contacts();
            virtual_y = MT2_START_Y;
            touch_active = true;
            send_frame(MT2_STATE_START, true, true);
            next_y = accepted_delta;
        }

        virtual_y = clamp_s13(next_y);
        last_motion_time_ms = now;
        send_frame(MT2_STATE_ACTIVE, true, true);
    }

    if (!touch_active) {
        return;
    }

    if (last_motion_time_ms != 0 && timer_elapsed32(last_motion_time_ms) >= MT2_RELEASE_MS) {
        release_contacts();
        return;
    }

    if (accepted_delta == 0) {
        send_frame(MT2_STATE_ACTIVE, true, true);
    }
}
