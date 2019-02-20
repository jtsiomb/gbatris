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
#ifndef GAME_H_
#define GAME_H_

#define SCR_WIDTH	240
#define SCR_HEIGHT	160
#define VIRT_WIDTH	256
#define VIRT_HEIGHT	256

#define SCR_ROWS	(SCR_HEIGHT / 8)
#define SCR_COLS	(SCR_WIDTH / 8)
#define VIRT_ROWS	(VIRT_HEIGHT / 8)
#define VIRT_COLS	(VIRT_WIDTH / 8)

uint16_t *scrmem, *chrmem;
uint16_t *chrmem_top;

long tick_interval;
int music;


int init_game(void);
void cleanup_game(void);

long update(long msec);
void game_input(int c);

#endif	/* GAME_H_ */
