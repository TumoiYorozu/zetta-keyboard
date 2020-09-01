#include "../default/keymap.c"


#include "lufa.h"
#include "ssd1306.h"

void matrix_init_user(void) {
    iota_gfx_init(false);
}
void matrix_scan_user(void) {
    iota_gfx_task();
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
    if (memcmp(dest->display, source->display, sizeof(dest->display))) {
        memcpy(dest->display, source->display, sizeof(dest->display));
        dest->dirty = true;
    }
}
void iota_gfx_task_user(void) {
    struct CharacterMatrix matrix;
    matrix_clear(&matrix);
    matrix_write_P(&matrix, PSTR("Hello World!"));
    matrix_update(&display, &matrix);
}

void matrix_master_OLED_init(void){

}
