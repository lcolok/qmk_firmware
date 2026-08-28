/* Copyright 2026
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Gate C v4b: firmware-only Magic Trackpad 2 scroll-frame experiment.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Feed the healthy vertical TMAG delta at the A+ wheel sampling cadence.
 * enabled=false immediately terminates the synthetic two-finger gesture. */
void mt2_scroll_poc_process(int16_t wheel_delta, bool enabled);
