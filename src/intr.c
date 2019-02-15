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
#include "intr.h"

#define MAX_INTR	14
static void (*intr_table[MAX_INTR])(void);

__attribute__ ((target("arm")))
static void intr_handler(void)
{
	int i;
	uint16_t iflags;

	clr_intr();
	iflags = REG_IF & 0x3fff;


	for(i=0; i<MAX_INTR; i++) {
		if((iflags & (1 << i)) && intr_table[i]) {
			intr_table[i]();
		}
	}

	REG_IF = iflags;	/* ack intr */
	set_intr();
}

void intr_init(void)
{
	INTR_VECTOR = (uint32_t)intr_handler;
}

void interrupt(int intr, void (*handler)(void))
{
	intr_table[intr] = handler;
}
