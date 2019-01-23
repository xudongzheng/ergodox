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
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {LAYOUT_ergodox(
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

// Define values for our custom keys. These values must be identical to those in
// generate.go for the comparisons to be valid. These may be triggered with
// arbitrary modifiers.
enum {
	XZ_PHYS_QWERTY = KC_KP_1,
	XZ_PHYS_MAC,
	XZ_PHYS_LAYER0,
	XZ_PHYS_LAYER1,
	XZ_PHYS_LAYER2,
	XZ_PHYS_LAYER3,
};

// Define modifiers and use a boolean array to store modifier states. The enum
// values define the index for each modifier. Store the corresponding keycode.
enum {
	XZ_MOD_LSHIFT,
	XZ_MOD_LCTRL,
	XZ_MOD_LGUI,
	XZ_MOD_LALT,
	XZ_MOD_RALT,
};
uint8_t xz_modifier_keys[] = {KC_LSHIFT, KC_LCTRL, KC_LGUI, KC_LALT, KC_RALT};
#define XZ_M_LEN(X) sizeof(X)/sizeof(X[0])
bool xz_modifiers[XZ_M_LEN(xz_modifier_keys)];

// Keep track of the current layer and whether QWERTY/MacOS mode is on.
uint8_t  xz_layer;
bool xz_flag_qwerty, xz_flag_mac;

void xz_toggle_modifier(int i) {
	bool result = xz_modifiers[i];
	uint16_t keycode = xz_modifier_keys[i];
	if (result) {
		unregister_code(keycode);
	} else {
		register_code(keycode);
	}
	xz_modifiers[i] = !result;
}

uint16_t xz_get_byte(uint16_t ptr) {
	return pgm_read_byte_near(keymapData + ptr);
}

uint16_t xz_get_word(uint16_t ptr) {
	return 256*xz_get_byte(ptr) + xz_get_byte(ptr+1);
}

void xz_record_internal(uint16_t keyPtr) {
	uint8_t stdLen = xz_get_byte(keyPtr);
	uint16_t macPtr = keyPtr + 1 + 2*stdLen;
	uint8_t macLen = xz_get_byte(macPtr);
	uint16_t colemakPtr = macPtr + 1 + 2*macLen;
	uint8_t colemak = xz_get_byte(colemakPtr);
	uint8_t qwerty = xz_get_byte(colemakPtr+1);

	// If the key is a compound key, handle keys recursively.
	if (stdLen + macLen > 0) {
		uint8_t len;
		uint16_t ptr;
		if (xz_flag_mac && macLen > 0) {
			len = macLen;
			ptr = macPtr + 1;
		} else {
			len = stdLen;
			ptr = keyPtr + 1;
		}
		for (int i = 0; i < len; i++) {
			xz_record_internal(xz_get_word(ptr + 2*i));
		}
		return;
	}

	// Process the underlying key. we only have to consider the Colemak key
	// since the QWERTY key is not defined for any of the following cases. For
	// all of the matched cases, return since no further actions needed.
	switch (colemak) {
	case KC_LSHIFT:
		xz_toggle_modifier(XZ_MOD_LSHIFT);
		return;
	case KC_LCTRL:
		xz_toggle_modifier(XZ_MOD_LCTRL);
		return;
	case KC_LGUI:
		xz_toggle_modifier(XZ_MOD_LGUI);
		return;
	case KC_LALT:
		xz_toggle_modifier(XZ_MOD_LALT);
		return;
	case KC_RALT:
		xz_toggle_modifier(XZ_MOD_RALT);
		return;
	case XZ_PHYS_QWERTY:
		xz_flag_qwerty = !xz_flag_qwerty;
		return;
	case XZ_PHYS_MAC:
		xz_flag_mac = !xz_flag_mac;
		return;
	}

	// Switch layer if requested. Return since no further actions needed.
	if (colemak >= XZ_PHYS_LAYER0 && colemak <= XZ_PHYS_LAYER3) {
		xz_layer = colemak - XZ_PHYS_LAYER0;
		return;
	}

	// Handle regular key. Send QWERTY key if QWERTY mode is enabled.
	uint16_t keycode;
	if (xz_flag_qwerty && qwerty != KC_NO) {
		keycode = qwerty;
	} else {
		keycode = colemak;
	}
	register_code(keycode);
	unregister_code(keycode);

	// Unset all modifiers.
	for (int i = 0; i < XZ_M_LEN(xz_modifiers); i++) {
		if (xz_modifiers[i]) {
			xz_toggle_modifier(i);
		}
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// We don't care when a key is released, only when it is pressed.
	if (!record->event.pressed) {
		return false;
	}

	// Map the key to a virtual key. Variable "keycode" comes from the keyboard
	// matrix and is unrelated to the keycode that is sent to the client.
	uint16_t layerPtr = 2 * (76*xz_layer + keycode);
	uint16_t keyPtr = xz_get_word(layerPtr);

	// Reset layer to the main layer if not on layer 3.
	if (xz_layer != 3) {
		xz_layer = 0;
	}

	// Handle press.
	xz_record_internal(keyPtr);
	return false;
}
