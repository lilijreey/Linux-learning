global start ;内核入口， global make is public

section .text ; code section
bits 32 ; 32bit instructions, default cpu is protected mode
start:
	;print 'ok' to screen
	mov dword [0xb800] , 0x2f4b2f4f
	hlt ;stop cpu
