// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include "quantum.h"
#include QMK_KEYBOARD_H

enum layer{
//    TENKEY = 0,
    HEX = 0,
    FUNC,
};

enum custom_keycodes {
  VI_SP = SAFE_RANGE,
  COL_Q,
  COL_QA,
  COL_N,
  COL_L_N,
  CTL_U,
  CTL_D,
  VI_WILD,
  KC_ESC_LNG2
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true; // 押されていない場合は処理を続行
    }

    switch (keycode) {
        case KC_ESC_LNG2:
            tap_code(KC_ESC);
            tap_code(KC_LNG2);
            return false; // ここで処理を終了
        case VI_SP:
            SEND_STRING("vi ");
            return false;
        case COL_Q:
            SEND_STRING(":q\n");
            return false;
        case COL_QA:
            SEND_STRING(":qa\n");
            return false;
        case COL_N:
            SEND_STRING(":n\n");
            return false;
        case COL_L_N:
            SEND_STRING(":N\n");
            return false;
        case CTL_U:
            SEND_STRING(SS_LCTL("u"));
            return false;
        case CTL_D:
            SEND_STRING(SS_LCTL("d"));
            return false;
        case VI_WILD:
            SEND_STRING("vi -p ./**/*.[ch]??\n");  // エンターキーを追加
            return false;
        default:
            return true; // カスタムキーコード以外の場合は処理を続行
    }

    return false; // カスタムキーコードの処理が完了したら、それ以上の処理を停止
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ 7 │ 8 │ 9 │ / │
     * ├───┼───┼───┼───┤
     * │ 4 │ 5 │ 6 │ * │
     * ├───┼───┼───┼───┤
     * │ 1 │ 2 │ 3 │ - │
     * ├───┼───┼───┼───┤
     * │ 0 │ . │Ent│ + │
     * └───┴───┴───┴───┘
     */
//    [TENKEY] = LAYOUT_ortho_4x4_with_2encoder(
//        KC_P7,   KC_P8,   KC_P9,   KC_PSLS,   KC_MUTE,
//        KC_P4,   KC_P5,   KC_P6,   KC_PAST,   KC_ESC,
//        KC_P1,   KC_P2,   KC_P3,   KC_PMNS,
//        LT(HEX, KC_P0),   LT(FUNC, KC_PDOT), LT(ADJ, KC_PENT), KC_PPLS
//    ),
    [HEX] = LAYOUT_ortho_4x4_with_2encoder(
        KC_NO,               KC_NO,      KC_NO,     KC_NO,      KC_NO,
        KC_ESC_LNG2,         COL_Q,      KC_PAST,   KC_BSPC,    KC_NO,
        KC_TAB,              VI_SP,      KC_UP,     KC_ENT,
        LT(FUNC, KC_P0),     KC_LEFT,    KC_DOWN,   KC_RIGHT
    ),
    [FUNC] = LAYOUT_ortho_4x4_with_2encoder(
        _______,   _______,  _______,   _______,      _______,
        _______,   COL_QA,   _______,   _______,   _______,
        _______,   VI_WILD,  CTL_U,     _______,
        KC_P0,     COL_L_N,    CTL_D,     COL_N
    )
};



#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
//    [TENKEY] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(RGB_MOD, RGB_RMOD)  },
//    [HEX]   =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(RGB_MOD, RGB_RMOD)  },
    [HEX]  =   {ENCODER_CCW_CW(RGB_VAD, RGB_VAI), ENCODER_CCW_CW(KC_DOWN, KC_UP)},
    [FUNC] =   {ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
};
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
//        case TENKEY:
//            oled_write_P(PSTR("TENKEY\n"), false);
//            break;
        case HEX:
            oled_write_P(PSTR("HEX\n"), false);
            break;
        case FUNC:
            oled_write_P(PSTR("FUNC\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    return false;
}
#endif
