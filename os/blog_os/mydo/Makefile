
OBJECT = iso 

all: $(OBJECT)

iso: kernel.bin
	cp kernel.bin isofiles/boot/
	grub-mkrescue -d /usr/lib/grub/i386-pc  -o os.iso isofiles

isoelf: kernel.bin
	cp kernel.bin isofiles/boot/
	grub-mkrescue -o oself.iso isofiles

kernel.bin: linker.ld multiboot_header.o long_mod_init.o boot.o 
	ld -n -o $@ -T $^

multiboot_header.o: multiboot_header.asm
	nasm -f elf64 multiboot_header.asm

boot.o: boot.asm 
	nasm -f elf64 boot.asm

long_mod_init.o: long_mod_init.asm
	nasm -f elf64 long_mod_init.asm


startOS: os.iso
	qemu-system-x86_64 -cdrom os.iso

clean:
	rm -rf $(OBJECT) *.o *.iso
