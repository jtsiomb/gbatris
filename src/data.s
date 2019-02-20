	.section .rodata

	.globl msglogo_pixels
	.globl music_samples
	.globl music_num_samples

	.align 4
msglogo_pixels:
	.incbin "data/msglogo.img"

	@ two channels of music samples right first
	.align 4
music_samples:
	.incbin "data/music"
music_num_samples:
	.long (. - music_samples) / 2
