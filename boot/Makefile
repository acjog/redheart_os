test: test.asm
	nasm test.asm -o test.bin
	sudo losetup /dev/loop0 ./a.img
	sudo dd if=./test.bin of=/dev/loop0 bs=512 count=1
	sudo losetup -d /dev/loop0
	/usr/local/bochs_2.6/bin/bochs -f ./bochsrc2

all:
	
