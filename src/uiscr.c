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
#include <stdio.h>
#include <string.h>
#include "gbaregs.h"
#include "game.h"
#include "tileset.h"
#include "timer.h"
#include "scoredb.h"
#include "uiscr.h"

#define LOGO_SIZE	(240 * 160 * 2)
#define MSGLOGO_TIME	2000

static uint16_t prevstate, keystate, keydelta;

extern unsigned char msglogo_pixels[];
extern unsigned int scorescr_tilemap[];
extern unsigned int namescr_tilemap[];

void splash_screen(void)
{
	unsigned long start;
	void *fbptr = (void*)VRAM_LFB_FB0_ADDR;

	/* mode 3 (240x160 16bpp) */
	REG_DISPCNT = 3 | DISPCNT_BG2;

	memcpy(fbptr, msglogo_pixels, LOGO_SIZE);
	start = timer_msec;

	while(timer_msec - start < MSGLOGO_TIME);
}

#define NM_COL	16
#define SC_COL	22

static const char *scorepal[SCR_ROWS] = {
	"<<<<<<<<<<<<>00000000000000000",
	"<<<<<<<<<<<<>00000000000000000",
	"888888888888>00000000000000000",
	"888888888888>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"999999999999>00000000000000000",
	"999999999999>00000000000000000",
	">>>>>>>>>>>>>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000",
	"000000000000>00000000000000000"
};

void score_screen(void)
{
	int i, j, row, hl;
	uint16_t *dptr = scrmem;
	unsigned int *sptr = scorescr_tilemap;
	char buf[32];

	load_scores();

	for(i=0; i<SCR_ROWS; i++) {
		for(j=0; j<SCR_COLS; j++) {
			uint16_t pal = BGTILE_PAL(scorepal[i][j] - '0');
			*dptr++ = (*sptr++ + tile_scorescr_start) | pal;
		}
		dptr += VIRT_COLS - SCR_COLS;
	}

	for(i=0; i<10; i++) {
		if(scores[i].score <= 0) break;

		row = i * 2 + 1;
		hl = i == last_score_rank;
		draw_str(NM_COL, row, scores[i].name, hl ? PAL_SCOREHL : PAL_SCORE);
		sprintf(buf, "%7u", (unsigned int)scores[i].score);
		draw_str(SC_COL, row, buf, hl ? PAL_SCOREHL : PAL_SCORE);
	}

	if(last_score_rank >= 0) {
		/* change the palette of the number tile too */
		row = last_score_rank * 2;
		sptr = scorescr_tilemap + row * SCR_COLS + 13;
		dptr = scrmem + row * VIRT_COLS + 13;
		for(i=0; i<2; i++) {
			for(j=0; j<3; j++) {
				uint16_t tile = sptr[j] + tile_scorescr_start;
				dptr[j] = tile | BGTILE_PAL(PAL_SCOREHL + 1);
			}
			dptr += VIRT_COLS;
			sptr += SCR_COLS;
		}
	}

	for(;;) {
		keystate = ~REG_KEYINPUT;
		keydelta = keystate ^ prevstate;
		prevstate = keystate;

		if(((keydelta & KEY_START) && !(keystate & KEY_START))) {
			break;
		}
	}

	/* we're done, prepare to start a new game */
	init_game();
}

char *name_screen(int score)
{
	int i, j;
	uint16_t *dptr = scrmem;
	unsigned int *sptr = namescr_tilemap;
	static char *name = "NUC";	/* TODO */

	for(i=0; i<SCR_ROWS; i++) {
		for(j=0; j<SCR_COLS; j++) {
			*dptr++ = *sptr++ + tile_namescr_start;
		}
		dptr += VIRT_COLS - SCR_COLS;
	}

	for(;;) {
		keystate = ~REG_KEYINPUT;
		keydelta = keystate ^ prevstate;
		prevstate = keystate;

		if(((keydelta & KEY_START) && !(keystate & KEY_START))) {
			break;
		}
	}

	return name;
}

void draw_str(int x, int y, const char *s, int pal)
{
	uint16_t *dptr = scrmem + y * VIRT_COLS + x;

	while(*s) {
		*dptr++ = (uint16_t)*s++ | BGTILE_PAL(pal);
	}
}
