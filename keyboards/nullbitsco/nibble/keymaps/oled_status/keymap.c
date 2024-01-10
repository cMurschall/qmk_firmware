/* Copyright 2021 Jose Luis Adelantado Torres
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

#ifdef OLED_ENABLE
#    include "oled_display.h"
#endif


#ifdef SNAKE_ENABLE
#    include "snake.h"
#endif


#ifdef CONWAY_ENABLE
#    include "conway.h"
#endif


// bool has_layer_changed = false;
// static uint8_t current_layer;

enum layer_names { _MA, _FN, _SNAKE, _CONWAY };

enum custom_keycodes {
    KC_CUST = SAFE_RANGE,
};


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_MA] = LAYOUT_ansi(
                KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
       KC_MUTE, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        KC_F10, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
         KC_F9, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_END,
         KC_F8, KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    KC_RALT, MO(_FN), KC_RCTL, KC_LEFT,          KC_DOWN, KC_RGHT
    ),
    [_FN] = LAYOUT_ansi(
                 QK_BOOT, KC_F1,   KC_F2,     KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,  KC_END,
        RGB_TOG, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        RGB_MOD, _______, _______,TO(_SNAKE), _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, TO(_CONWAY), _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______,                     _______,                   _______, _______, _______, _______,          _______, _______
    ),
    [_SNAKE] = LAYOUT_ansi(
                 TO(_MA), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_UP  , _______,
        _______, _______, _______, _______,                   _______,                   _______, _______, _______, KC_LEFT,          KC_DOWN, KC_RGHT
    ),
    [_CONWAY] = LAYOUT_ansi(
                 TO(_MA), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_UP  , _______,
        _______, _______, _______, _______,                   _______,                   _______, _______, _______, KC_LEFT,          KC_DOWN, KC_RGHT
    ),    
};
// clang-format on


void keyboard_post_init_user(void) {
   
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_timer         = timer_read32();
    toelter_logo_timer = timer_read32();
    toelter_pos_timer = timer_read32();

    set_oled_mode(OLED_MODE_IDLE);
    return OLED_ROTATION_0;
}

bool oled_task_user(void) {
    if (IS_LAYER_ON(_CONWAY)) {
        // oled_write_P(PSTR("conway \n"), false);
        render_conway_grid();
        return false;
    }
    if (IS_LAYER_ON(_SNAKE)) {
        // oled_write_P(PSTR("snake \n"), false);
        render_snake_game();
        return false;
    }
    if (IS_LAYER_ON(_MA) || IS_LAYER_ON(_FN)) {
        if (timer_elapsed(oled_timer) >= 3000) {
            set_oled_mode(OLED_MODE_IDLE);
        }
        render_status_frame();
    }
    return false;
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    if (IS_LAYER_OFF_STATE(state, _SNAKE)) {
#ifdef CONSOLE_ENABLE
        uprintf("snake deinit\n");
#endif
        snake_deinit();
    }

    if (IS_LAYER_OFF_STATE(state, _CONWAY)) {
#ifdef CONSOLE_ENABLE
        uprintf("conway deinit\n");
#endif
        conway_deinit();
    }

    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Send keystrokes to host keyboard, if connected (see readme)
    process_record_remote_kb(keycode, record);

#ifdef CONWAY_ENABLE
    if (IS_LAYER_ON(_CONWAY) && record->event.pressed) {
        process_record_conway(keycode);
        return true;
    }
#endif

#ifdef SNAKE_ENABLE
    if (IS_LAYER_ON(_SNAKE) && record->event.pressed) {
        process_record_snake(keycode);
        return true;
    }
#endif


    switch (keycode) {
        case RGB_TOG:
            if (record->event.pressed) {
#ifdef OLED_ENABLE
                process_record_keymap_oled(keycode);
#endif

            }
            break;
        case KC_CUST: // custom macro
            if (record->event.pressed) {
            }
            break;
    }

#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif 
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
#ifdef OLED_ENABLE
        process_record_encoder_oled(KC_VOLU);
#endif
    } else {
        tap_code(KC_VOLD);
#ifdef OLED_ENABLE
        process_record_encoder_oled(KC_VOLD);
#endif
    }


#ifdef SNAKE_ENABLE
    encoder_update_snake(index, clockwise);
#endif


    return true;
}

void matrix_init_user(void) {
    // Initialize remote keyboard, if connected (see readme)
    matrix_init_remote_kb();
}

void matrix_scan_user(void) {
    // Scan and parse keystrokes from remote keyboard, if connected (see readme)
    matrix_scan_remote_kb();

}
