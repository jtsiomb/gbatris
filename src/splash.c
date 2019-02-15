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
#include <string.h>
#include "gbaregs.h"
#include "timer.h"

#define LOGO_SIZE	(240 * 160 * 2)
#define MSGLOGO_TIME	2000

extern unsigned char msglogo_pixels[];
extern unsigned char gbatris_pixels[];

void splash_screen(void)
{
	unsigned long start;
	void *fbptr = (void*)VRAM_LFB_FB0_ADDR;

	/* mode 3 (240x160 16bpp) */
	REG_DISPCNT = 3 | DISPCNT_BG2;

	memcpy(fbptr, msglogo_pixels, LOGO_SIZE);
	start = timer_msec;

	while(timer_msec - start < MSGLOGO_TIME);

	while(REG_VCOUNT < 160);
	memcpy(fbptr, gbatris_pixels, LOGO_SIZE);

	while(REG_KEYINPUT & KEY_START);
	while((REG_KEYINPUT & KEY_START) == 0);
}
