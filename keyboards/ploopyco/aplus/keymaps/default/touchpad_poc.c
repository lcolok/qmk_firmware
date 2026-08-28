/* Copyright 2026
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "touchpad_poc.h"

#include <stdlib.h>

#include "host_driver.h"
#include "report.h"
#include "timer.h"

#define TOUCHPAD_COORD_MIN 0
#define TOUCHPAD_COORD_MAX 4095
#define TOUCHPAD_START_Y 2048
#define TOUCHPAD_FINGER1_X 1700
#define TOUCHPAD_FINGER2_X 2400
#define TOUCHPAD_EDGE_MARGIN 384
#define TOUCHPAD_FILTER_WINDOW_MS 25
#define TOUCHPAD_START_THRESHOLD 4
#define TOUCHPAD_RELEASE_MS 80

static bool     touch_active        = false;
static int32_t  virtual_y           = TOUCHPAD_START_Y;
static int16_t  candidate_delta     = 0;
static uint32_t candidate_started   = 0;
static uint32_t last_motion_time_ms = 0;

static uint16_t clamp_coord(int32_t value) {
    if (value < TOUCHPAD_COORD_MIN) {
        return TOUCHPAD_COORD_MIN;
    }
    if (value > TOUCHPAD_COORD_MAX) {
        return TOUCHPAD_COORD_MAX;
    }
    return (uint16_t)value;
}

static void send_touch_report(bool contacts_down) {
    report_digitizer_t report = {
#ifdef DIGITIZER_SHARED_EP
        .report_id = REPORT_ID_DIGITIZER,
#endif
        .finger1_state = contacts_down ? 0x03 : 0x00, /* confidence + tip, ID 0 */
        .finger1_x     = TOUCHPAD_FINGER1_X,
        .finger1_y     = clamp_coord(virtual_y),
        .finger2_state = contacts_down ? 0x07 : 0x00, /* confidence + tip, ID 1 */
        .finger2_x     = TOUCHPAD_FINGER2_X,
        .finger2_y     = clamp_coord(virtual_y),
        .contact_count = contacts_down ? 2 : 0,
    };
    send_digitizer(&report);
}

static void release_contacts(void) {
    if (touch_active) {
        send_touch_report(false);
    }
    touch_active        = false;
    candidate_delta     = 0;
    candidate_started   = 0;
    last_motion_time_ms = 0;
    virtual_y           = TOUCHPAD_START_Y;
}

void touchpad_poc_process(int16_t wheel_delta, bool enabled) {
    const uint32_t now = timer_read32();

    if (!enabled) {
        release_contacts();
        return;
    }

    if (candidate_started == 0) {
        candidate_started = now;
    }
    candidate_delta += wheel_delta;

    int16_t accepted_delta = 0;
    if (abs(candidate_delta) >= TOUCHPAD_START_THRESHOLD) {
        accepted_delta   = candidate_delta;
        candidate_delta  = 0;
        candidate_started = now;
    } else if (timer_elapsed32(candidate_started) >= TOUCHPAD_FILTER_WINDOW_MS) {
        /* Static TMAG noise measured on the real A+ is overwhelmingly +/-1 or
         * +/-2 and cancels over a short window. Discard sub-threshold drift. */
        candidate_delta   = 0;
        candidate_started = now;
    }

    if (accepted_delta != 0) {
        if (!touch_active) {
            touch_active = true;
            virtual_y    = TOUCHPAD_START_Y;
            send_touch_report(true);
        }

        int32_t next_y = virtual_y + accepted_delta;
        if (next_y < TOUCHPAD_EDGE_MARGIN || next_y > (TOUCHPAD_COORD_MAX - TOUCHPAD_EDGE_MARGIN)) {
            /* The fake surface is finite while the knob is not. End one
             * gesture, recenter, and start another rather than wrapping an
             * active contact across the pad. */
            send_touch_report(false);
            virtual_y = TOUCHPAD_START_Y;
            send_touch_report(true);
            next_y = virtual_y + accepted_delta;
        }
        virtual_y           = next_y;
        last_motion_time_ms = now;
    }

    if (!touch_active) {
        return;
    }

    if (last_motion_time_ms != 0 && timer_elapsed32(last_motion_time_ms) >= TOUCHPAD_RELEASE_MS) {
        release_contacts();
        return;
    }

    /* Keep reporting active contacts at the A+ wheel polling cadence (~90Hz),
     * even between movement samples. This is intentionally above 60Hz so the
     * host gets a coherent two-finger frame stream. */
    send_touch_report(true);
}
