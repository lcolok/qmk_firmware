/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"

// clang-format off
#ifdef RGB_MATRIX_ENABLE
const ckled2001_led g_ckled2001_leds[RGB_MATRIX_LED_COUNT] = {
/* Refer to CKLED2001 manual for these locations
 *   driver
 *   |  R location
 *   |  |       G location
 *   |  |       |       B location
 *   |  |       |       | */
    {0, I_2,    G_2,    H_2},
    {0, I_3,    G_3,    H_3},
    {0, I_4,    G_4,    H_4},
    {0, I_5,    G_5,    H_5},
    {0, I_6,    G_6,    H_6},
    {0, I_7,    G_7,    H_7},
    {0, I_8,    G_8,    H_8},
    {0, I_9,    G_9,    H_9},
    {0, I_10,   G_10,   H_10},
    {0, I_11,   G_11,   H_11},
    {0, I_12,   G_12,   H_12},
    {0, I_13,   G_13,   H_13},
    {0, I_14,   G_14,   H_14},
    {0, I_15,   G_15,   H_15},
    {0, I_16,   G_16,   H_16},
    {1, C_4,    A_4,    B_4},  // 16

    {0, C_1,    A_1,    B_1},
    {0, C_2,    A_2,    B_2},
    {0, C_3,    A_3,    B_3},
    {0, C_4,    A_4,    B_4},
    {0, C_5,    A_5,    B_5},
    {0, C_6,    A_6,    B_6},
    {0, C_7,    A_7,    B_7},
    {0, C_8,    A_8,    B_8},
    {0, C_9,    A_9,    B_9},
    {0, C_10,   A_10,   B_10},
    {0, C_11,   A_11,   B_11},
    {0, C_12,   A_12,   B_12},
    {0, C_13,   A_13,   B_13},
    {0, C_14,   A_14,   B_14},
    {0, C_15,   A_15,   B_15},
    {0, C_16,   A_16,   B_16},
    {1, C_2,    A_2,    B_2},  // 17

    {1, F_13,   D_13,   E_13},
    {0, F_1,    D_1,    E_1},
    {0, F_2,    D_2,    E_2},
    {0, F_3,    D_3,    E_3},
    {0, F_4,    D_4,    E_4},
    {0, F_5,    D_5,    E_5},
    {0, F_6,    D_6,    E_6},
    {0, F_7,    D_7,    E_7},
    {0, F_8,    D_8,    E_8},
    {0, F_9,    D_9,    E_9},
    {0, F_10,   D_10,   E_10},
    {0, F_11,   D_11,   E_11},
    {0, F_12,   D_12,   E_12},
    {0, F_13,   D_13,   E_13},
    {0, F_14,   D_14,   E_14},
    {0, F_15,   D_15,   E_15},
    {0, F_16,   D_16,   E_16},
    {1, C_1,    A_1,    B_1},  // 17

    {1, F_12,   D_12,   E_12},
    {1, C_16,   A_16,   B_16},
    {1, C_15,   A_15,   B_15},
    {1, C_14,   A_14,   B_14},
    {1, C_13,   A_13,   B_13},
    {1, C_12,   A_12,   B_12},
    {1, C_11,   A_11,   B_11},
    {1, C_10,   A_10,   B_10},
    {1, C_9,    A_9,    B_9},
    {1, C_8,    A_8,    B_8},
    {1, C_7,    A_7,    B_7},
    {1, C_6,    A_6,    B_6},
    {1, C_5,    A_5,    B_5},
    {1, C_3,    A_3,    B_3},  // 13

    {1, F_8,    D_8,    E_8},
    {1, I_16,   G_16,   H_16},
    {1, I_14,   G_14,   H_14},
    {1, I_13,   G_13,   H_13},
    {1, I_12,   G_12,   H_12},
    {1, I_11,   G_11,   H_11},
    {1, I_10,   G_10,   H_10},
    {1, I_9,    G_9,    H_9},
    {1, I_8,    G_8,    H_8},
    {1, I_7,    G_7,    H_7},
    {1, I_6,    G_6,    H_6},
    {1, I_5,    G_5,    H_5},
    {1, I_3,    G_3,    H_3},
    {1, I_1,    G_1,    H_1},  // 13

    {1, F_7,    D_7,    E_7},
    {1, F_16,   D_16,   E_16},
    {1, F_15,   D_15,   E_15},
    {1, F_14,   D_14,   E_14},
    {1, F_10,   D_10,   E_10},
    {1, F_6,    D_6,    E_6},
    {1, F_5,    D_5,    E_5},
    {1, F_4,    D_4,    E_4},
    {1, F_3,    D_3,    E_3},
    {1, F_2,    D_2,    E_2},
    {1, F_1,    D_1,    E_1},
    {1, I_2,    G_2,    H_2},
};
#endif

// ---------------------------------------------------------------------------
// Auto mode: DIP selects the policy, the host signal selects the state.
//
// Division of labour:
//   DIP (single, pin A8) = policy selector.
//     DIP off -> manual: DIP position selects the default layer (the original
//                Keychron behaviour, preserved through the weak hook in
//                q3_pro.c).
//     DIP on  -> auto: the host-side companion program decides the layer.
//
//   VIA custom value (channel 0, value_id 0x10) = state source.
//     SET: value_data[0] = target layer, value_data[1] = lease seconds
//          (0 -> AUTO_LEASE_DEFAULT).
//     GET: value_data[0] = mode (0 manual / 1 auto), [1] = last applied layer,
//          [2] = lease remaining seconds, [3] = signal present (0/1).
//     GET works in both modes for observability; SET is only accepted in auto.
//
//   Lease expiry falls back to AUTO_FALLBACK_LAYER, so losing the companion
//   program cannot leave the keyboard stuck on a host-chosen layer.
// ---------------------------------------------------------------------------

#ifdef VIA_ENABLE
#    include "via.h"

#    define AUTO_VALUE_ID        0x10
#    define AUTO_FALLBACK_LAYER  0
#    define AUTO_LEASE_DEFAULT   7

// Declared weak in q3_pro.c; this is the variant's strong definition.
bool q3p_dip_policy(uint8_t index, bool active);

static bool     auto_mode  = false;
static uint8_t  auto_layer = AUTO_FALLBACK_LAYER;
static uint8_t  auto_lease = AUTO_LEASE_DEFAULT;
static uint32_t auto_last  = 0;
static bool     auto_have  = false;

static void auto_expire(void) {
    if (auto_have && timer_elapsed32(auto_last) >= (uint32_t)auto_lease * 1000) {
        auto_have = false;
        if (auto_mode) default_layer_set(1UL << AUTO_FALLBACK_LAYER);
    }
}

bool q3p_dip_policy(uint8_t index, bool active) {
    if (index != 0) return false;

    auto_mode = active;

    if (!auto_mode) {
        // 手动模式撤销旧信号，重新进入自动模式必须收到新租约。
        auto_have = false;
        return false;
    }

    auto_expire();
    default_layer_set(1UL << (auto_have ? auto_layer : AUTO_FALLBACK_LAYER));
    return true;
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    if (length < 3) {
        if (length) data[0] = id_unhandled;
        return;
    }
    auto_expire();
    uint8_t *command_id   = &(data[0]);
    uint8_t *channel_id   = &(data[1]);
    uint8_t *value_id     = &(data[2]);
    uint8_t *value_data   = &(data[3]);

    if (*channel_id != id_custom_channel || *value_id != AUTO_VALUE_ID) {
        *command_id = id_unhandled;
        return;
    }

    if (*command_id == id_custom_get_value) {
        if (length < 7) {
            *command_id = id_unhandled;
            return;
        }
        uint32_t remain = 0;
        if (auto_have) {
            uint32_t elapsed = timer_elapsed32(auto_last);
            uint32_t lease_ms = (uint32_t)auto_lease * 1000;
            remain = (elapsed < lease_ms) ? lease_ms - elapsed : 0;
        }
        value_data[0] = auto_mode ? 1 : 0;
        value_data[1] = auto_layer;
        value_data[2] = (uint8_t)(remain / 1000);
        value_data[3] = auto_have ? 1 : 0;
        return;
    }

    if (*command_id == id_custom_set_value) {
        if (length < 5 || !auto_mode) {
            // The host does not own the layer while the DIP says manual.
            *command_id = id_unhandled;
            return;
        }

        uint8_t target = value_data[0];
        if (target >= DYNAMIC_KEYMAP_LAYER_COUNT) {
            *command_id = id_unhandled;
            return;
        }

        auto_layer = target;
        auto_lease = value_data[1] ? value_data[1] : AUTO_LEASE_DEFAULT;
        auto_last  = timer_read32();
        auto_have  = true;

        default_layer_set(1UL << target);
        return;
    }

    *command_id = id_unhandled;
}

void housekeeping_task_user(void) {
    auto_expire();
}
#endif
