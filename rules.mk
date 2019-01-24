# Per https://goo.gl/NdhWAv, optimize binary size.
EXTRAFLAGS += -flto

# ErgoDox EZ's makefile enables many features that we do not need. Disable those
# to minimize binary rize. We need CUSTOM_MATRIX as we define our own matrix and
# EXTRAKEY_ENABLE for some of the media-related keys.
MOUSEKEY_ENABLE = no
RGBLIGHT_ENABLE = no
COMMAND_ENABLE = no
NKRO_ENABLE = no
UNICODE_ENABLE = no
SWAP_HANDS_ENABLE = no
