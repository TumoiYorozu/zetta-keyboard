#include "../default/keymap.c"

#include "paw3204.h"
#include "pointing_device.h"

extern keymap_config_t keymap_config;



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

    init_paw3204();
}


report_mouse_t mouse_rep;

void keyboard_post_init_user() {
    debug_enable = true;
    debug_mouse = true;
}

uint16_t mouse_rem_x = 0; // x256
uint16_t mouse_rem_y = 0; // x256

void matrix_scan_user(void) {

#ifdef POINTING_DEVICE_ENABLE
    static int  cnt;
    static bool paw_ready;
    if (cnt++ % 50 == 0) {
        uint8_t pid = read_pid_paw3204();
        if (pid == 0x30) {
            //dprint("paw3204 OK\n");
            paw_ready = true;
        } else {
            dprintf("paw3204 NG:%d\n", pid);
            paw_ready = false;
        }
    }

    if (paw_ready) {
        uint8_t stat;
        int8_t x, y;

        read_paw3204(&stat, &x, &y);
        mouse_rep.buttons = 0;
        mouse_rep.h       = 0;
        mouse_rep.v       = 0;

        mouse_rep = pointing_device_get_report();

        uint16_t v = (uint16_t)x*x + (uint16_t)y*y;
        // uint16_t c = (
        //     v < 9 ? 1 :
        //     v < 16 ? 1 :
        //         4
        //     );
        if(v < 20){
            mouse_rem_x += x*2;
            mouse_rem_y += y*2;
        } else if (v < 40*40) {
            mouse_rem_x += x * 4;
            mouse_rem_y += y * 4;
        } else if (v < 80*80) {
            mouse_rem_x += x * 8;
            mouse_rem_y += y * 8;
        } else {
            mouse_rem_x += x * 16;
            mouse_rem_y += y * 16;
        }
#define MOUSE_THRESHOLD 4

        int16_t dx, dy;
        dx = (-MOUSE_THRESHOLD < mouse_rem_x && mouse_rem_x < MOUSE_THRESHOLD) ? 0 : mouse_rem_x;
        dy = (-MOUSE_THRESHOLD < mouse_rem_y && mouse_rem_y < MOUSE_THRESHOLD) ? 0 : mouse_rem_y;
        dx = (dx < -127*MOUSE_THRESHOLD ? -127*MOUSE_THRESHOLD : dx > 127*MOUSE_THRESHOLD ? 127*MOUSE_THRESHOLD : dx);
        dy = (dy < -127*MOUSE_THRESHOLD ? -127*MOUSE_THRESHOLD : dy > 127*MOUSE_THRESHOLD ? 127*MOUSE_THRESHOLD : dy);
        mouse_rem_x -= dx;
        mouse_rem_y -= dy;

        mouse_rep.x       =  dy / MOUSE_THRESHOLD;
        mouse_rep.y       = -dx / MOUSE_THRESHOLD;

        if (cnt % 10 == 0) {
            // dprintf("stat:%3d   x:%4d y:%4d  |  x:%4d y:%4d\n", stat, (int)x,(int)y, mouse_rep.x, mouse_rep.y);
        }
        // dprintf("stat:%3d x:%4d y:%4d\n", stat, mouse_rep.x, mouse_rep.y);

        if (stat & 0x80) {
            pointing_device_set_report(mouse_rep);
        }
    }
#undef MOUSE_THRESHOLD
#endif
}

