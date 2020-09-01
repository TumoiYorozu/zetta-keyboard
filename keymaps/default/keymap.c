/* Copyright 2020 TumoiYorozu
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
#include QMK_KEYBOARD_H

#include "debug.h"

#ifdef RGBLIGHT_ENABLE
#include "rgblight.h"
extern rgblight_config_t rgblight_config;
int RGB_current_mode;
#endif

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL,
    KC_WIN_CTRL_L,
    KC_WIN_CTRL_R,
    KC_WIN_TAB,
};

#define KC_MO MO
#define KC_BL_TOGG BL_TOGG
#define KC_BL_BRTG BL_BRTG
#define KC_BL_INC  BL_INC
#define KC_BL_DEC  BL_DEC

#define KC_RGB_M_P   RGB_M_P
#define KC_RGB_M_B   RGB_M_B
#define KC_RGB_M_R   RGB_M_R
#define KC_RGB_M_SW  RGB_M_SW
#define KC_RGB_M_SN  RGB_M_SN
#define KC_RGB_M_K   RGB_M_K
#define KC_RGB_M_X   RGB_M_X
#define KC_RGB_M_G   RGB_M_G
#define KC_RGB_HUI   RGB_HUI
#define KC_RGB_HUD   RGB_HUD
#define KC_RGB_SAI   RGB_SAI
#define KC_RGB_SAD   RGB_SAD
#define KC_RGB_VAI   RGB_VAI
#define KC_RGB_VAD   RGB_VAD
#define KC_RGB_TOG   RGB_TOG
#define KC_RGB_MOD   RGB_MOD

#define KC_RESET     RESET
/*
      RGB_M_P,
      RGB_M_B,
      RGB_M_R,
      RGB_M_SW,
      RGB_M_SN,
      RGB_M_K,
      RGB_M_X,
      RGB_M_G,
      RGB_HUI,
      RGB_HUD,
      RGB_SAI,
      RGB_SAD,
      RGB_VAI,
      RGB_VAD,
      RGB_TOG,
      RGB_MOD,
      */


#define KC_ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT_kc(
        ESC, 1, 2, 3, 4, 5, 6,
        TAB, Q, W, E, R, T, Y,
        LCTL, A, S, D, F, G, NO,
        LSFT, Z, X, C, V, B, BTN3,
        F5, PSCR, LWIN, RALT, SPC, MO(_FN), NO,
           F1, F2, WIN_TAB, WIN_CTRL_L, WIN_CTRL_R, BTN1, BTN2,

           6,    7, 8, 9, 0,   MINS,  EQL, JYEN, BSPC,       HOME, END, PSLS, PAST,
        MUTE,    Y, U, I, O,      P, LBRC, RBRC,  ENT,         P7,  P8,   P9, PMNS,
           G,    H, J, K, L,   SCLN, QUOT, NUHS,               P4,  P5,   P6, PPLS,
        MUTE,    B, N, M, COMM, DOT, SLSH,   RO, RSFT,         P1,  P2,   P3, PENT,
        BTN2, BTN1, SPC, MO(_FN), RWIN, RCTL, PGUP, PGDN,       UP, P0, PDOT,
        WIN_CTRL_L, WIN_CTRL_R, WIN_TAB, F10, F11, F12,  LEFT, DOWN, RGHT
    ),


    [_FN] = LAYOUT_kc(
        ZKHK,  F1, F2, F3, F4, F5, F6,
        CAPS,    ,   ,END,   ,   ,   ,
            ,HOME,   ,   ,   ,   ,   ,
            ,    ,   ,   ,   ,   ,   ,
            ,    ,   ,   ,   ,   ,   ,
            ,    ,   ,   ,   ,   ,   ,

          F6, F7,F8, F9,     F10,    F11,  F12,     ,  DEL,        INS,    ,     ,     ,
            ,   ,  ,   ,      UP,       ,     ,     ,     ,           ,    ,     ,     ,
            ,   ,  ,  LEFT, RGHT,       ,     ,  ENT,                 ,    ,     ,     ,
            ,   ,  ,   ,    DOWN,       ,     ,     ,     ,           ,    ,     ,     ,
            ,   ,  ,   ,        ,       ,     ,     ,       TRNS,          ,     ,
            ,   ,  ,   ,        ,       ,             TRNS, TRNS, TRNS
    )


    /*
    [_FN] = LAYOUT_kc(
        ZKHK, F1, F2, F3, F4, F5, F6,
        CAPS, RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,
        LCTL, RGB_M_X,RGB_M_G,RGB_HUI,RGB_HUD,RGB_SAI, NO,
        LSFT,  RGB_SAD,RGB_VAI,RGB_VAD,RGB_TOG,RGB_MOD, BTN3,
        PSCR, F5, LWIN, RALT, SPC, NO, NO,
           F1, F2, F3, F4, F6, BTN1, BTN2,

        F6, F7,F8,F9,F10,   F11,  F12, JYEN,  DEL,                          HOME, END, PSLS, PAST,
        NO,  RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,RGB_M_X, ENT,  P7,  P8,   P9, PMNS,
         RGB_M_G,RGB_HUI,RGB_HUD,RGB_SAI,RGB_SAD,RGB_VAI,RGB_VAD,RGB_TOG,     P4,  P5,   P6, PPLS,
        NO, RGB_MOD, N, M, BL_TOGG, BL_BRTG, BL_INC, BL_DEC, RSFT,       P1,  P2,   P3, PENT,
        NO, NO, SPC, NO, RWIN, RCTL, PGUP, PGDN,    MS_U,     P0, PDOT,
        F7, F8, F9, F10, F11, F12,            MS_L, MS_D, MS_R
    )
    //*/
};


void encoder_update_user(uint8_t  index, bool clockwise) {

    debug_enable = true; dprintf("encoder_update_user: %d %d\n", (int)index, (int)clockwise);

    if (index == 0) {
        if (clockwise) {
            tap_code(KC_MS_WH_RIGHT);
        } else {
            tap_code(KC_MS_WH_LEFT);
        }
    } else if (index == 1){
        if (clockwise) {
            tap_code(KC_MS_WH_DOWN);
        } else {
            tap_code(KC_MS_WH_UP);
        }
    } else if (index == 2){
        if (clockwise) {
            tap_code(KC_AUDIO_VOL_UP);
        } else {
            tap_code(KC_AUDIO_VOL_DOWN);
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    debug_enable = true;
    dprintf("keycode: %d\n", (int)keycode);

    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/\n");
            } else {
                // when keycode QMKURL is released
            }
            break;
        case KC_WIN_CTRL_L:
            if (record->event.pressed) {
                register_code(KC_LCTRL);
                register_code(KC_LWIN);
                register_code(KC_LEFT);
            } else {
                unregister_code(KC_LEFT);
                unregister_code(KC_LWIN);
                unregister_code(KC_LCTRL);
            }
            return false;
        case KC_WIN_CTRL_R:
            if (record->event.pressed) {
                register_code(KC_LCTRL);
                register_code(KC_LWIN);
                register_code(KC_RIGHT);
            } else {
                unregister_code(KC_RIGHT);
                unregister_code(KC_LWIN);
                unregister_code(KC_LCTRL);
            }
            return false;
        case KC_WIN_TAB:
            if (record->event.pressed) {
                register_code(KC_LWIN);
                tap_code(KC_TAB);
                unregister_code(KC_LWIN);
                return false;
            }
    }
    return true;
}
