	.arm
	.globl read_sram
	.globl write_sram

	.equ SRAM_ADDR, 0xe000000

@ void read_sram(int sram_offs, void *buf, int size)
read_sram:
	cmp r2, #0
	beq 1f
	ldr r3, =SRAM_ADDR
	add r0, r3
0:	ldrb r3, [r0], #1
	strb r3, [r1], #1
	subs r2, #1
	bne 0b
1:	bx lr

@ void write_sram(int sram_offs, void *buf, int size)
write_sram:
	cmp r2, #0
	beq 1f
	ldr r3, =SRAM_ADDR
	add r0, r3
0:	ldrb r3, [r1], #1
	strb r3, [r0], #1
	subs r2, #1
	bne 0b
1:	bx lr
