set -e -o pipefail

cd $(dirname $0)

# TODO given QMK version is supposed to handle Caps Lock correctly on macOS but
# it still doesn't seem to work. We should look into that.
if [ ! -d qmk ]; then
	git clone git@github.com:qmk/qmk_firmware.git qmk
	(cd qmk && git checkout b91ffba4be0da2b587fae7a8a25d5dfce2f55d94)
	apt-get install -y make gcc-avr avr-libc
fi

# TODO QMK ends up looking for files within the keyboards/ergodox/ez directory
# rather than the keyboards/ergodox_ez directory. It's likely a bug and this is
# a temporary workaround.
mkdir qmk/keyboards/ergodox
ln -s $PWD/qmk/keyboards/ergodox_ez $PWD/qmk/keyboards/ergodox/ez

# Create custom keymap.
dest=qmk/keyboards/ergodox_ez/keymaps/xudongz
mkdir -p $dest
cp keymap.c $dest/keymap.c

cd qmk
make -j $(nproc) keyboard=ergodox_ez keymap=xudongz
