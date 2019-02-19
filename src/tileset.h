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
#ifndef TILESET_H_
#define TILESET_H_

enum {
	TILE_BLACK,
	TILE_PF,
	TILE_LPFSEP,
	TILE_RPFSEP,
	TILE_BPFSEP,
	TILE_BLPFSEP,
	TILE_BRPFSEP,
	TILE_GAMEOVER,
	TILE_FRM_TL,
	TILE_FRM_TR,
	TILE_FRM_BL,
	TILE_FRM_BR,
	TILE_FRM_LTEE,
	TILE_FRM_RTEE,
	TILE_FRM_THLINE,
	TILE_FRM_MHLINE,
	TILE_FRM_BHLINE,
	TILE_FRM_LVLINE,
	TILE_FRM_RVLINE,
	TILE_X,
	TILE_BLOCK,

	TILE_FONT_START = 32
};

#define FIRST_BLOCK_PAL		8

enum {
	PAL_GREY,
	PAL_INV,
	PAL_SCORE,
	PAL_SCOREHL,
	PAL_SCOREHL_NUM,
	PAL_VKEYB,
	PAL_VKEYB_INV,
	PAL_VKEYB_HL,

	PAL_BLKL = FIRST_BLOCK_PAL,
	PAL_BLKJ,
	PAL_BLKI,
	PAL_BLKO,
	PAL_BLKZ,
	PAL_BLKS,
	PAL_BLKT
};


int tile_block_start, tile_font_start, tile_scorescr_start, tile_namescr_start;

void setup_tileset(void *chrmem);

#endif	/* TILESET_H_ */
