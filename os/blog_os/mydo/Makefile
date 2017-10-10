
OBJECT = kernel.bin

all: $(OBJECT)

iso: kernel.bin
	cp kernel.bin isofiles/boot/
	grub-mkrescue -o os.iso isofiles

kernel.bin: linker.ld multiboot_header.o boot.o
	ld -n -o $@ -T $^

multiboot_header.o: multiboot_header.asm
	nasm -f elf64 multiboot_header.asm

boot.o: boot.asm 
	nasm -f elf64 boot.asm

startOS: os.iso
	qemu-system-x86_64 -cdrom os.iso

clean:
	rm -rf $(OBJECT) *.
