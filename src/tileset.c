#include <stdlib.h>
#include <string.h>
#include "tileset.h"
#include "gbaregs.h"
#include "bgtiles.h"

static const unsigned char blkcols[][3] = {
	{31, 15, 10},
	{20, 20, 31},
	{10, 31, 15},
	{31, 31, 10},
	{10, 31, 31},
	{31, 10, 28},
	{20, 20, 20}
};
#define NUM_BLKCOLS	(sizeof blkcols / sizeof *blkcols)


static const unsigned char blktile_pixels[] = {
	13, 14, 14, 14, 14, 14, 14, 9,
	10, 8, 8, 8, 8, 8, 8, 5,
	10, 8, 8, 8, 8, 8, 8, 4,
	10, 8, 8, 4, 6, 8, 8, 4,
	10, 8, 8, 9, 12, 8, 8, 4,
	10, 8, 8, 8, 8, 8, 8, 4,
	10, 8, 8, 8, 8, 8, 8, 4,
	7, 1, 1, 1, 1, 1, 1, 2
};

static const unsigned char blktile_pal_value[] = {
	0, 8, 8, 9, 10, 10, 12, 14, 15, 18, 19, 19, 20, 21, 22, 23
};

#define TILE_SIZE		0x20

void setup_tileset(void *chrmem)
{
	int i, j;
	uint16_t *cptr;
	uint16_t *dest = chrmem;
	const unsigned char *src;

	/* background tiles */
	src = bgtiles_pixels;
	for(i=0; i<BGTILES_DATA_SIZE / 4; i++) {
		*dest++ = (uint16_t)src[0] | ((uint16_t)src[1] << 4) |
			((uint16_t)src[2] << 8) | ((uint16_t)src[3] << 12);
		src += 4;
	}

	/* block tile */
	src = blktile_pixels;
	for(i=0; i<TILE_SIZE / 2; i++) {
		*dest++ = (uint16_t)src[0] | ((uint16_t)src[1] << 4) |
			((uint16_t)src[2] << 8) | ((uint16_t)src[3] << 12);
		src += 4;
	}

	/* make palettes */
	cptr = (uint16_t*)CRAM_BG_ADDR;
	for(i=0; i<FIRST_PIECE_PAL; i++) {
		for(j=0; j<16; j++) {
			int v = (j << 1) | (j >> 3);
			*cptr++ = v | (v << 5) | (v << 10);
		}
	}

	for(i=0; i<NUM_BLKCOLS; i++) {
		int cr = blkcols[i][0];
		int cg = blkcols[i][1];
		int cb = blkcols[i][2];

		for(j=0; j<16; j++) {
			int v = blktile_pal_value[j];
			int r = cr * v / 31;
			int g = cg * v / 31;
			int b = cb * v / 31;

			*cptr++ = r | (g << 5) | (b << 10);
		}
	}
}
