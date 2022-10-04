/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

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

#include QMK_KEYBOARD_H

// Tap Dance declarations
enum {
    TD_Q_SHIFT,
    TD_W_SHIFT,
    TD_E_SHIFT,
    TD_R_SHIFT,
    TD_T_SHIFT,
    TD_Y_SHIFT,
    TD_U_SHIFT,
    TD_I_SHIFT,
    TD_O_SHIFT,
    TD_P_SHIFT,

    TD_A_SHIFT,
    TD_S_SHIFT,
    TD_D_SHIFT,
    TD_F_SHIFT,
    TD_G_SHIFT,
    TD_H_SHIFT,
    TD_J_SHIFT,
    TD_K_SHIFT,
    TD_L_SHIFT,

    TD_Z_SHIFT,
    TD_X_SHIFT,
    TD_C_SHIFT,
    TD_V_SHIFT,
    TD_B_SHIFT,
    TD_N_SHIFT,
    TD_M_SHIFT
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_Q_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_Q, S(KC_Q)),
    [TD_W_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_W, S(KC_W)), 
    [TD_E_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_E, S(KC_E)), 
    [TD_R_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_R, S(KC_R)), 
    [TD_T_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_T, S(KC_T)),
    [TD_Y_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_Y, S(KC_Y)), 
    [TD_U_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_U, S(KC_U)),
    [TD_I_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_I, S(KC_I)),
    [TD_O_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_O, S(KC_O)),
    [TD_P_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_P, S(KC_P)),


    [TD_A_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_A, S(KC_A)),
    [TD_S_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_S, S(KC_S)), 
    [TD_D_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_D, S(KC_D)), 
    [TD_F_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_F, S(KC_F)), 
    [TD_G_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_G, S(KC_G)),
    [TD_H_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_H, S(KC_H)), 
    [TD_J_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_J, S(KC_J)),
    [TD_K_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_K, S(KC_K)),
    [TD_L_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_L, S(KC_L)),


    [TD_Z_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_Z, S(KC_Z)),
    [TD_X_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_X, S(KC_X)), 
    [TD_C_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_C, S(KC_C)), 
    [TD_V_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_V, S(KC_V)), 
    [TD_B_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_B, S(KC_B)),
    [TD_N_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_N, S(KC_N)), 
    [TD_M_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_M, S(KC_M)),
};

enum layer_names { _DEFAULT = 0, _FUNCTIONS };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [_DEFAULT] = LAYOUT(
        KC_ESC,  KC_F1,             KC_F2,             KC_F3,             KC_F4,             KC_F5,             KC_F6,             KC_F7,             KC_F8,             KC_F9,             KC_F10,  KC_F11,   KC_F12,  KC_DEL,           KC_MUTE,
        KC_GRV,  KC_1,              KC_2,              KC_3,              KC_4,              KC_5,              KC_6,              KC_7,              KC_8,              KC_9,              KC_0,    KC_MINS,  KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  TD(TD_Q_SHIFT),    TD(TD_W_SHIFT),    TD(TD_E_SHIFT),    TD(TD_R_SHIFT),    TD(TD_T_SHIFT),    TD(TD_Y_SHIFT),    TD(TD_U_SHIFT),    TD(TD_I_SHIFT),    TD(TD_O_SHIFT),    TD(TD_P_SHIFT),    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        KC_CAPS, TD(TD_A_SHIFT),    TD(TD_S_SHIFT),    TD(TD_D_SHIFT),    TD(TD_F_SHIFT),    TD(TD_G_SHIFT),    TD(TD_H_SHIFT),    TD(TD_J_SHIFT),    TD(TD_K_SHIFT),    TD(TD_L_SHIFT),    KC_SCLN,           KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT, TD(TD_Z_SHIFT),    TD(TD_X_SHIFT),    TD(TD_C_SHIFT),    TD(TD_V_SHIFT),    TD(TD_B_SHIFT),    TD(TD_N_SHIFT),    TD(TD_M_SHIFT),    KC_COMM,           KC_DOT,            KC_SLSH,           KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI,           KC_LALT,                            KC_SPC,                                                                                          KC_RALT, MO(_FUNCTIONS),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_FUNCTIONS] = LAYOUT(
        _______, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,          _______,
        _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,            _______,
        _______, _______, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),


};
// clang-format on

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = false;
    debug_matrix = false;
    // debug_keyboard=true;
    // debug_mouse=true;
}

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return false;
}
#endif // ENCODER_ENABLE
