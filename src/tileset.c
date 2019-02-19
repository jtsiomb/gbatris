/*
gbatris - a tetris game for the GameBoy Advance
Copyright (C) 2019  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <string.h>
#include "tileset.h"
#include "gbaregs.h"
#include "bgtiles.h"
#include "fontdata.h"
#include "game.h"


static const unsigned char blkcols[][3] = {
	{31, 20, 0},
	{5, 31, 0},
	{20, 20, 31},
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

extern unsigned char scorescr_tiles[];
extern int scorescr_num_tiles;
extern unsigned char namescr_tiles[];
extern int namescr_num_tiles;
extern unsigned char namescr_cmap[][3];

#define TILE_SIZE		0x20
#define ADDR_TO_TILE(x)		(((uint32_t)(x) - (uint32_t)chrmem) / TILE_SIZE)

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
	tile_block_start = ADDR_TO_TILE(dest);
	src = blktile_pixels;
	for(i=0; i<TILE_SIZE / 2; i++) {
		*dest++ = (uint16_t)src[0] | ((uint16_t)src[1] << 4) |
			((uint16_t)src[2] << 8) | ((uint16_t)src[3] << 12);
		src += 4;
	}

	/* font tiles */
	tile_font_start = TILE_FONT_START;
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

	/* tiles for the menus */
	tile_scorescr_start = ADDR_TO_TILE(dest);
	src = scorescr_tiles;
	for(i=0; i<scorescr_num_tiles; i++) {
		for(j=0; j<TILE_SIZE / 2; j++) {
			uint16_t p4;
			unsigned char pp = *src++;
			pp = (pp >> 4) | (pp << 4);
			p4 = (uint16_t)pp;
			pp = *src++;
			pp = (pp >> 4) | (pp << 4);
			p4 |= (uint16_t)pp << 8;
			*dest++ = p4;
		}
	}

	tile_namescr_start = ADDR_TO_TILE(dest);
	src = namescr_tiles;
	for(i=0; i<namescr_num_tiles; i++) {
		for(j=0; j<TILE_SIZE / 2; j++) {
			uint16_t p4;
			unsigned char pp = *src++;
			pp = (pp >> 4) | (pp << 4);
			p4 = (uint16_t)pp;
			pp = *src++;
			pp = (pp >> 4) | (pp << 4);
			p4 |= (uint16_t)pp << 8;
			*dest++ = p4;
		}
	}

	chrmem_top = dest;

	/* make palettes */
	cptr = (uint16_t*)CRAM_BG_ADDR;
	for(i=0; i<16; i++) {
		int v = (i << 1) | (i >> 3);
		*cptr = v | (v << 5) | (v << 10);
		v = ~v & 0x1f;
		cptr[16] = v | (v << 5) | (v << 10);
		cptr++;
	}

	cptr = (uint16_t*)CRAM_BG_ADDR + PAL_SCORE * 16;
	for(i=0; i<16; i++) {
		*cptr++ = i < 8 ? 0x7fff : 0;
	}
	/* PAL_SCOREHL */
	for(i=0; i<16; i++) {
		*cptr++ = i < 8 ? 0x01ff : 0;
	}
	/* PAL_SCOREHL_NUM */
	for(i=0; i<16; i++) {
		int v = (i << 1) | (i >> 3);
		*cptr++ = ((v << 4) & 0x1e0) | v;
	}

	/* PAL_VKEYB & PAL_VKEYB_INV */
	for(i=0; i<16; i++) {
		int v = namescr_cmap[i][0] * 31 / 255;
		cptr[i] = (v << 10) | (v << 5) | v;
	}
	cptr[15] = 0;
	cptr += 16;
	for(i=0; i<16; i++) {
		int idx = 17 - i;
		int v = namescr_cmap[idx > 14 ? 14 : idx][0] * 31 / 255;
		cptr[i] = (v << 10) | (v << 5) | v;
	}
	cptr[15] = 0x7fff;
	cptr += 16;
	/* PAL_VKEYB_HL */
	for(i=0; i<16; i++) {
		if(i == 15) {
			*cptr++ = 0x7f0c;
		} else {
			int v = (i << 1) | (i >> 3);
			*cptr++ = v | (v << 5) | (v << 10);
		}
	}


	cptr = (uint16_t*)CRAM_BG_ADDR + FIRST_BLOCK_PAL * 16;
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
