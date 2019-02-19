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
#include <stdarg.h>
#include "gbaregs.h"
#include "game.h"
#include "tileset.h"
#include "timer.h"
#include "keyb.h"
#include "scoredb.h"
#include "uiscr.h"
#include "sound.h"
#include "debug.h"

#define LOGO_SIZE	(240 * 160 * 2)
#define MSGLOGO_TIME	2000

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
		while(REG_VCOUNT < 160);
		update_keyb();

		if(KEYPRESS(KEY_START)) break;
	}

	/* we're done, prepare to start a new game */
	init_game();
}

#define VKEYB_ASCII_OFFS	32

#define VKEYB_ROWS		3
#define VKEYB_COLS		13
static const char vkeyb_chr[VKEYB_ROWS][VKEYB_COLS] = {
	"ABCDEFGHIJKLM",
	"NOPQRSTUVWXYZ",
	"0123456789 \b\n"
};

#define VKEYB_FIRST_ROW	13
#define VKEYB_FIRST_COL	2
#define SP_ROW		17
#define SP_COL		22
static int vkeyb_offs[][2] = {
	{SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL},
	{SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL},
	{SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL},
	{SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL},
	/* 0 - 9 */
	{17, 2}, {17, 4}, {17, 6}, {17, 8}, {17, 10}, {17, 12}, {17, 14}, {17, 16},
	{17, 18}, {17, 20},
	{SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL},
	{SP_ROW, SP_COL}, {SP_ROW, SP_COL}, {SP_ROW, SP_COL},
	/* A-M */
	{13, 2}, {13, 4}, {13, 6}, {13, 8}, {13, 10}, {13, 12}, {13, 14}, {13, 16},
	{13, 18}, {13, 20}, {13, 22}, {13, 24}, {13, 26},
	/* N-Z */
	{15, 2}, {15, 4}, {15, 6}, {15, 8}, {15, 10}, {15, 12}, {15, 14}, {15, 16},
	{15, 18}, {15, 20}, {15, 22}, {15, 24}, {15, 26}
};

static void copy_bigtile(int srow, int scol, int drow, int dcol, int palidx);
static void big_print(int crow, int ccol, int cpal, const char *fmt, ...);

#define DRAW_NAME()	\
	do { \
		big_print(8, 24, PAL_VKEYB_INV, "    "); \
		big_print(8, 24, PAL_VKEYB_INV, "%s", name); \
	} while(0)

#define DEF_NAME	(name[name_len] != 0)

char *name_screen(int score)
{
	int i, j;
	int curx = 0, cury = 0, next_curx = VKEYB_COLS - 1, next_cury = VKEYB_ROWS - 1;
	uint16_t *dptr = scrmem;
	unsigned int *sptr = namescr_tilemap;
	static char name[5];
	int name_len = 0;

	/* retrieve the last highscore name */
	strcpy(name, scores[10].name);

	for(i=0; i<SCR_ROWS; i++) {
		for(j=0; j<SCR_COLS; j++) {
			*dptr++ = (*sptr++ + tile_namescr_start) | BGTILE_PAL(PAL_VKEYB);
		}
		dptr += VIRT_COLS - SCR_COLS;
	}

	big_print(4, 15, PAL_VKEYB, "%d", score);
	DRAW_NAME();

	key_repeat(500, 75, KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN);

	for(;;) {
		while(REG_VCOUNT < 160);
		update_keyb();

		if(KEYPRESS(KEY_START)) {
			goto done;
		}
		if(KEYPRESS(KEY_A)) {
			int c = vkeyb_chr[cury][curx];
			switch(c) {
			case '\n':
				goto done;
			case '\b':
				if(name_len) {
					name[--name_len] = 0;
				}
				break;
			default:
				if(name_len < NAME_SIZE) {
					name[name_len++] = c;
					name[name_len] = 0;
				}
			}
			DRAW_NAME();
		}
		if(KEYPRESS(KEY_B)) {
			if(name_len) {
				name[--name_len] = 0;
				DRAW_NAME();
			}
		}

		if(KEYPRESS(KEY_LEFT)) {
			next_curx = (curx + VKEYB_COLS - 1) % VKEYB_COLS;
			if(DEF_NAME) {
				name[name_len] = 0;
				DRAW_NAME();
			}
		}
		if(KEYPRESS(KEY_RIGHT)) {
			next_curx = (curx + 1) % VKEYB_COLS;
			if(DEF_NAME) {
				name[name_len] = 0;
				DRAW_NAME();
			}
		}
		if(KEYPRESS(KEY_UP)) {
			next_cury = (cury + VKEYB_ROWS - 1) % VKEYB_ROWS;
			if(DEF_NAME) {
				name[name_len] = 0;
				DRAW_NAME();
			}
		}
		if(KEYPRESS(KEY_DOWN)) {
			next_cury = (cury + 1) % VKEYB_ROWS;
			if(DEF_NAME) {
				name[name_len] = 0;
				DRAW_NAME();
			}
		}

		if(next_curx != curx || next_cury != cury) {
			int row = VKEYB_FIRST_ROW + cury * 2;
			int col = VKEYB_FIRST_COL + curx * 2;
			copy_bigtile(row, col, row, col, PAL_VKEYB);
			curx = next_curx;
			cury = next_cury;
			row = VKEYB_FIRST_ROW + cury * 2;
			col = VKEYB_FIRST_COL + curx * 2;
			copy_bigtile(row, col, row, col, PAL_VKEYB_HL);
		}
	}

done:
	return name;
}

static void copy_bigtile(int srow, int scol, int drow, int dcol, int palidx)
{
	uint16_t *dptr = scrmem + drow * VIRT_COLS + dcol;
	uint16_t pal = BGTILE_PAL(palidx);
	unsigned int *tmap = namescr_tilemap + srow * SCR_COLS + scol;

	dptr[0] = (tmap[0] + tile_namescr_start) | pal;
	dptr[1] = (tmap[1] + tile_namescr_start) | pal;
	dptr[VIRT_COLS] = (tmap[SCR_COLS] + tile_namescr_start) | pal;
	dptr[VIRT_COLS + 1] = (tmap[SCR_COLS + 1] + tile_namescr_start) | pal;
}

static void big_print(int crow, int ccol, int cpal, const char *fmt, ...)
{
	va_list ap;
	int len, glyph_row, glyph_col;
	char buf[24];
	char *s = buf;

	va_start(ap, fmt);
	len = vsprintf(buf, fmt, ap);
	va_end(ap);

	ccol -= len;

	while(*s) {
		int c = *s++;
		if(c == ' ') {
			glyph_row = glyph_col = 0;
		} else {
			glyph_row = vkeyb_offs[c - VKEYB_ASCII_OFFS][0];
			glyph_col = vkeyb_offs[c - VKEYB_ASCII_OFFS][1];
		}
		copy_bigtile(glyph_row, glyph_col, crow, ccol, cpal);
		ccol += 2;
	}
}

void draw_str(int x, int y, const char *s, int pal)
{
	uint16_t *dptr = scrmem + y * VIRT_COLS + x;

	while(*s) {
		*dptr++ = (uint16_t)*s++ | BGTILE_PAL(pal);
	}
}
