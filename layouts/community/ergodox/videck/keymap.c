#include <stdbool.h>

#include QMK_KEYBOARD_H
#include "action_layer.h"

enum {
  BASE = 0,     // Default layer
  ARROWS = 1,   // Arrows and Fx keys layer
  MOUSE = 2,    // Mouse movement and buttons layer

  TD_L = 0,     // Tap dance index for left shift
  TD_R = 1,     // Tap dance index for right shift
  TD_A = 2      // Tap dance index for arrow layer key
};

// See the videck.png image for a visualization of the layout.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_ergodox(
    // left hand
    KC_GRV,   KC_1,    KC_2,    KC_3,  KC_4,   KC_5,   KC_NO,
    KC_EQL,   KC_Q,    KC_W,    KC_E,  KC_R,   KC_T,   KC_NO,
    KC_BSPC,  KC_A,    KC_S,    KC_D,  KC_F,   KC_G,
    TD(TD_L), KC_Z,    KC_X,    KC_C,  KC_V,   KC_B,   KC_LCTL,
    KC_MUTE,  KC_VOLD, KC_VOLU, KC_NO, KC_ESC,
                                               KC_NO,  KC_NO,
                                                       TG(MOUSE),
                                       KC_SPC, KC_TAB, KC_LGUI,
    // right hand
    KC_NO,    KC_6,   KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
    KC_NO,    KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
              KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    TD(TD_A), KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, TD(TD_R),
                      KC_LALT, KC_NO,   KC_LBRC, KC_RBRC, KC_DELT,
    KC_NO,    KC_APP,
    KC_INS,
    KC_SLCK,  KC_ENT, KC_SPC
  ),
  [ARROWS] = LAYOUT_ergodox(
    // left hand
    KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_END,  KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_HOME, KC_TRNS, KC_TRNS, KC_PGDN, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                 KC_TRNS, KC_TRNS,
                                                          KC_NO,
                                        KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
    KC_TRNS, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_F12,
             KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [MOUSE] = LAYOUT_ergodox(
    // left hand
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                 KC_BTN3, KC_TRNS,
                                                          KC_TRNS,
                                        KC_BTN1, KC_BTN2, KC_TRNS,
    // right hand
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_TRNS, KC_TRNS,
    KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS
  )
};

typedef void (*videck_tap_dance_trigger_t) (const uint16_t kc);

typedef struct
{
  uint16_t kc1;
  uint16_t kc2;
  videck_tap_dance_trigger_t trigger;
} videck_tap_dance_tuple_t;

typedef struct
{
  uint8_t layer;
} videck_tap_dance_layer_t;

static void videck_tap_dance_finished(qk_tap_dance_state_t *state, void *user_data) {
  videck_tap_dance_tuple_t *const tuple = (videck_tap_dance_tuple_t*)user_data;

  if (state->count == 1) {
    register_code16(tuple->kc1);
  } else if (state->count == 2) {
    register_code16(tuple->kc2);
  }
}

static void videck_tap_dance_reset(qk_tap_dance_state_t *state, void *user_data) {
  videck_tap_dance_tuple_t *const tuple = (videck_tap_dance_tuple_t*)user_data;

  if (state->count == 1) {
    unregister_code16(tuple->kc1);
  } else if (state->count == 2) {
    unregister_code16(tuple->kc2);
    tuple->trigger(tuple->kc2);
  }
}

static void videck_tap_dance_layer_finished(qk_tap_dance_state_t *state, void *user_data) {
   const uint8_t layer = ((videck_tap_dance_layer_t*)user_data)->layer;

  if (state->count == 1 || state->count == 2) {
    layer_invert(layer);
  }
}

static void videck_tap_dance_layer_reset(qk_tap_dance_state_t *state, void *user_data) {
   const uint8_t layer = ((videck_tap_dance_layer_t*)user_data)->layer;

  if (state->count == 1) {
    layer_invert(layer);
  }
}

static bool caps_lock_is_on;

// Toggles caps lock status.
static void videck_caps_trigger (const uint16_t kc) {
  caps_lock_is_on ^= true;
}

#define ACTION_TAP_DANCE_DOUBLE_TRIGGER(kc1, kc2, double_trigger) { \
    .fn = { NULL, videck_tap_dance_finished, videck_tap_dance_reset }, \
    .user_data = (void *)&((videck_tap_dance_tuple_t) { kc1, kc2, double_trigger }),  \
  }

#define ACTION_TAP_DANCE_DOUBLE_LAYER(layer) { \
    .fn = { NULL, videck_tap_dance_layer_finished, videck_tap_dance_layer_reset }, \
    .user_data = (void *)&((videck_tap_dance_layer_t) { layer }),  \
  }

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_L] = ACTION_TAP_DANCE_DOUBLE_TRIGGER(KC_LSFT, KC_CAPS, videck_caps_trigger),
  [TD_R] = ACTION_TAP_DANCE_DOUBLE_TRIGGER(KC_RSFT, KC_CAPS, videck_caps_trigger),
  [TD_A] = ACTION_TAP_DANCE_DOUBLE_LAYER(ARROWS)
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  ergodox_led_all_set(LED_BRIGHTNESS_LO);
  caps_lock_is_on = false;
};

static void indicate_using_led(const uint8_t led, const bool enabled) {
  if (enabled) {
    ergodox_right_led_on(led);
  } else {
    ergodox_right_led_off(led);
  }
}

static inline void indicate_caps_lock_state(const bool enabled) {
  indicate_using_led(1, enabled);
}

static inline void indicate_arrows_layer_state(const bool enabled) {
  indicate_using_led(2, enabled);
}

static inline void indicate_mouse_layer_state(const bool enabled) {
  indicate_using_led(3, enabled);
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  // Check if we have shift locked.
  indicate_caps_lock_state(caps_lock_is_on);
  indicate_arrows_layer_state(IS_LAYER_ON(ARROWS));
  indicate_mouse_layer_state(IS_LAYER_ON(MOUSE));
};
