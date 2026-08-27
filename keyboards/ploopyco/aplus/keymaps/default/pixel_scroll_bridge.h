/* Copyright 2026
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Ploopy A+ experimental macOS pixel-scroll bridge.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Mode bits carried in each delta packet so the host can preserve the A+
 * firmware's non-scroll knob behaviours instead of blindly turning every
 * physical movement into pixels. */
enum pixel_scroll_mode_flags {
    PSCR_FLAG_LEFT_HANDED       = 1u << 2,
    PSCR_FLAG_HORIZONTAL_ARROWS = 1u << 3,
    PSCR_FLAG_VERTICAL_ARROWS   = 1u << 4,
    PSCR_FLAG_CONTROL_LAYER     = 1u << 5,
    PSCR_FLAG_LEFT_KNOB_DOWN    = 1u << 6,
    PSCR_FLAG_RIGHT_KNOB_DOWN   = 1u << 7,
};

/* Called from the normal pointing task. */
void pixel_scroll_bridge_housekeeping(void);
void pixel_scroll_bridge_send_sample(uint16_t left_angle, uint16_t right_angle,
                                     int16_t left_delta, int16_t right_delta,
                                     uint8_t mode_flags);

/* CLAIM switches the normal wheel path off while a healthy companion is
 * present. A 1-second heartbeat timeout automatically restores factory wheel
 * output. */
bool pixel_scroll_bridge_takeover(void);

/* Sampling remains factory-like while merely observing. CLAIM raises the
 * TMAG polling rate so a 120 Hz display is not starved by the stock >10 ms
 * wheel-processing interval. */
uint8_t pixel_scroll_bridge_poll_interval_ms(void);
