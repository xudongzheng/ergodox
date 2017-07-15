#include "ergodox.h"

// Define our own keyboard matrix. It doesn't matter if we overwrite the
// standard enum values since everything is handled in process_record_user().
enum {
	XZ_L11, XZ_L12, XZ_L13, XZ_L14, XZ_L15, XZ_L16, XZ_L17,
	XZ_L21, XZ_L22, XZ_L23, XZ_L24, XZ_L25, XZ_L26, XZ_L27,
	XZ_L31, XZ_L32, XZ_L33, XZ_L34, XZ_L35, XZ_L36,
	XZ_L41, XZ_L42, XZ_L43, XZ_L44, XZ_L45, XZ_L46, XZ_L47,
	XZ_L51, XZ_L52, XZ_L53, XZ_L54, XZ_L55,
	XZ_L61, XZ_L62, XZ_L63, XZ_L64, XZ_L65, XZ_L66,
	XZ_R11, XZ_R12, XZ_R13, XZ_R14, XZ_R15, XZ_R16, XZ_R17,
	XZ_R21, XZ_R22, XZ_R23, XZ_R24, XZ_R25, XZ_R26, XZ_R27,
	XZ_R31, XZ_R32, XZ_R33, XZ_R34, XZ_R35, XZ_R36,
	XZ_R41, XZ_R42, XZ_R43, XZ_R44, XZ_R45, XZ_R46, XZ_R47,
	XZ_R51, XZ_R52, XZ_R53, XZ_R54, XZ_R55,
	XZ_R61, XZ_R62, XZ_R63, XZ_R64, XZ_R65, XZ_R66,
};

// Set custom keymap to be used by the QMK firmware.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {KEYMAP(
	XZ_L11, XZ_L12, XZ_L13, XZ_L14, XZ_L15, XZ_L16, XZ_L17,
	XZ_L21, XZ_L22, XZ_L23, XZ_L24, XZ_L25, XZ_L26, XZ_L27,
	XZ_L31, XZ_L32, XZ_L33, XZ_L34, XZ_L35, XZ_L36,
	XZ_L41, XZ_L42, XZ_L43, XZ_L44, XZ_L45, XZ_L46, XZ_L47,
	XZ_L51, XZ_L52, XZ_L53, XZ_L54, XZ_L55,
	XZ_L61, XZ_L62, XZ_L63, XZ_L64, XZ_L65, XZ_L66,
	XZ_R11, XZ_R12, XZ_R13, XZ_R14, XZ_R15, XZ_R16, XZ_R17,
	XZ_R21, XZ_R22, XZ_R23, XZ_R24, XZ_R25, XZ_R26, XZ_R27,
	XZ_R31, XZ_R32, XZ_R33, XZ_R34, XZ_R35, XZ_R36,
	XZ_R41, XZ_R42, XZ_R43, XZ_R44, XZ_R45, XZ_R46, XZ_R47,
	XZ_R51, XZ_R52, XZ_R53, XZ_R54, XZ_R55,
	XZ_R61, XZ_R62, XZ_R63, XZ_R64, XZ_R65, XZ_R66
)};

// Define custom keys and characters. TODO alternatively we can set XZ_PASS to
// KC_TRANSPARENT and start XZ_LAY0 at SAFE_RANGE.
enum {
	XZ_PASS = SAFE_RANGE,
	XZ_CLMK,
	XZ_LAY0,
	XZ_LAY1,
	XZ_LAY2,
	XZ_LAY3,
};

// Define the keycode for the base layer. TODO KC_MS_BTN3 does not work.
uint16_t xz_layer_base[] = {
	KC_MS_BTN3, KC_1, KC_2, KC_3, KC_4, KC_5, KC_PSCREEN,
	KC_TAB, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_CAPSLOCK,
	KC_ESCAPE, KC_A, KC_R, KC_S, KC_T, KC_D,
	XZ_LAY2, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_AUDIO_VOL_DOWN,
	XZ_CLMK, KC_NO, KC_LALT, KC_LEFT, KC_DOWN,
	KC_DELETE, KC_HOME, KC_PGUP, KC_SPACE, KC_LCTL, KC_LGUI,
	KC_NO, KC_6, KC_7, KC_8, KC_9, KC_0, KC_APPLICATION,
	KC_AUDIO_MUTE, KC_J, KC_L, KC_U, KC_Y, KC_SCOLON, KC_BSLASH,
	KC_H, KC_N, KC_E, KC_I, KC_O, KC_BSPACE,
	KC_AUDIO_VOL_UP, KC_K, KC_M, KC_COMMA, KC_DOT, KC_SLASH, XZ_LAY2,
	KC_UP, KC_RIGHT, KC_LALT, KC_NO, XZ_LAY3,
	KC_END, KC_INSERT, KC_PGDOWN, XZ_LAY1, KC_LSHIFT, KC_ENTER,
};

// If the user has Colemak in software, we need to send QWERTY keys so input
// isn't converted to Colemak twice. On Windows computers, we have escaped
// mapped to caps lock and vice versa so we need to account for that.
uint16_t xz_layer_colemak[] = {
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, XZ_PASS, XZ_PASS, KC_E, KC_R, KC_T, KC_ESCAPE,
	KC_CAPSLOCK, XZ_PASS, KC_S, KC_D, KC_F, KC_G,
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, KC_Y, KC_U, KC_I, KC_O, KC_P, XZ_PASS,
	XZ_PASS, KC_J, KC_K, KC_L, KC_SCOLON, XZ_PASS,
	XZ_PASS, KC_N, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
	XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS, XZ_PASS,
};

// Define layer 1 for the function keys.
uint16_t xz_layer_1[] = {
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_NO, KC_NO,
	KC_NO, KC_F5, KC_F6, KC_F7, KC_F8, KC_NO,
	KC_NO, KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_F13, KC_F14, KC_F15, KC_F16, KC_NO,
	KC_NO, KC_F17, KC_F18, KC_F19, KC_F20, KC_NO,
	KC_NO, KC_NO, KC_F21, KC_F22, KC_F23, KC_F24, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
};

// Define layer 2 for additional punctuation. TODO some of these will fail to
// work if software Colemak is enabled. Additionally, these characters will
// likely not work outside of the main desktop.
uint16_t xz_layer_2[] = {
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, RALT(LSFT(KC_1)), KC_TILD, KC_GRAVE, KC_HASH, KC_NO,
	KC_NO, RALT(KC_A), KC_EXLM, KC_PLUS, KC_EQUAL, KC_DLR,
	KC_NO, RALT(KC_SLASH), KC_AT, KC_LBRACKET, KC_RBRACKET, KC_PERC, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_QUOTE, KC_DQUO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_CIRC, RALT(KC_Y), RALT(KC_U), KC_NO, KC_NO, KC_NO,
	KC_AMPR, RALT(KC_N), RALT(KC_E), RALT(KC_I), RALT(KC_O), KC_NO,
	KC_NO, KC_ASTR, KC_MINUS, KC_UNDS, KC_LCBR, KC_RCBR, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_LPRN, KC_RPRN,
};

// Define layer 3 for a right-handed number pad.
uint16_t xz_layer_3[] = {
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO, KC_PLUS, KC_7, KC_8, KC_9, KC_MINUS, KC_NO,
	KC_ASTR, KC_4, KC_5, KC_6, KC_SLASH, KC_NO,
	KC_NO, KC_0, KC_1, KC_2, KC_3, KC_DOT, KC_NO,
	KC_NO, KC_NO, KC_NO, KC_NO, XZ_LAY0,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
};

// Maintain the state of the keyboard.
uint16_t xz_layer;
bool xz_colemak;
bool xz_modifier_ctrl, xz_modifier_shift, xz_modifier_alt, xz_modifier_gui;

void xz_resolve_modifier(bool *active, uint16_t keycode) {
	if (*active) {
		unregister_code(keycode);
	} else {
		register_code(keycode);
	}
	*active = !*active;
}

void xz_resolve_keycode(uint16_t keycode) {
	if (keycode == XZ_CLMK) {
		xz_colemak = !xz_colemak;
	} else if (keycode == XZ_LAY0) {
		xz_layer = 0;
	} else if (keycode == XZ_LAY1) {
		xz_layer = 1;
	} else if (keycode == XZ_LAY2) {
		xz_layer = 2;
	} else if (keycode == XZ_LAY3) {
		xz_layer = 3;
	} else if (keycode == KC_LCTL) {
		xz_resolve_modifier(&xz_modifier_ctrl, KC_LCTRL);
	} else if (keycode == KC_LSHIFT) {
		xz_resolve_modifier(&xz_modifier_shift, KC_LSHIFT);
	} else if (keycode == KC_LALT) {
		xz_resolve_modifier(&xz_modifier_alt, KC_LALT);
	} else if (keycode == KC_LGUI) {
		xz_resolve_modifier(&xz_modifier_gui, KC_LGUI);
	} else {
		if (keycode & QK_RALT) {
			register_code(KC_RALT);
		}
		if (keycode & QK_LSFT) {
			xz_resolve_modifier(&xz_modifier_shift, KC_LSHIFT);
		}
		register_code(keycode);
		unregister_code(keycode);
		if (xz_modifier_ctrl) {
			xz_resolve_modifier(&xz_modifier_ctrl, KC_LCTRL);
		}
		if (xz_modifier_shift) {
			xz_resolve_modifier(&xz_modifier_shift, KC_LSHIFT);
		}
		if (xz_modifier_alt) {
			xz_resolve_modifier(&xz_modifier_alt, KC_LALT);
		}
		if (xz_modifier_gui) {
			xz_resolve_modifier(&xz_modifier_gui, KC_LGUI);
		}
		if (keycode & QK_RALT) {
			unregister_code(KC_RALT);
		}
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// We don't care when a key is released, only when it is pressed. TODO weird
	// things can potentially happen if we hold a key on the keyboard.
	if (!record->event.pressed) {
		return false;
	}

	// Resolve the key based on the active layer.
	if (xz_layer == 1) {
		xz_resolve_keycode(xz_layer_1[keycode]);
		xz_layer = 0;
	} else if (xz_layer == 2) {
		xz_resolve_keycode(xz_layer_2[keycode]);
		xz_layer = 0;
	} else if (xz_layer == 3) {
		xz_resolve_keycode(xz_layer_3[keycode]);
	} else if (xz_colemak && xz_layer_colemak[keycode] != XZ_PASS) {
		xz_resolve_keycode(xz_layer_colemak[keycode]);
	} else {
		xz_resolve_keycode(xz_layer_base[keycode]);
	}
	return false;
}
