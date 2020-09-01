/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "split_util.h"
#include "config.h"
#include "transport.h"

#include "debug.h"

#define ERROR_DISCONNECT_COUNT 5

#define ROWS_PER_HAND (MATRIX_ROWS / 2)

static pin_t row_pins[MATRIX_NUM_PIN] = MATRIX_PINS;

extern matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
extern matrix_row_t matrix[MATRIX_ROWS];      // debounced values

// matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
// matrix_row_t matrix[MATRIX_ROWS];      // debounced values

// matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
// matrix_row_t matrix[MATRIX_ROWS];      // debounced values


uint8_t thisHand, thatHand, pinNum;

// user-defined overridable functions
__attribute__((weak)) void matrix_slave_scan_user(void) {}

// matrix_row_t matrix_get_row(uint8_t row) {
//     // TODO: return the requested row data
//     return 0;
// }

// void matrix_print(void) {
//     // TODO: use print() to dump the current matrix state to console
// }


static void init_pins(void) {
    // unselect_rows();
    // for (uint8_t x = 0; x < MATRIX_COLS; x++) {
    //     setPinInputHigh(col_pins[x]);
    // }

    for (uint8_t x = 0; x < pinNum; x++) {
        setPinInputHigh(row_pins[x]);
    }
}
static void select_row(uint8_t row) {
    setPinOutput(row_pins[row]);
    writePinLow(row_pins[row]);
}

static void unselect_row(uint8_t row) { setPinInputHigh(row_pins[row]); }


static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {

    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;
    select_row(current_row);
    // matrix_io_delay();
    wait_us(30);

    for (uint8_t idx = 0; idx < pinNum; idx++) {
        if (idx == current_row) continue;

        // Select the col pin to read (active low)
        uint8_t pin_state = readPin(row_pins[idx]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << idx);
    }
    unselect_row(current_row);
    if (current_matrix[current_row] != current_row_value) {
        current_matrix[current_row] = current_row_value;
        return true;
    }

    //debug_enable = true;
    //dprintf("%d : %d\n", (int)current_row, (int)current_row_value);

    /*
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    // Select row and wait for row selecton to stabilize
    select_row(current_row);
    matrix_io_delay();

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        // Select the col pin to read (active low)
        uint8_t pin_state = readPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
    }

    // Unselect row
    unselect_row(current_row);

    // If the row has changed, store the row and return the changed flag.
    if (current_matrix[current_row] != current_row_value) {
        current_matrix[current_row] = current_row_value;
        return true;
    }
    */
    return false;
}


void matrix_post_scan(void) {
    if (is_keyboard_master()) {
        static uint8_t error_count;

        if (!transport_master(matrix + thatHand)) {
            error_count++;

            if (error_count > ERROR_DISCONNECT_COUNT) {
                // reset other half if disconnected
                for (int i = 0; i < ROWS_PER_HAND; ++i) {
                    matrix[thatHand + i] = 0;
                }
            }
        } else {
            error_count = 0;
        }
        //debug_enable = true; dprintf("error_count %d\n", (int)error_count);

        matrix_scan_quantum();
    } else {
        transport_slave(matrix + thisHand);

        matrix_slave_scan_user();
    }
}

void matrix_init(void) {
    split_pre_init();
    thisHand = isLeftHand ? 0 : (ROWS_PER_HAND);
    thatHand = ROWS_PER_HAND - thisHand;
    pinNum   = isLeftHand ? MATRIX_NUM_PIN_LEFT : MATRIX_NUM_PIN_RIGHT;

    init_pins();
    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        raw_matrix[i] = 0;
        matrix[i]     = 0;
    }

    debounce_init(ROWS_PER_HAND);
    matrix_init_quantum();
    split_post_init();
}

uint8_t matrix_scan(void) {
    bool changed = false;


    //debug_enable = true;
    //dprintf("isLeftHand : %d\n", isLeftHand);
    //dprintf("is_keyboard_master : %d\n", is_keyboard_master());


    for (uint8_t current_row = 0; current_row < ROWS_PER_HAND; current_row++) {
        changed |= read_cols_on_row(raw_matrix, current_row);
    }

    debounce(raw_matrix, matrix + thisHand, ROWS_PER_HAND, changed);

    matrix_post_scan();
    return (uint8_t)changed;
}
