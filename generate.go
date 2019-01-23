//go:generate go run generate.go

package main

// #include "keycode.h"
import "C"

import (
	"bytes"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

// The keyboard does not have a number pad so we can repurpose those keys for
// our own purpose. Use them for keys that modify the keyboard state.
const (
	XZ_PHYS_QWERTY = C.KC_KP_1 + iota
	XZ_PHYS_MAC
	XZ_PHYS_LAYER0
	XZ_PHYS_LAYER1
	XZ_PHYS_LAYER2
	XZ_PHYS_LAYER3
)

type virtualKey struct {
	name string

	stdKeys, macKeys []string
	colemak, qwerty  uint8
}

type layer [76]string

func writeKeys(buf *bytes.Buffer, reverse map[string]uint16, keys []string) {
	binary.Write(buf, binary.BigEndian, uint8(len(keys)))
	for _, value := range keys {
		i, ok := reverse[value]
		if !ok {
			log.Fatal("unrecognized " + value + " key referenced")
		}
		binary.Write(buf, binary.BigEndian, i)
	}
}

func macroC(name string, colemak uint8) virtualKey {
	return virtualKey{name: name, colemak: colemak}
}

func macroCQ(name string, colemak, qwerty uint8) virtualKey {
	return virtualKey{name: name, colemak: colemak, qwerty: qwerty}
}

func macroShift(shifted, name string) virtualKey {
	return virtualKey{name: shifted, stdKeys: []string{"lshift", name}}
}

func macroTilde(letter string) virtualKey {
	return virtualKey{
		name:    letter + "_tilde",
		stdKeys: []string{"ralt", letter},
		macKeys: []string{"lalt", "e", letter},
	}
}

func macroUnicode(name, codepoint string) virtualKey {
	stdKeys := []string{"lctrl", "lshift", "u"}
	stdKeys = append(stdKeys, strings.Split(codepoint, "")...)
	stdKeys = append(stdKeys, "space")
	return virtualKey{name: name, stdKeys: stdKeys}
}

func main() {
	keys := []virtualKey{
		// Define placeholder for unused keys.
		{name: ""},

		// Define custom keys.
		macroC("qwerty", XZ_PHYS_QWERTY),
		macroC("mac", XZ_PHYS_MAC),
		macroC("layer0", XZ_PHYS_LAYER0),
		macroC("layer1", XZ_PHYS_LAYER1),
		macroC("layer2", XZ_PHYS_LAYER2),
		macroC("layer3", XZ_PHYS_LAYER3),

		// Define modifiers. The keycode values for these are only used for
		// comparison and are not sent to the client. The values in
		// xz_modifier_keys, which are the same, are sent instead.
		macroC("lshift", C.KC_LSHIFT),
		macroC("lctrl", C.KC_LCTRL),
		macroC("lgui", C.KC_LGUI),
		macroC("lalt", C.KC_LALT),
		macroC("ralt", C.KC_RALT),

		// Define the virtual keys for main three rows of the main layer.
		macroC("q", C.KC_Q),
		macroC("w", C.KC_W),
		macroCQ("f", C.KC_F, C.KC_E),
		macroCQ("p", C.KC_P, C.KC_R),
		macroCQ("g", C.KC_G, C.KC_T),
		macroC("a", C.KC_A),
		macroCQ("r", C.KC_R, C.KC_S),
		macroCQ("s", C.KC_S, C.KC_D),
		macroCQ("t", C.KC_T, C.KC_F),
		macroCQ("d", C.KC_D, C.KC_G),
		macroC("z", C.KC_Z),
		macroC("x", C.KC_X),
		macroC("c", C.KC_C),
		macroC("v", C.KC_V),
		macroC("b", C.KC_B),
		macroCQ("j", C.KC_J, C.KC_Y),
		macroCQ("l", C.KC_L, C.KC_U),
		macroCQ("u", C.KC_U, C.KC_I),
		macroCQ("y", C.KC_Y, C.KC_O),
		macroCQ("scolon", C.KC_SCOLON, C.KC_P),
		macroC("h", C.KC_H),
		macroCQ("n", C.KC_N, C.KC_J),
		macroCQ("e", C.KC_E, C.KC_K),
		macroCQ("i", C.KC_I, C.KC_L),
		macroCQ("o", C.KC_O, C.KC_SCOLON),
		macroCQ("k", C.KC_K, C.KC_N),
		macroC("m", C.KC_M),
		macroC("comma", C.KC_COMMA),
		macroC("dot", C.KC_DOT),
		macroC("slash", C.KC_SLASH),

		// Define additional keys for the main layer in the upper five rows.
		macroC("pscreen", C.KC_PSCREEN),
		macroC("application", C.KC_APPLICATION),
		macroC("tab", C.KC_TAB),
		macroC("capslock", C.KC_CAPSLOCK),
		macroC("audio_mute", C.KC_AUDIO_MUTE),
		macroC("bslash", C.KC_BSLASH),
		macroC("escape", C.KC_ESCAPE),
		macroC("bspace", C.KC_BSPACE),
		macroC("audio_vol_down", C.KC_AUDIO_VOL_DOWN),
		macroC("audio_vol_up", C.KC_AUDIO_VOL_UP),
		macroC("left", C.KC_LEFT),
		macroC("down", C.KC_DOWN),
		macroC("up", C.KC_UP),
		macroC("right", C.KC_RIGHT),

		// Define additional keys for the left and the right thumb clusters.
		macroC("delete", C.KC_DELETE),
		macroC("home", C.KC_HOME),
		macroC("pgup", C.KC_PGUP),
		macroC("space", C.KC_SPACE),
		macroC("end", C.KC_END),
		macroC("insert", C.KC_INSERT),
		macroC("pgdown", C.KC_PGDOWN),
		macroC("enter", C.KC_ENTER),

		// Define numbers and their shifted values.
		macroC("1", C.KC_1), macroShift("exlm", "1"),
		macroC("2", C.KC_2), macroShift("at", "2"),
		macroC("3", C.KC_3), macroShift("hash", "3"),
		macroC("4", C.KC_4), macroShift("dlr", "4"),
		macroC("5", C.KC_5), macroShift("perc", "5"),
		macroC("6", C.KC_6), macroShift("circ", "6"),
		macroC("7", C.KC_7), macroShift("ampr", "7"),
		macroC("8", C.KC_8), macroShift("astr", "8"),
		macroC("9", C.KC_9), macroShift("lprn", "9"),
		macroC("0", C.KC_0), macroShift("rprn", "0"),

		// Define remaining punctuation keys.
		macroC("grave", C.KC_GRAVE), macroShift("tild", "grave"),
		macroC("minus", C.KC_MINUS), macroShift("unds", "minus"),
		macroC("equal", C.KC_EQUAL), macroShift("plus", "equal"),
		macroC("lbracket", C.KC_LBRACKET), macroShift("lcbr", "lbracket"),
		macroC("rbracket", C.KC_RBRACKET), macroShift("rcbr", "rbracket"),
		macroC("quote", C.KC_QUOTE), macroShift("dquo", "quote"),
		macroShift("colon", "scolon"),

		// Define function keys.
		macroC("f1", C.KC_F1), macroC("f2", C.KC_F2),
		macroC("f3", C.KC_F3), macroC("f4", C.KC_F4),
		macroC("f5", C.KC_F5), macroC("f6", C.KC_F6),
		macroC("f7", C.KC_F7), macroC("f8", C.KC_F8),
		macroC("f9", C.KC_F9), macroC("f10", C.KC_F10),
		macroC("f11", C.KC_F11), macroC("f12", C.KC_F12),
		macroC("f13", C.KC_F13), macroC("f14", C.KC_F14),
		macroC("f15", C.KC_F15), macroC("f16", C.KC_F16),
		macroC("f17", C.KC_F17), macroC("f18", C.KC_F18),
		macroC("f19", C.KC_F19), macroC("f20", C.KC_F20),
		macroC("f21", C.KC_F21), macroC("f22", C.KC_F22),
		macroC("f23", C.KC_F23), macroC("f24", C.KC_F24),

		// Define Spanish accented vowels.
		macroTilde("a"), macroTilde("e"), macroTilde("i"),
		macroTilde("o"), macroTilde("u"),

		// Define additional Spanish characters.
		{
			name:    "n_tilde",
			stdKeys: []string{"ralt", "n"},
			macKeys: []string{"lalt", "n", "n"},
		},
		{
			name:    "u_dier",
			stdKeys: []string{"ralt", "y"},
			macKeys: []string{"lalt", "u", "u"},
		},
		{
			name:    "exlm_inv",
			stdKeys: []string{"ralt", "lshift", "1"},
			macKeys: []string{"lalt", "1"},
		},
		{
			name:    "ques_inv",
			stdKeys: []string{"ralt", "slash"},
			macKeys: []string{"lalt", "lshift", "slash"},
		},

		// Define custom sequences.
		{
			name: "firefox_menu",
			stdKeys: []string{
				"lctrl", "l", "lshift", "tab", "lshift", "tab", "application",
			},
		},
		macroUnicode("emoji_grin", "1f604"),
		macroUnicode("emoji_squint", "1f606"),
		macroUnicode("emoji_joy", "1f602"),
		macroUnicode("emoji_sweat", "1f605"),
		macroUnicode("emoji_smile", "1f600"),
		macroUnicode("emoji_thumb", "1f44d"),
		macroUnicode("emoji_thinking", "1f914"),
		macroUnicode("emoji_wink", "1f609"),
		macroUnicode("emoji_pouting", "1f621"),
		macroUnicode("emoji_ok", "1f44c"),
	}

	layers := []layer{
		{
			"", "1", "2", "3", "4", "5", "pscreen",
			"tab", "q", "w", "f", "p", "g", "capslock",
			"escape", "a", "r", "s", "t", "d",
			"layer2", "z", "x", "c", "v", "b", "audio_vol_down",
			"qwerty", "mac", "lalt", "left", "down",
			"delete", "home", "pgup", "space", "lctrl", "lgui",
			"firefox_menu", "6", "7", "8", "9", "0", "application",
			"audio_mute", "j", "l", "u", "y", "scolon", "bslash",
			"h", "n", "e", "i", "o", "bspace",
			"audio_vol_up", "k", "m", "comma", "dot", "slash", "layer2",
			"up", "right", "lalt", "", "layer3",
			"end", "insert", "pgdown", "layer1", "lshift", "enter",
		},
		{
			"", "", "", "", "", "", "",
			"", "f1", "f2", "f3", "f4", "", "",
			"", "f5", "f6", "f7", "f8", "",
			"", "f9", "f10", "f11", "f12", "", "",
			"", "", "", "", "",
			"", "", "", "", "", "",
			"", "", "", "", "", "", "",
			"", "", "f13", "f14", "f15", "f16", "",
			"", "f17", "f18", "f19", "f20", "",
			"", "", "f21", "f22", "f23", "f24", "",
			"", "", "", "", "",
			"", "", "", "", "", "",
		},
		{
			"", "emoji_grin", "emoji_squint", "emoji_joy", "emoji_sweat", "emoji_smile", "",
			"", "", "exlm_inv", "tild", "grave", "hash", "",
			"", "a_tilde", "exlm", "plus", "equal", "dlr",
			"", "ques_inv", "at", "lbracket", "rbracket", "perc", "",
			"", "", "", "", "",
			"", "", "", "quote", "dquo", "",
			"", "emoji_thumb", "emoji_thinking", "emoji_wink", "emoji_pouting", "emoji_ok", "",
			"", "circ", "u_dier", "u_tilde", "", "", "",
			"ampr", "n_tilde", "e_tilde", "i_tilde", "o_tilde", "",
			"", "astr", "minus", "unds", "lcbr", "rcbr", "",
			"", "", "", "", "",
			"", "", "", "", "lprn", "rprn",
		},
		{
			"", "", "", "", "", "", "",
			"tab", "", "", "", "", "", "",
			"", "slash", "astr", "minus", "plus", "",
			"", "", "", "equal", "dot", "", "",
			"", "", "", "", "",
			"", "", "", "space", "lctrl", "",
			"", "", "", "", "", "", "",
			"", "", "7", "8", "9", "colon", "",
			"", "4", "5", "6", "0", "bspace",
			"", "", "1", "2", "3", "", "",
			"", "", "", "", "layer0",
			"", "", "", "", "", "enter",
		},
	}

	buf := bytes.NewBuffer(make([]byte, len(layers)*76*2))
	reverse := make(map[string]uint16)
	for _, value := range keys {
		reverse[value.name] = uint16(buf.Len())
		writeKeys(buf, reverse, value.stdKeys)
		writeKeys(buf, reverse, value.macKeys)
		binary.Write(buf, binary.BigEndian, value.colemak)
		binary.Write(buf, binary.BigEndian, value.qwerty)
	}

	data := buf.Bytes()
	for k1, v1 := range layers {
		for k2, v2 := range v1 {
			i, ok := reverse[v2]
			if !ok {
				log.Fatal("unrecognized " + v2 + " key referenced")
			}
			binary.BigEndian.PutUint16(data[76*2*k1+2*k2:], i)
		}
	}

	f, err := os.Create("qmk/keyboards/ergodox_ez/keymaps/xudongz/keymap.c")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	fmt.Fprint(f, "#include QMK_KEYBOARD_H\n\n")
	fmt.Fprint(f, "const unsigned char keymapData[] PROGMEM = {")
	for key := range data {
		if key%12 == 0 {
			fmt.Fprint(f, "\n\t")
		} else {
			fmt.Fprint(f, " ")
		}
		fmt.Fprint(f, "0x"+hex.EncodeToString(data[key:key+1])+",")
	}
	fmt.Fprint(f, "\n};\n\n")

	g, err := os.Open("keymap.c")
	if err != nil {
		log.Fatal(err)
	}
	defer g.Close()

	io.Copy(f, g)
}
