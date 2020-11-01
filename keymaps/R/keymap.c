#include "../default/keymap.c"

#include "paw3204.h"
#include "pointing_device.h"

extern keymap_config_t keymap_config;


/*
#define MOUSE_PAW3204_SCLK D0
#define MOUSE_PAW3204_DATA D1

#define SCROL_PAW3204_SCLK F7
#define SCROL_PAW3204_DATA F6
/*/
#define MOUSE_PAW3204_SCLK F7
#define MOUSE_PAW3204_DATA F6

#define SCROL_PAW3204_SCLK D0
#define SCROL_PAW3204_DATA D1
//*/

void rgb_led_monitor();
void rgb_led_monitor_init();


void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif

    init_paw3204(MOUSE_PAW3204_SCLK, MOUSE_PAW3204_DATA);
    init_paw3204(SCROL_PAW3204_SCLK, SCROL_PAW3204_DATA);
    rgb_led_monitor_init();
}


report_mouse_t mouse_rep;

void keyboard_post_init_user() {
    debug_enable = true;
    debug_mouse = true;
}

int16_t mouse_rem_x = 0; // x256
int16_t mouse_rem_y = 0; // x256
int16_t scrol_rem_x = 0; // x256
int16_t scrol_rem_y = 0; // x256


void matrix_scan_user(void) {
    rgb_led_monitor();

    /*
    static int  cnt_m, cnt_s;
    static bool paw_ready_m, paw_ready_s;
    // rgblight_task();
    if (cnt_m++ % 50 == 0) {
        uint8_t pid = read_pid_paw3204(MOUSE_PAW3204_SCLK, MOUSE_PAW3204_DATA);
        if (pid == 0x30) {
            //dprint("paw3204 OK\n");
            paw_ready_m = true;
        } else {
            // dprintf("paw3204 NG:%d\n", pid);
            paw_ready_m = false;
        }
    }
    if (cnt_s++ % 50 == 0) {
        uint8_t pid = read_pid_paw3204(SCROL_PAW3204_SCLK, SCROL_PAW3204_DATA);
        if (pid == 0x30) {
            //dprint("paw3204 OK\n");
            paw_ready_s = true;
        } else {
            // dprintf("paw3204 NG:%d\n", pid);
            paw_ready_s = false;
        }
    }
    if (paw_ready_m)  {
        uint8_t stat;
        int8_t x, y;

        read_paw3204(&stat, &x, &y, MOUSE_PAW3204_SCLK, MOUSE_PAW3204_DATA);

        mouse_rep = pointing_device_get_report();

        int16_t v = (int16_t)x*x + (int16_t)y*y;
        int16_t c = (
            v < 20    ? 5 :
            v < 40*40 ? 12:
            v < 80*80 ? 18:
                        24);

        mouse_rem_x += x * c;
        mouse_rem_y += y * c;

#define MOUSE_THRESHOLD 8

        int16_t dx, dy;
        dx = (-MOUSE_THRESHOLD < mouse_rem_x && mouse_rem_x < MOUSE_THRESHOLD) ? 0 : mouse_rem_x;
        dy = (-MOUSE_THRESHOLD < mouse_rem_y && mouse_rem_y < MOUSE_THRESHOLD) ? 0 : mouse_rem_y;
        dx = (dx < -127*MOUSE_THRESHOLD ? -127*MOUSE_THRESHOLD : dx > 127*MOUSE_THRESHOLD ? 127*MOUSE_THRESHOLD : dx);
        dy = (dy < -127*MOUSE_THRESHOLD ? -127*MOUSE_THRESHOLD : dy > 127*MOUSE_THRESHOLD ? 127*MOUSE_THRESHOLD : dy);
        mouse_rem_x -= dx;
        mouse_rem_y -= dy;

        mouse_rep.x       =  dy / MOUSE_THRESHOLD;
        mouse_rep.y       = -dx / MOUSE_THRESHOLD;

        if (cnt_m % 10 == 0) {
            // dprintf("stat:%3d   x:%4d y:%4d  |  x:%4d y:%4d\n", stat, (int)x,(int)y, mouse_rep.x, mouse_rep.y);
        }
        // dprintf("stat:%3d x:%4d y:%4d\n", stat, mouse_rep.x, mouse_rep.y);

        if (stat & 0x80) {
            pointing_device_set_report(mouse_rep);
        }
    }
#undef MOUSE_THRESHO
#define SCROL_MIN         3
#define SCROL_THRESHOLD_Y 32
#define SCROL_THRESHOLD_X 32

    while (paw_ready_s && false) {
        uint8_t stat;
        int8_t x, y;

        read_paw3204(&stat, &y, &x, SCROL_PAW3204_SCLK, SCROL_PAW3204_DATA);
        if ((stat & 0x80) == 0) break;

        int8_t ax = abs(x);
        int8_t ay = abs(y);
        if(ax < SCROL_MIN && ay < SCROL_MIN) {
            scrol_rem_x = scrol_rem_y = 0;
            break;
        }
        if(ax >= ay) {
            x += (x > 0 ? -SCROL_MIN : SCROL_MIN);
            if(ax >= 0)  scrol_rem_x += x;
            if(ax >= 15) scrol_rem_x += x;
        } else {
            y += (y > 0 ? -SCROL_MIN : SCROL_MIN);
            if(ay >= 0)  scrol_rem_y += y;
            if(ay >= 15) scrol_rem_y += y;
            if(ay >= 30) scrol_rem_y += y;
            if(ay >= 45) scrol_rem_y += y;
            if(ay >= 60) scrol_rem_y += y;
            // for(int i = 1; i < 25; ++i) { if(ay >= i * 5) dprintf("%d", i); } if(ay >= 5) dprintf("\n");
        }
        int16_t dx = scrol_rem_x / SCROL_THRESHOLD_X;
        int16_t dy = scrol_rem_y / SCROL_THRESHOLD_Y;
        if(dx | dy) {
            mouse_rep = pointing_device_get_report();
            mouse_rep.h += dx;
            mouse_rep.v += dy;

            scrol_rem_x -= dx * SCROL_THRESHOLD_X;
            scrol_rem_y -= dy * SCROL_THRESHOLD_Y;

            pointing_device_set_report(mouse_rep);
        }
        break;
    }
#undef SCROL_MIN
#undef SCROL_THRESHOLD_X
#undef SCROL_THRESHOLD_Y
    //*/
}
