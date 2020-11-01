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
#include "pointing_device.h"
#include "raw_hid.h"

#ifdef RGBLIGHT_ENABLE
#include "rgblight.h"
extern rgblight_config_t rgblight_config;
int RGB_current_mode;
#endif

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN,
    _RG
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL,
    KC_WIN_CTRL_L,
    KC_WIN_CTRL_R,
    KC_WIN_TAB,
    KC_SLP_RST,
};

#define KC__n      KC_NO
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

#define KC_DM_REC1   DM_REC1
#define KC_DM_REC2   DM_REC2
#define KC_DM_PLY1   DM_PLY1
#define KC_DM_PLY2   DM_PLY2
#define KC_DM_RSTP   DYN_REC_STOP

#define KC_RESET     RESET
#define KC_EEP_RST   EEPROM_RESET

#define KC_SH(key)   LSFT(KC_##key)
#define KC_AL(key)   LALT(KC_##key)
#define KC_RW(key)   RWIN_T(KC_##key)
#define KC__LT(layer,key)   LT(layer,KC_##key)
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
        LCTL, A, S, D, F, G, _n,
        LSFT, Z, X, C, V, B, MS_BTN3,
        F5,_LT(_RG,PSCR), LWIN, RALT, SPC, MO(_FN), _n,
        DM_PLY1, DM_PLY2, WIN_TAB, WIN_CTRL_L, WIN_CTRL_R, MS_BTN1, MS_BTN2,

              6,       7, 8, 9, 0,        MINS,  EQL, JYEN, BSPC,       HOME, END, PSLS, PAST,
           MUTE,       Y, U, I, O,           P,  LBRC, RBRC, ENT,         P7,  P8,   P9, PMNS,
              G,       H, J, K, L,        SCLN, QUOT, NUHS,               P4,  P5,   P6, PPLS,
        MS_BTN3,       B, N, M, COMM,      DOT, SLSH,   RO, RSFT,         P1,  P2,   P3, PENT,
        MS_BTN2, MS_BTN1, SPC, MO(_FN), RW(NO), RCTL, PGUP, PGDN,       UP, P0, PDOT,
        WIN_CTRL_L, WIN_CTRL_R, WIN_TAB, WWW_BACK, WWW_FORWARD, F12,  LEFT, DOWN, RGHT
    ),

    [_FN] = LAYOUT_kc(
           ZKHK,      F1,         F2,   F3,   F4,   F5,  F6,
          PAUSE,    SCLN,       PGUP,  END,   UP,  ENT, ENT,
               ,    HOME,       PGDN, LEFT, DOWN, RGHT,    ,
               ,WWW_BACK,WWW_FORWARD,   RO,  DEL, BSPC,    ,
           PSCR,        ,           ,     ,     ,     ,    ,
        DM_REC1, DM_REC2,    DM_RSTP,     ,     ,     ,    ,

          F6,      F7,      F8, F9,     F10,    F11,  F12,  F12,  DEL,       NLCK, INS, PAUSE, SLP_RST,
            , RGB_HUI,    MS_U, _n,      UP,   BSPC,  DEL,     ,     ,           ,    ,      ,        ,
        RGB_HUD, MS_L,    MS_R,  LEFT, RGHT,       ,     ,  ENT,                 ,    ,      ,        ,
            , RGB_MOD,    MS_D, _n,    DOWN, SH(RO),     ,     ,     ,           ,    ,      ,        ,
            ,        ,        ,   ,        ,       ,     ,     ,       TRNS,          ,      ,
            ,        ,        ,   ,        ,       ,             TRNS, TRNS, TRNS
    ),

    [_RG] = LAYOUT_kc(
              7,       8,          9,    0, MINS,  EQL,JYEN,
              U,       I,          O,    P, LBRC, RBRC,NUHS,
               ,       H,          J,    K,    L, SCLN,    ,
               ,       N,          M, COMM,  DOT, SLSH,    ,
            F12,        ,           ,     ,     ,     ,    ,
               ,        ,           ,     ,     ,     ,    ,

            ,        ,        ,   ,        ,       ,     ,     ,     ,           ,    ,      ,        ,
            ,        ,        ,   ,        ,       ,     ,     ,     ,           ,    ,      ,        ,
            ,        ,        ,   ,        ,       ,     ,     ,                 ,    ,      ,        ,
            ,        ,        ,   ,        ,       ,     ,     ,     ,           ,    ,      ,        ,
            ,        ,        ,   ,        ,       ,     ,     ,       TRNS,          ,      ,
            ,        ,        ,   ,        ,       ,             TRNS, TRNS, TRNS
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

#ifdef POINTING_DEVICE_ENABLE
void my_mouse_rot(uint8_t key) {
    report_mouse_t currentReport = pointing_device_get_report();
    if (key == KC_WH_U) currentReport.v++;
    if (key == KC_WH_D) currentReport.v--;
    if (key == KC_WH_R) currentReport.h++;
    if (key == KC_WH_L) currentReport.h--;
    pointing_device_set_report(currentReport);
}
#endif

void encoder_update_user(uint8_t  index, bool clockwise) {
#ifdef POINTING_DEVICE_ENABLE
    debug_enable = true; dprintf("encoder_update_user: %d %d\n", (int)index, (int)clockwise);
    if (index == 0) {
        if (clockwise) {
            my_mouse_rot(KC_MS_WH_RIGHT);
        } else {
            my_mouse_rot(KC_MS_WH_LEFT);
        }
    } else if (index == 1){
        if (clockwise) {
            my_mouse_rot(KC_MS_WH_DOWN);
        } else {
            my_mouse_rot(KC_MS_WH_UP);
        }
    } else if (index == 2){
        if (clockwise) {
            tap_code(KC_AUDIO_VOL_UP);
        } else {
            tap_code(KC_AUDIO_VOL_DOWN);
        }
    }
#endif
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
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
                unregister_code(KC_LWIN);
            }
            return false;
        case KC_SLP_RST:
            tap_code(KC_SLEP);
            reset_keyboard();
            return false;

#ifdef POINTING_DEVICE_ENABLE
        case KC_MS_BTN1:
        case KC_MS_BTN2:
        case KC_MS_BTN3:
        {
            report_mouse_t currentReport = pointing_device_get_report();
            uint8_t btn = 1 << (keycode - KC_MS_BTN1);
            if (record->event.pressed) {
                currentReport.buttons |= btn;
            } else {
                currentReport.buttons &= ~btn;
            }
            pointing_device_set_report(currentReport);
            return false;
        }
#endif
    }
    return true;
}

void rgb_led_monitor_init() {
    //Serial.begin(115200);
}

/*

30 fps
98 LEDs
 3 channel
 1 byte/channel




120*32 = 3840 Byte
30 fps
98 LEDs
 1 byte/pix


32byte * 8bit = 256bit
(256-2)/ceil(98/4) = 254/25 = 10.16 bit/pix

10^3 = 1000
8^3 = 512

*/


void rgb_led_monitor() {
}


void set_serial_m2s_buffer_led_dat(uint8_t i, uint8_t v);

void decode_and_set_led(uint8_t val, void *led1) {
    //sethsv_raw(hue, sat, val > RGBLIGHT_LIMIT_VAL ? RGBLIGHT_LIMIT_VAL : val, led1);
    if (1) {
        uint16_t b = val % 6;
        uint16_t g = val / 6 % 6;
        uint16_t r = val / 36;
        b = (b*(RGBLIGHT_LIMIT_VAL*8/5)/8);
        g = (g*(RGBLIGHT_LIMIT_VAL*8/5)/8);
        r = (r*(RGBLIGHT_LIMIT_VAL*8/5)/8);
        setrgb(r, g, b, (LED_TYPE *)led1);
    } else {
        uint16_t g = val & 0b111;
        uint16_t r = (val >> 3) & 0b111;
        uint16_t b = val >> 6;
        g = (g*(RGBLIGHT_LIMIT_VAL*8/7)/8);
        r = (r*(RGBLIGHT_LIMIT_VAL*8/7)/8);
        b = (b*(RGBLIGHT_LIMIT_VAL*4/3)/4);
        // g = g * g * RGBLIGHT_LIMIT_VAL / 49;
        // r = r * r * RGBLIGHT_LIMIT_VAL / 49;
        // b = b * b * RGBLIGHT_LIMIT_VAL / 9;
        // g = g * (g*14-g*g)*RGBLIGHT_LIMIT_VAL/343;
        // r = r * (r*14-r*r)*RGBLIGHT_LIMIT_VAL/343;
        // b = b * (b*6 -b*b)*RGBLIGHT_LIMIT_VAL/81;
        setrgb(r, g, b, (LED_TYPE *)led1);
    }
}

static void set_led_rgb(RGB x, uint16_t val, LED_TYPE *led1) {
    setrgb(x.r * val / 16, x.g * val / 16, x.b * val / 16, (LED_TYPE *)led1);
}

// 受信イベント関数
void raw_hid_receive(uint8_t *data, uint8_t length) {
#ifndef RGBLIGHT_DISPLAY
    return;
#endif
    const int pix_per_line = 30;
    //dprintf("RGBLED_NUM_R %d\n", RGBLED_NUM_R);
    uint8_t line = data[0];
    uint8_t isGray = (line >= 128);
    line -= isGray * 128;
    uint8_t update = 0;
    if (isGray) {
        int i = 1;
        int j = line * pix_per_line * 2;
        HSV hsv = {rgblight_config.hue, rgblight_config.sat, RGBLIGHT_LIMIT_VAL};
        RGB rgb = hsv_to_rgb(hsv);
        // dprintf("[%3d %3d %3d]\n", (int)(rgb.r), (int)(rgb.g), (int)(rgb.b));
        for(; i < length; ++i){
            if (j >= RGBLED_NUM_L) break;
            set_serial_m2s_buffer_led_dat(j, data[i]);
            j+=2;
        }
        if (j == RGBLED_NUM_L + 1){
            set_led_rgb(rgb, data[i-1]>>4, (LED_TYPE *)&led[j-1]);
            update = 1;
        }
        for(; i < length; ++i){
            if (j >= RGBLED_NUM) break;
            set_led_rgb(rgb, data[i]&15, (LED_TYPE *)&led[j]); ++j;
            set_led_rgb(rgb, data[i]>>4, (LED_TYPE *)&led[j]); ++j;
            update = 1;
        }
    } else {
        int j = line * pix_per_line;
        for(int i = 1; i < length; ++i){
            if (j >= RGBLED_NUM) break;
            if (j < RGBLED_NUM_L) {
                set_serial_m2s_buffer_led_dat(j, data[i]);
            } else {
                decode_and_set_led(data[i], &led[j]);
                update = 1;
            }
            ++j;
        }
    }
    if(update) {
        rgblight_set();
    }
}

