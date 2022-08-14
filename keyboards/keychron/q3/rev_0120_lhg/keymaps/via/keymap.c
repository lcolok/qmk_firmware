/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can      REDistribute it and/or modify
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

#include QMK_KEYBOARD_H

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    LEFT_HANDED_GAMING,
    WIN_FN,
    PLUS_ONE,
    PLUS_TWO,
    PLUS_THREE,
    PLUS_FOUR,
    PLUS_FIVE
};

enum custom_keycodes {
    KC_MISSION_CONTROL = USER00,
    KC_LAUNCHPAD,
    KC_LOPTN,
    KC_ROPTN,
    KC_LCMMD,
    KC_RCMMD,
    KC_SIRI,
    KC_TASK_VIEW,
    KC_FILE_EXPLORER,
    KC_SCREEN_SHOT,
    KC_CORTANA
};

#define KC_MCTL KC_MISSION_CONTROL
#define KC_LPAD KC_LAUNCHPAD
#define KC_TASK KC_TASK_VIEW
#define KC_FLXP KC_FILE_EXPLORER
#define KC_SNAP KC_SCREEN_SHOT
#define KC_CRTA KC_CORTANA

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

key_combination_t key_comb_list[4] = {
    {2, {KC_LWIN, KC_TAB}},
    {2, {KC_LWIN, KC_E}},
    {3, {KC_LSFT, KC_LCMD, KC_4}},
    {2, {KC_LWIN, KC_C}}
};

static uint8_t mac_keycode[4] = { KC_LOPT, KC_ROPT, KC_LCMD, KC_RCMD };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_87(MT(MOD_LCTL | MOD_LSFT | MOD_LALT | MOD_LGUI,KC_ESC),KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_NO,KC_NO,MO(PLUS_TWO),KC_GRV,KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_MINS,KC_EQL,KC_BSPC,KC_INS,KC_HOME,KC_PGUP,KC_TAB,KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_LBRC,KC_RBRC,KC_BSLS,KC_DEL,KC_END,KC_PGDN,LT(7,KC_ENT),KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,KC_QUOT,KC_ENT,KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,KC_RSFT,KC_UP,KC_LCTL,KC_LOPTN,KC_LCMMD,KC_SPC,KC_RCMMD,KC_ROPTN,MO(MAC_FN),KC_RCTL,KC_LEFT,KC_DOWN,KC_RGHT),

[MAC_FN] = LAYOUT_ansi_87(KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO),

[WIN_BASE] = LAYOUT_ansi_87(KC_ESC,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_PSCR,KC_CRTA,MO(PLUS_TWO),KC_GRV,KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_MINS,KC_EQL,KC_BSPC,KC_INS,KC_HOME,KC_PGUP,KC_TAB,KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_LBRC,KC_RBRC,KC_BSLS,KC_DEL,KC_END,KC_PGDN,LT(7,KC_ENT),KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,KC_QUOT,KC_ENT,KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,KC_RSFT,KC_UP,KC_LCTL,KC_LALT,KC_LCTL,KC_SPC,KC_RCTL,KC_RALT,MO(LEFT_HANDED_GAMING),KC_RGUI,KC_LEFT,KC_DOWN,KC_RGHT),

[LEFT_HANDED_GAMING] = LAYOUT_ansi_87(KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_VOLD,KC_VOLU,TO(4),KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO),

[WIN_FN] = LAYOUT_ansi_87(KC_ESC,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_PSCR,KC_CRTA,MO(PLUS_TWO),KC_GRV,KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_MINS,KC_EQL,KC_BSPC,KC_INS,KC_HOME,KC_PGUP,KC_TAB,KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_LBRC,KC_RBRC,KC_BSLS,KC_DEL,KC_END,KC_PGDN,LT(7,KC_ENT),KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,KC_QUOT,KC_ENT,KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,KC_RSFT,KC_UP,KC_LCTL,KC_LGUI,KC_LALT,KC_SPC,KC_RALT,KC_RGUI,MO(PLUS_ONE),KC_RCTL,KC_LEFT,KC_DOWN,KC_RGHT),

[PLUS_ONE] = LAYOUT_ansi_87(KC_NO,KC_BRID,KC_BRIU,KC_MCTL,KC_LPAD,RGB_VAD,RGB_VAI,KC_MPRV,KC_MPLY,KC_MNXT,KC_MUTE,KC_VOLD,KC_VOLU,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO),

[PLUS_TWO] = LAYOUT_ansi_87(KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,RGB_TOG,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,RGB_SPI,RGB_SAI,RGB_MOD,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,RGB_SPD,RGB_SAD,RGB_RMOD,KC_NO,A(LWIN(KC_LEFT)),KC_NO,A(LWIN(KC_RGHT)),KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,RGB_VAI,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,RGB_HUD,RGB_VAD,RGB_HUI),

[PLUS_THREE] = LAYOUT_ansi_87(KC_MS_BTN2,KC_MS_BTN3,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NLCK,KC_P7,KC_P8,KC_P9,KC_BSPC,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_PAST,KC_NO,KC_NO,KC_PMNS,KC_PPLS,KC_TRNS,KC_P4,KC_P5,KC_P6,MO(PLUS_ONE),KC_PGUP,KC_UP,KC_PGDN,KC_HOME,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_P1,KC_P2,KC_P3,KC_NO,KC_LEFT,KC_DOWN,KC_RGHT,KC_END,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_ENT,KC_TRNS,C(S(KC_Z)),C(S(KC_X)),C(S(KC_C)),KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_PSLS,KC_NO,KC_NO,KC_TRNS,KC_TRNS,KC_TRNS,MO(PLUS_TWO),KC_NO,KC_NO,KC_NO,KC_NO,KC_P0,KC_PDOT,KC_PENT),

[PLUS_FOUR] = LAYOUT_ansi_87(KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO),

[PLUS_FIVE] = LAYOUT_ansi_87(KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO),
};

#define WHITE    {0,0,255}
#define ORANGE   {21,255,255}
#define BLUE     {169,255,255}
#define GREEN    {85,255,255}
#define RED      {0,255,255}
#define YELLOW   {43,255,255}
#define NO_COLOR {0,0,0}
#define LayerNum WHITE

const uint8_t PROGMEM rgb_matrix_led_map[][DRIVER_LED_TOTAL][5] = {
    { //layer 0 MAC_BASE
        LayerNum,              NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              WHITE                                ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }, { //layer 1 MAC_FN
        NO_COLOR,              LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              WHITE                                ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }, { //layer 2 WIN_BASE
        NO_COLOR,              NO_COLOR,  LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              ORANGE                               ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }, { //layer 3 LEFT_HANDED_GAMING
        NO_COLOR,              NO_COLOR,  NO_COLOR,  LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  BLUE    ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, BLUE      ,  BLUE    ,  BLUE    ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  BLUE    ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  BLUE    ,  BLUE    ,  BLUE    ,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             BLUE    ,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              BLUE                                 ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  BLUE    ,  BLUE    ,  BLUE
    }, { //layer 4 WIN_FN
           GREEN,                 GREEN,     GREEN,     GREEN,  LayerNum,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,
           GREEN,    GREEN  ,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,
           GREEN,    GREEN  ,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,
           GREEN,    GREEN  ,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN           ,
           GREEN            ,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN,     GREEN           ,                GREEN,
           GREEN,      GREEN,     GREEN,                           GREEN                                 ,     GREEN,     GREEN,     GREEN,     GREEN           ,     GREEN,     GREEN,     GREEN
    }, { //layer 5
        NO_COLOR,              NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              WHITE                                ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }, { //layer 6
        NO_COLOR,              NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              WHITE                                ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }, { //layer 7
             RED,                   RED,       RED,       RED,       RED,       RED,       RED,  LayerNum,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,
             RED,      RED  ,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,
             RED,      RED  ,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,
             RED,      RED  ,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED           ,
             RED            ,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED,       RED           ,                  RED,
             RED,        RED,       RED,                             RED                                            ,       RED,       RED,       RED,       RED,       RED,       RED,       RED
    }, { //layer 8
        NO_COLOR,              NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              WHITE                                ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }, { //layer 9
        NO_COLOR,              NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  LayerNum,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,
        NO_COLOR, NO_COLOR  ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,
        NO_COLOR            ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,             NO_COLOR,
        NO_COLOR, NO_COLOR,  NO_COLOR,                              WHITE                                ,  NO_COLOR,  NO_COLOR,  NO_COLOR,  NO_COLOR           ,  NO_COLOR,  NO_COLOR,  NO_COLOR
    }
};

void set_layer_color(int layer) {

  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&rgb_matrix_led_map[layer][i][0]),
      .s = pgm_read_byte(&rgb_matrix_led_map[layer][i][1]),
      .v = pgm_read_byte(&rgb_matrix_led_map[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        //rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

uint16_t prev;
bool siri;

void matrix_scan_user(void) {
    if (siri) {
        if (timer_elapsed(prev) >= 500) {
            siri = false;
            unregister_code(KC_LCMD);
            unregister_code(KC_SPACE);
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_MISSION_CONTROL:
            if (record->event.pressed) {
                host_consumer_send(0x29F);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        case KC_LAUNCHPAD:
            if (record->event.pressed) {
                host_consumer_send(0x2A0);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        case KC_LOPTN:
        case KC_ROPTN:
        case KC_LCMMD:
        case KC_RCMMD:
            if (record->event.pressed) {
                register_code(mac_keycode[keycode - KC_LOPTN]);
            } else {
                unregister_code(mac_keycode[keycode - KC_LOPTN]);
            }
            return false;  // Skip all further processing of this key
        case KC_SIRI:
            if (record->event.pressed) {
                register_code(KC_LCMD);
                register_code(KC_SPACE);
                prev = timer_read() | 1;
                siri = true;
            } else {
                // Do something else when release
            }
            return false;  // Skip all further processing of this key
        case KC_TASK:
        case KC_FLXP:
        case KC_SNAP:
        case KC_CRTA:
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    register_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    unregister_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            }
            return false;  // Skip all further processing of this key
        default:
            return true;  // Process all other keycodes normally
    }
}

void rgb_matrix_indicators_user(void) {
    set_layer_color(biton32(layer_state));
}
