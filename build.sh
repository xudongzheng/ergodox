set -e -o pipefail

cd $(dirname $0)

if [ ! -d qmk ]; then
	git clone git@github.com:qmk/qmk_firmware.git qmk
	(cd qmk && git checkout 8df044b86828a374fc2c872c2bedc2f4b567f5bf)
	apt-get install -y make gcc-avr avr-libc
fi

if [ ! -f keycode.h ]; then
	cp qmk/tmk_core/common/keycode.h .
fi

# Create custom keymap.
dest=qmk/keyboards/ergodox_ez/keymaps/xudongz
mkdir -p $dest
go generate
cp rules.mk $dest

cd qmk
make -j $(nproc) keyboard=ergodox_ez keymap=xudongz
