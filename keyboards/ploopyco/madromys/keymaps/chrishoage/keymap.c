/* Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
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

enum chrishoage_ploopy_keycodes {
    LK_BTN5 = QK_USER_0,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_BTN4,    OSL(1), KC_BTN3,            KC_BTN5,
        KC_BTN1,                                KC_BTN2
    ),
    [1] = LAYOUT(
        DPI_CONFIG,    KC_NO, KC_NO,            LK_BTN5,
        QK_BOOT,                                QK_RBT
    )
};
// clang-format on

bool drag_scroll_lock = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LK_BTN5:
            if (record->event.pressed) {
                drag_scroll_lock = !drag_scroll_lock;
            }

            if (drag_scroll_lock) {
                register_code(KC_BTN5);
            } else {
                unregister_code(KC_BTN5);
            }

            return false;
    }
    return true;
}
