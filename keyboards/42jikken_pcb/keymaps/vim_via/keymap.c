// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

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
  CTL_U,
  CTL_D,
  CTL_CURLY,
  VI_WILD
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case VI_SP:
      if (record->event.pressed) {SEND_STRING("vi ");}
    case COL_Q:
      if (record->event.pressed) {SEND_STRING(":q\n");}
    case COL_QA:
      if (record->event.pressed) {SEND_STRING(":qa\n");}
    case CTL_U:
      if (record->event.pressed) {SEND_STRING(SS_LCTL("u"));}
    case CTL_D:
      if (record->event.pressed) {SEND_STRING(SS_LCTL("d"));}
    case CTL_CURLY:
      if (record->event.pressed) {SEND_STRING(SS_LCTL("}"));}
    case VI_WILD:
      if (record->event.pressed) {SEND_STRING("vi -p ./**/*.*pp");}
      return false;
      break;
  }
  return true;
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
        KC_P4,               KC_P5,      KC_P6,     KC_NO,      KC_NO,
        KC_ESC,              COL_Q,      KC_NO,     KC_DEL,      KC_NO,
        KC_TAB,              VI_SP,      KC_UP,     KC_ENT,
        LT(FUNC, KC_P0),  KC_LEFT,    KC_DOWN,   KC_RIGHT
    ),
    [FUNC] = LAYOUT_ortho_4x4_with_2encoder(
        KC_F7,   KC_F8,   KC_F9,   KC_F10,      KC_NO,
        KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_NO,
        KC_F1,   VI_WILD,   KC_F3,   KC_F12,
        KC_P0,   KC_PDOT, KC_PENT, KC_PPLS
    )
};



#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
//    [TENKEY] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(RGB_MOD, RGB_RMOD)  },
    [HEX]  =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(RGB_VAD, RGB_VAI)  },
    [FUNC]  =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },
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
