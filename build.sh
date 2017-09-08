set -e -o pipefail

cd $(dirname $0)

if [ ! -d qmk ]; then
	git clone git@github.com:qmk/qmk_firmware.git
	cd qmk_firmware
	git checkout 7260fc3eef98fb7b0e2ed24d3d0d14cf2e613000
	cd ..
	mv qmk_firmware qmk
	apt-get install -y gcc-avr avr-libc
fi

dest=qmk/keyboards/ergodox/keymaps/xudongz
mkdir -p $dest
cp keymap.c $dest/keymap.c

cd qmk
make -j $(nproc) keyboard=ergodox keymap=xudongz
