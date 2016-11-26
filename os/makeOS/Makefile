all:

boot: boot.bin
	dd if=boot.bin of=bootOS bs=512 count=1

boot.bin: boot.asm
	nasm boot.asm -o boot.bin

