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
#ifndef KEYB_H_
#define KEYB_H_

#include <stdint.h>

#define KEYPRESS(key)	((keystate & (key)) && (keydelta & (key)))
#define KEYRELEASE(key)	((keystate & (key)) == 0 && (keydelta & (key)))

uint16_t keystate, keydelta;

void key_repeat(int start, int rep, uint16_t mask);

void update_keyb(void);

#endif	/* KEYB_H_ */
