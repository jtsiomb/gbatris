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
#include "keyb.h"
#include "gbaregs.h"
#include "timer.h"

#define NUM_KEYS	10

static int rep_start, rep_rep;
static unsigned long first_press[16], last_press[16];
static uint16_t repmask;

void key_repeat(int start, int rep, uint16_t mask)
{
	rep_start = start;
	rep_rep = rep;
	repmask = mask;
}

void update_keyb(void)
{
	static uint16_t prevstate;
	int i;
	unsigned long msec = timer_msec;

	keystate = ~REG_KEYINPUT;
	keydelta = keystate ^ prevstate;
	prevstate = keystate;

	for(i=0; i<NUM_KEYS; i++) {
		uint16_t bit = 1 << i;
		if(!(bit & repmask)) {
			continue;
		}

		if(keystate & bit) {
			if(keydelta & bit) {
				first_press[i] = msec;
			} else {
				if(msec - first_press[i] >= rep_start && msec - last_press[i] >= rep_rep) {
					keydelta |= bit;
					last_press[i] = msec;
				}
			}
		}
	}
}
