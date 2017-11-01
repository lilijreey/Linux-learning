global start ;内核入口， global make is public
extern long_mode_start

section .text ; code section
bits 32 ; 32bit instructions, default cpu is protected mode
start:
	;创建一个call stack
	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call init_page_tables
	call enable_paging

	; load the 64-bit GDT
	lgdt [gdt64.pointer]
	; 使用far jump 重建加载cs 寄存器,把cs制为gdt64
        jmp gdt64.code:long_mode_start
	; 之句话之后必须使用64位指令

	;print 'ok' to screen
	mov dword [0xb8000] , 0x2f4b2f4f
	hlt ;stop cpu

check_multiboot: ;检查已经加载kernel, multiboot2 规范规定在加载os前eax被设置为 0x36d76289
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret

.no_multiboot:
	mov al, "0"
	jmp error

check_cpuid:
    ; 检查cpu是否支持cpuid指令，如果支持就可以使用cpuid指令来查询是否支持long mode
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error


init_page_tables:
	;map first P4 enter to P3 table
	mov eax, p3_table
	or eax, 0b11; present + writable
	mov [p4_table], eax

	;map first P3 enter to P2 table
	mov eax, p2_table
	or eax, 0b11; present + writable
	mov [p3_table], eax

	;map each p2 enter to 2Mib page
	;first p2_table enter -> [0 ~ 2Mib)
	;second p2_table enter -> [2Mib~4Mib)
	;... for loop
	mov ecx, 0
.init_p2_table:
	mov eax, 0x200000; 2Mib
	mul ecx ;; eax *= ecx set offset ecx-th 2Mib
	or eax, 0b10000011; set present + writable + huge
	mov [p2_table + ecx * 8], eax

	inc ecx
	cmp ecx, 512; is over ?
	jne .init_p2_table

	ret


enable_paging:
    ;进入long mode, 开始分页
    ;1. 初始化CR3 指向顶级表
    mov eax, p4_table
    mov cr3, eax

    ; enable PAE-flag in rc4
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the EFER MSR (model specific register)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret








error:
	;打印ERR: +错误信息
	mov dword [0xb8000], 0xf4524f45 ;0xf4是color,52是E的ascii吗
	mov dword [0xb8004], 0x3f3a4f52 ; R
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

section .rodata
gdt64: ;x86的段机制是不能不用的，必须设置GDT 但是当前几乎都不知用段，所以只申请一个段给所有内存使用
	dq 0;
.code: equ $-gdt64 ; equ 是把 $-gdt64 赋值给.code
	dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
	dw $-gdt64 -1 ;$就是当前的地址;存入gdt的长度
	dq gdt64 ;存入gdt的地址;使用 lgdt 指令加载

section .bss
align 4096
;这里设置为内核保留的1G空间，并且把0~1G的虚拟地址映射到相同的物理地址上
;并把p4_table的第一个enter指向p3_table, p3_table的first enter 指向 p2_table
p4_table:
	resb 4096
p3_table:
	resb 4096
p2_table:
	resb 4096
;p1_table: 由于使用2M页，所以没有p1表
;        resb 4096
stack_bottom:
	resb 64 ;reserve byte 保留64字节
stack_top:


