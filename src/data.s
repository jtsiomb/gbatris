	.section .rodata

	.globl msglogo_pixels
	.globl gbatris_pixels

	.align 4
msglogo_pixels:
	.incbin "data/msglogo.img"

	.align 4
gbatris_pixels:
	.incbin "data/gbatris.img"
