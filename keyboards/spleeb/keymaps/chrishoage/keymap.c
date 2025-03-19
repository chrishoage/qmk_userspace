// Copyright 2022 Chris Hoage (@chrishoage)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "action.h"
#include "modifiers.h"
#include QMK_KEYBOARD_H

enum chrishoage_keycodes {
    KM_TOG = QK_USER_0,
    KM_LCTL,
    KM_LGUI,
    KM_RCTL,
    KM_RGUI,
};

// Double tap TD(0) to enter bootloader
static void enter_qk_boot(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2) {
        reset_keyboard();
        reset_tap_dance(state);
    }
}

enum SpleebLayer { _BASE = 0, _FN, _MOUSE, _TEMPLATE };

tap_dance_action_t tap_dance_actions[] = {[0] = ACTION_TAP_DANCE_FN(enter_qk_boot)};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[_BASE] = LAYOUT(
         KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,  KC_ESC,                                          KC_BSPC,    KC_6,    KC_7,    KC_8,   KC_9,    KC_0, KC_BSLS,
         KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, KC_MINS,                                           KC_EQL,    KC_Y,    KC_U,    KC_I,   KC_O,    KC_P, KC_QUOT,
        KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G, KC_LBRC,                                          KC_RBRC,    KC_H,    KC_J,    KC_K,   KC_L, KC_SCLN, KC_RSFT,
                    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,          KC_MUTE,                        ENC_STR,             KC_N,    KC_M, KC_COMM, KC_DOT, KC_SLSH,
                                            KM_LCTL, KC_LALT, KM_LGUI,  KC_ENT,   MO(_FN),        MO(_FN),  KC_SPC, KM_RGUI, KC_RALT, KM_RCTL
    ),

	[_FN] = LAYOUT(
        KC_PAUS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,  KC_ESC,                                           KC_DEL,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_PSCR,
        KM_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                          KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, KC_TRNS, KC_TRNS,
        KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                          KC_TRNS, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_TRNS,  QK_RBT,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          ENC_STL,                        KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   TD(0),
                                            KC_TRNS, KC_TRNS, KC_TRNS,  KC_SPC, KC_TRNS,      KC_TRNS,  KC_ENT, KC_TRNS, KC_TRNS, KC_TRNS
    ),

	[_MOUSE] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        DRGSCRL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, SNIPING,                                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN3,          KC_TRNS,                         KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

	[_TEMPLATE] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                         KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
};
// clang-format on

bool           km_ctl       = false;
bool           km_gui       = false;
static uint8_t cur_kvm_head = 0;

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t mods = get_mods();
    if (km_gui) {
        // When GUI is held trigger [ ] to move workspaces
        tap_code(clockwise ? KC_RBRC : KC_LBRC);
        return false;
    }

    if (km_ctl) {
        clear_mods();
        if (mods & MOD_MASK_SHIFT) {
            register_mods(MOD_MASK_SHIFT);
        }
        // When CTRL is hled trigger page up/down to move tabs (Firefox, VSCode)
        tap_code16(LCTL(clockwise ? KC_PGDN : KC_PGUP));
        set_mods(mods);
        return false;
    }
    if (mods & MOD_MASK_ALT) {
        // When ALT is held trigger up/down to move line up/down
        tap_code(clockwise ? KC_DOWN : KC_UP);
        return false;
    }
    // Defer to encoder_update_kb to trigger spleeb_encoder_mode_trigger
    return true;
}

enum spleeb_enc_mode {
    DEF_DPI,
    SNP_DPI,
    VOL,
    SEL,
};

void spleeb_encoder_mode_trigger(uint8_t mode, bool clockwise) {
    dprintf("spleeb_encoder_mode_trigger m: %u, c: %u\n", mode, clockwise);
    switch (mode) {
        case DEF_DPI:
            spleeb_cycle_pointer_default_dpi(clockwise);
            break;
        case SNP_DPI:
            spleeb_cycle_pointer_sniping_dpi(clockwise);
            break;
        case VOL:
            tap_code(clockwise ? KC_VOLU : KC_VOLD);
            break;
        case SEL: {
            bool     is_shift = get_mods() & MOD_MASK_SHIFT;
            uint16_t dir      = clockwise ? KC_RIGHT : KC_LEFT;
            if (is_shift) {
                tap_code(dir);
            } else {
                tap_code16(LSFT(LCTL(dir)));
            }
            break;
        }

        default:
            break;
    }
}

const char *spleeb_encoder_mode_string(uint8_t mode) {
    switch (mode) {
        case DEF_DPI:
            return "df dpi";
        case SNP_DPI:
            return "sn dpi";
        case VOL:
            return "volume";
        case SEL:
            return "select";
    }

    return get_u8_str(mode, ' ');
}

void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOUSE);
}

bool saved_nkro_state;

uint32_t restore_nkro_state(uint32_t trigger_time, void *cb_arg) {
    clear_keyboard();
    keymap_config.nkro = saved_nkro_state;

    return 0;
}

#define KVM_LEADER KC_SCROLL_LOCK
#define KVM_SEL_START KC_1
#define KVM_HEAD_COUNT 2

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KM_TOG:
            if (record->event.pressed) {
                saved_nkro_state = keymap_config.nkro;
                cur_kvm_head     = (cur_kvm_head + 1) % KVM_HEAD_COUNT;

                clear_keyboard();
                layer_clear();
                keymap_config.nkro = false;

                tap_code(KVM_LEADER);
                wait_ms(10);
                tap_code(KVM_LEADER);
                wait_ms(10);
                tap_code(KVM_SEL_START + cur_kvm_head);
                wait_ms(500);
                defer_exec(500, restore_nkro_state, NULL);
                return false;
            }
            break;
        case KM_LCTL: {
            uint8_t mod = cur_kvm_head == 0 ? MOD_BIT(KC_LCTL) : MOD_BIT(KC_LGUI);
            km_ctl      = record->event.pressed;
            if (record->event.pressed) {
                register_mods(mod);
            } else {
                unregister_mods(mod);
            }
            return false;
            break;
        }
        case KM_LGUI: {
            uint8_t mod = cur_kvm_head == 0 ? MOD_BIT(KC_LGUI) : MOD_BIT(KC_LCTL);
            km_gui      = record->event.pressed;
            if (record->event.pressed) {
                register_mods(mod);
            } else {
                unregister_mods(mod);
            }
            return false;
            break;
        }
        case KM_RCTL: {
            uint8_t mod = cur_kvm_head == 0 ? MOD_BIT(KC_RCTL) : MOD_BIT(KC_RGUI);
            km_ctl      = record->event.pressed;
            if (record->event.pressed) {
                register_mods(mod);
            } else {
                unregister_mods(mod);
            }

            return false;
            break;
        }
        case KM_RGUI: {
            uint8_t mod = cur_kvm_head == 0 ? MOD_BIT(KC_RGUI) : MOD_BIT(KC_RCTL);
            km_gui      = record->event.pressed;
            if (record->event.pressed) {
                register_mods(mod);
            } else {
                unregister_mods(mod);
            }
            return false;
            break;
        }
    }
    return true;
}

const spleeb_enc_mode_t spleeb_encoder_mode_map[NUM_ENCODERS][SPLEEB_ENCODER_MODE_COUNT] = {
    [0] = {SPLEEB_ENC_MODE(VOL), SPLEEB_ENC_MODE(SEL)},
    [1] = {SPLEEB_ENC_MODE(DEF_DPI), SPLEEB_ENC_MODE(SNP_DPI), SPLEEB_ENC_MODE(SEL)},
};
