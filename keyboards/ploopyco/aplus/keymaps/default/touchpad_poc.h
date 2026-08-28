/* Copyright 2026
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Gate C: firmware-only macOS Touch Pad recognition experiment.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Feed the healthy vertical TMAG delta at the normal wheel sampling cadence.
 * enabled=false immediately lifts both virtual contacts. */
void touchpad_poc_process(int16_t wheel_delta, bool enabled);
