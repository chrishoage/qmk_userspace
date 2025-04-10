/* Copyright 2019 Danny Nguyen <danny@keeb.io>
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
#include "rgb_matrix/rgb_matrix.h"
#include QMK_KEYBOARD_H

enum chrishoage_keycodes {
    KM_TOG = QK_USER_0,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    [0] = LAYOUT(
        KC_MPRV, KC_MNXT, KC_MUTE,
        OSL(1) , KC_MSTP, KC_MPLY,
        KC_PSCR, KM_TOG , KC_DEL
    ),
    [1] = LAYOUT(
        QK_BOOT, RM_TOGG, KC_TRNS,
        TG(0)  , RM_VALU, RM_VALD,
        KC_F14 , KC_F13 , KC_F16
    ),
    // clang-format on
};

void keyboard_post_init_user() {
    rgblight_disable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_PINWHEEL);
    rgb_matrix_disable_noeeprom();
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch (get_highest_layer(layer_state)) {
        case 1:
            for (uint8_t i = led_min; i < led_max; i++) {
                if (g_led_config.flags[i] & LED_FLAG_KEYLIGHT) {
                    if (i == g_led_config.matrix_co[1][0]) {
                        rgb_matrix_set_color(i, RGB_PINK);
                    } else {
                        rgb_matrix_set_color(i, RGB_OFF);
                    }
                }
            }
            break;
    }
    return true;
}

bool          last_enabled = false;
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 1:
            last_enabled = rgb_matrix_is_enabled();
            rgb_matrix_enable_noeeprom();
            break;
        default:
            if (last_enabled) {
                rgb_matrix_enable_noeeprom();
            } else {
                rgb_matrix_disable_noeeprom();
            }
            break;
    }
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case 0:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
            break;
        case 1:
            if (clockwise) {
                rgb_matrix_step_noeeprom();
            } else {
                rgb_matrix_step_reverse_noeeprom();
            }
            break;
    }
    return false;
}

#define KVM_LEADER KC_SCROLL_LOCK
#define KVM_SEL_START KC_1
#define KVM_HEAD_COUNT 2

static uint8_t cur_kvm_head = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KM_TOG:
            if (record->event.pressed) {
                cur_kvm_head = (cur_kvm_head + 1) % KVM_HEAD_COUNT;

                clear_keyboard();
                layer_clear();

                tap_code(KVM_LEADER);
                wait_ms(10);
                tap_code(KVM_LEADER);
                wait_ms(10);
                tap_code(KVM_SEL_START + cur_kvm_head);
                wait_ms(500);
                return false;
            }
            break;
    }
    return true;
}
