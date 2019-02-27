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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>
#include "game.h"
#include "uiscr.h"
#include "blocks.h"
#include "scoredb.h"
#include "tileset.h"
#include "timer.h"
#include "gbaregs.h"
#include "sound.h"

#define MUSIC_SAMPLE_RATE	22050

/* defined in data.s */
extern unsigned char music_samples[];
extern unsigned long music_num_samples;


enum { ERASE_PIECE, DRAW_PIECE };

/* dimensions of the playfield */
#define PF_ROWS		18
#define PF_COLS		10
/* offset of the playfield from the left side of the screen */
#define PF_XOFFS	7
#define PF_YOFFS	0

uint16_t scr[SCR_COLS * SCR_ROWS];

int music = 1;

static void update_cur_block(void);
static void addscore(int nlines);
static void print_numbers(void);
static int spawn(void);
static int collision(int block, const int *pos);
static void stick(int block, const int *pos);
static void erase_completed(void);
static void draw_block(int block, const int *pos, int rot, int mode);
static void drawbg(void);
static void drawpf(void);
static void draw_line(int row, int blink);
static void place_str(int x, int y, const char *s);


static int pos[2], next_pos[2];
static int cur_block, next_block, prev_block;
static int cur_rot, prev_rot;
static int complines[4];
static int num_complines;
static int gameover;
static int pause;
static int score, level, lines;
static int just_spawned;

static const char *bgdata[SCR_ROWS] = {
	"      L..........R{-----}     ",
	"      L..........R(.....)     ",
	"      L..........R[_____]     ",
	"      L..........R.......     ",
	"      L..........R            ",
	"      L..........R{-----}     ",
	"      L..........R(.....)     ",
	"      L..........R(.....)     ",
	"      L..........R>=====<     ",
	"      L..........R(.....)     ",
	"      L..........R(.....)     ",
	"      L..........R[_____]     ",
	"      L..........R {----}     ",
	"      L..........R (....)     ",
	"      L..........R (....)     ",
	"      L..........R (....)     ",
	"      L..........R (....)     ",
	"      L..........R [____]     ",
	"      `BBBBBBBBBB/            ",
	"                              "
};

#define NUM_LEVELS	21
static const long level_speed[NUM_LEVELS] = {
	887, 820, 753, 686, 619, 552, 469, 368, 285, 184,
	167, 151, 134, 117, 107, 98, 88, 79, 69, 60, 50
};

int init_game(void)
{
	int i, j;
	uint16_t *row = scr;

	uint32_t seed = REG_TM0CNT_L | (timer_msec << 16);
	srand(seed);

	pause = 0;
	gameover = 0;
	num_complines = 0;
	score = level = lines = 0;
	tick_interval = level_speed[0];
	cur_block = -1;
	prev_block = 0;
	next_block = rand() % NUM_BLOCKS;

	memset(scrmem, 0, VIRT_COLS * VIRT_ROWS * 2);

	/* fill the screen buffer, and draw */
	for(i=0; i<SCR_ROWS; i++) {
		for(j=0; j<SCR_COLS; j++) {
			int tile;
			switch(bgdata[i][j]) {
			case 'L':
				tile = TILE_LPFSEP;
				break;
			case 'R':
				tile = TILE_RPFSEP;
				break;
			case 'B':
				tile = TILE_BPFSEP;
				break;
			case '`':
				tile = TILE_BLPFSEP;
				break;
			case '/':
				tile = TILE_BRPFSEP;
				break;
			case '.':
				tile = TILE_PF;
				break;
			case '{':
				tile = TILE_FRM_TL;
				break;
			case '}':
				tile = TILE_FRM_TR;
				break;
			case '[':
				tile = TILE_FRM_BL;
				break;
			case ']':
				tile = TILE_FRM_BR;
				break;
			case '>':
				tile = TILE_FRM_LTEE;
				break;
			case '<':
				tile = TILE_FRM_RTEE;
				break;
			case '-':
				tile = TILE_FRM_THLINE;
				break;
			case '_':
				tile = TILE_FRM_BHLINE;
				break;
			case '=':
				tile = TILE_FRM_MHLINE;
				break;
			case '(':
				tile = TILE_FRM_LVLINE;
				break;
			case ')':
				tile = TILE_FRM_RVLINE;
				break;
			case ' ':
			default:
				tile = TILE_BLACK;
			}
			row[j] = tile;
		}
		row += SCR_COLS;
	}

	place_str(12 + PF_XOFFS, 1, "SCORE");
	place_str(12 + PF_XOFFS, 6, "LEVEL");
	place_str(12 + PF_XOFFS, 9, "LINES");

	drawbg();
	print_numbers();

	if(music) {
		void *rdata = music_samples;
		void *ldata = music_samples + music_num_samples;
		play_dsound(rdata, ldata, 0, music_num_samples, MUSIC_SAMPLE_RATE, DS_LOOP | DS_STEREO);
	}
	return 0;
}

#define BLINK_UPD_RATE	100
#define GAMEOVER_FILL_RATE	50
#define WAIT_INF	0x7fffffff

long update(long msec)
{
	static long prev_tick;
	long dt;

	if(pause) {
		prev_tick = msec;
		return WAIT_INF;
	}

	dt = msec - prev_tick;

	if(gameover) {
		int i, row = PF_ROWS - gameover;
		uint16_t *ptr;

		if(dt < GAMEOVER_FILL_RATE) {
			return GAMEOVER_FILL_RATE - dt;
		}

		if(row >= 0) {
			ptr = scr + (row + PF_YOFFS) * SCR_COLS + PF_XOFFS;
			for(i=0; i<PF_COLS; i++) {
				*ptr++ = TILE_GAMEOVER;
			}
			draw_line(row, 1);

			gameover++;
			prev_tick = msec;
			return GAMEOVER_FILL_RATE;
		}

		return WAIT_INF;
	}

	if(num_complines) {
		/* lines where completed, we're in blinking mode */
		int i, blink = dt >> 8;

		if(blink > 6) {
			erase_completed();
			num_complines = 0;
			return 0;
		}

		for(i=0; i<num_complines; i++) {
			draw_line(complines[i], blink & 1);
		}
		return BLINK_UPD_RATE;
	}

	/* fall */
	while(dt >= tick_interval) {
		if(cur_block >= 0) {
			just_spawned = 0;
			next_pos[0] = pos[0] + 1;
			if(collision(cur_block, next_pos)) {
				next_pos[0] = pos[0];
				stick(cur_block, next_pos);
				return 0;
			}
		} else {
			/* respawn */
			if(spawn() == -1) {
				gameover = 1;
				if(music) {
					stop_dsound();	/* stop the music */
				}
				return 0;
			}
		}

		dt -= tick_interval;
		prev_tick = msec;
	}

	update_cur_block();
	return tick_interval - dt;
}

static void update_cur_block(void)
{
	if(cur_block < 0) return;

	if(memcmp(pos, next_pos, sizeof pos) != 0 || cur_rot != prev_rot) {
		draw_block(cur_block, pos, prev_rot, ERASE_PIECE);
		draw_block(cur_block, next_pos, cur_rot, DRAW_PIECE);
		memcpy(pos, next_pos, sizeof pos);
		prev_rot = cur_rot;
	}
}

static void addscore(int nlines)
{
	static const int stab[] = {40, 100, 300, 1200};	/* bonus per line completed */

	assert(nlines < 5);

	score += stab[nlines - 1] * (level + 1);
	lines += nlines;

	level = lines / 10;
	if(level > NUM_LEVELS - 1) level = NUM_LEVELS - 1;

	tick_interval = level_speed[level];

	print_numbers();
}

static void print_numbers(void)
{
	char buf[16];

	if(score > 999999) {
		sprintf(buf, "%7d", score);
	} else {
		sprintf(buf, "%6d", score);
	}
	draw_str(11 + PF_XOFFS, 3, buf, 0);

	sprintf(buf, "%2d", level);
	draw_str(14 + PF_XOFFS, 7, buf, 0);

	if(lines > 9999) {
		sprintf(buf, "%5d", lines);
	} else {
		sprintf(buf, "%4d", lines);
	}
	draw_str(12 + PF_XOFFS, 10, buf, 0);
}

void game_input(int c)
{
	char *name = 0;

	switch(c) {
	case 'a':
		if(!pause) {
			next_pos[1] = pos[1] - 1;
			if(collision(cur_block, next_pos)) {
				next_pos[1] = pos[1];
			} else {
				snd_shift();
			}
		}
		break;

	case 'd':
		if(!pause) {
			next_pos[1] = pos[1] + 1;
			if(collision(cur_block, next_pos)) {
				next_pos[1] = pos[1];
			} else {
				snd_shift();
			}
		}
		break;

	case 'w':
		if(!pause) {
			prev_rot = cur_rot;
			cur_rot = (cur_rot + 1) & 3;
			if(collision(cur_block, next_pos)) {
				cur_rot = prev_rot;
			} else {
				snd_rot();
			}
		}
		break;

	case 's':
		/* ignore drops until the first update after a spawn */
		if(cur_block >= 0 && !just_spawned && !pause) {
			next_pos[0] = pos[0] + 1;
			if(collision(cur_block, next_pos)) {
				next_pos[0] = pos[0];
				update_cur_block();
				stick(cur_block, next_pos);	/* stick immediately */
			}
		}
		break;

	case '\n':
		if(!pause && cur_block >= 0) {
			next_pos[0] = pos[0] + 1;
			while(!collision(cur_block, next_pos)) {
				next_pos[0]++;
			}
			next_pos[0]--;
			update_cur_block();
			stick(cur_block, next_pos);	/* stick immediately */
		}
		break;

	case 'p':
		if(gameover) {
			if(score && is_highscore(score)) {
				name = name_screen(score);
			}
			save_score(name, score, lines, level);
			score_screen();
		} else {
			pause ^= 1;

			if(music) {
				if(pause) {
					pause_dsound();
				} else {
					resume_dsound();
				}
			}
		}
		break;

	case '\b':
		if(music) {
			stop_dsound();
		}
		if(score && is_highscore(score)) {
			name = name_screen(score);
		}
		save_score(name, score, lines, level);
		score_screen();
		break;

	case 'm':
		music ^= 1;
		if(music) {
			void *right = music_samples;
			void *left = music_samples + music_num_samples;
			play_dsound(right, left, 0, music_num_samples, MUSIC_SAMPLE_RATE, DS_LOOP | DS_STEREO);
		} else {
			stop_dsound();
		}
		break;

	default:
		break;
	}
}

static int spawn(void)
{
	static const int preview_pos[] = {13, 13};
	int r, tries = 2;

	do {
		r = rand() % NUM_BLOCKS;
	} while(tries-- > 0 && (r | prev_block | next_block) == prev_block);

	draw_block(next_block, preview_pos, 0, ERASE_PIECE);
	draw_block(r, preview_pos, 0, DRAW_PIECE);

	cur_block = next_block;
	next_block = r;

	prev_rot = cur_rot = 0;
	pos[0] = block_spawnpos[cur_block][0];
	next_pos[0] = pos[0] + 1;
	pos[1] = next_pos[1] = PF_COLS / 2 + block_spawnpos[cur_block][1];

	if(collision(cur_block, next_pos)) {
		return -1;
	}

	just_spawned = 1;
	return 0;
}

static int collision(int block, const int *pos)
{
	int i;
	unsigned char *p = blocks[block][cur_rot];

	for(i=0; i<4; i++) {
		int x = PF_XOFFS + pos[1] + BLKX(*p);
		int y = PF_YOFFS + pos[0] + BLKY(*p);
		p++;

		if(y < 0) continue;

		if(scr[y * SCR_COLS + x] != TILE_PF) return 1;
	}

	return 0;
}

static void stick(int block, const int *pos)
{
	int i, j, nblank;
	uint16_t *pfline;
	unsigned char *p = blocks[block][cur_rot];

	num_complines = 0;
	prev_block = cur_block;	/* used by the spawn routine */
	cur_block = -1;

	for(i=0; i<4; i++) {
		int x = pos[1] + BLKX(*p);
		int y = pos[0] + BLKY(*p);
		p++;

		pfline = scr + (y + PF_YOFFS) * SCR_COLS + PF_XOFFS;
		pfline[x] = TILE_BLOCK | BGTILE_PAL(block + FIRST_BLOCK_PAL);

		nblank = 0;
		for(j=0; j<PF_COLS; j++) {
			if(pfline[j] == TILE_PF) {
				nblank++;
			}
		}

		if(nblank == 0) {
			complines[num_complines++] = y;
		}
	}

	snd_stick();

	if(num_complines) {
		addscore(num_complines);
	}
}

static void erase_completed(void)
{
	int i, j, srow, drow;
	uint16_t *pfstart = scr + PF_YOFFS * SCR_COLS + PF_XOFFS;
	uint16_t *dptr;

	/* sort completed lines from highest to lowest row number */
	for(i=0; i<num_complines-1; i++) {
		for(j=i+1; j<num_complines; j++) {
			if(complines[j] > complines[i]) {
				int tmp = complines[j];
				complines[j] = complines[i];
				complines[i] = tmp;
			}
		}
	}

	srow = drow = PF_ROWS - 1;
	dptr = pfstart + drow * SCR_COLS;

	for(i=0; i<PF_ROWS; i++) {
		for(j=0; j<num_complines; j++) {
			if(complines[j] == srow) {
				srow--;
			}
		}

		if(srow < 0) {
			for(j=0; j<PF_COLS; j++) {
				dptr[j] = TILE_PF;
			}

		} else if(srow != drow) {
			uint16_t *sptr = pfstart + srow * SCR_COLS;
			memcpy(dptr, sptr, PF_COLS * sizeof *dptr);
		}

		srow--;
		drow--;
		dptr -= SCR_COLS;
	}

	drawpf();
}

static void draw_block(int block, const int *pos, int rot, int mode)
{
	int i, tile, pal;
	uint16_t tval;
	unsigned char *p = blocks[block][rot];

	if(mode == ERASE_PIECE) {
		tile = TILE_PF;
		pal = 0;
	} else {
		tile = TILE_BLOCK;
		pal = FIRST_BLOCK_PAL + block;
	}
	tval = tile | BGTILE_PAL(pal);

	for(i=0; i<4; i++) {
		int x = PF_XOFFS + pos[1] + BLKX(*p);
		int y = PF_YOFFS + pos[0] + BLKY(*p);
		p++;

		if(y < 0) continue;

		scrmem[y * VIRT_COLS + x] = tval;
	}
}

static void drawbg(void)
{
	int i, j;
	uint16_t *sptr = scr;
	uint16_t *dptr = scrmem;

	for(i=0; i<SCR_ROWS; i++) {
		for(j=0; j<SCR_COLS; j++) {
			*dptr++ = *sptr++;
		}
		dptr += VIRT_COLS - SCR_COLS;
	}
}

static void drawpf(void)
{
	int i, j;
	uint16_t *sptr = scr + PF_YOFFS * SCR_COLS + PF_XOFFS;
	uint16_t *dptr = scrmem + PF_XOFFS;

	for(i=0; i<PF_ROWS; i++) {
		for(j=0; j<PF_COLS; j++) {
			*dptr++ = *sptr++;
		}
		sptr += SCR_COLS - PF_COLS;
		dptr += VIRT_COLS - PF_COLS;
	}
}

static void draw_line(int row, int blink)
{
	int i;
	uint16_t *dptr = scrmem + row * VIRT_COLS + PF_XOFFS;

	if(blink) {
		uint16_t *sptr = scr + (row + PF_YOFFS) * SCR_COLS + PF_XOFFS;

		for(i=0; i<PF_COLS; i++) {
			*dptr++ = *sptr++;
		}
	} else {
		for(i=0; i<PF_COLS; i++) {
			*dptr++ = TILE_PF;
		}
	}
}


static void place_str(int x, int y, const char *s)
{
	uint16_t *dptr = scr + y * SCR_COLS + x;

	while(*s) {
		*dptr++ = *s++;
	}
}

void dbgblock(int x, int y, int pal)
{
	scrmem[y * VIRT_COLS + x] = TILE_X | BGTILE_PAL(pal + 8);
}
