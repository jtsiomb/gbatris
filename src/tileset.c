#include <stdlib.h>
#include <string.h>
#include "tileset.h"
#include "gbaregs.h"
#include "bgtiles.h"
#include "fontdata.h"


static const unsigned char blkcols[][3] = {
	{31, 20, 0},
	{5, 31, 0},
	{20, 31, 0},
	{31, 31, 0},
	{0, 31, 31},
	{31, 15, 31},
	{31, 8, 0}
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
	0, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 27, 28, 29, 30, 31
};

#define TILE_SIZE		0x20

void setup_tileset(void *chrmem)
{
	int i, j, k;
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

	/* font tiles */
	dest = (uint16_t*)((char*)chrmem + TILE_FONT_START * TILE_SIZE);
	src = fontdata;
	for(i=0; i<FONT_COUNT; i++) {
		for(j=0; j<8; j++) {
			uint32_t row = 0;
			unsigned char fpix = *src++;
			for(k=0; k<8; k++) {
				int bit = (fpix >> k) & 1;
				row = (row << 4) | (bit ? 2 : 13);
			}
			*dest++ = row & 0xffff;
			*dest++ = row >> 16;
		}
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
