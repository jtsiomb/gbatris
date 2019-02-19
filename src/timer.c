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
#include "timer.h"

/* clock is 16.78MHz
 * - no prescale: 59.595ns
 * - prescale 64: 3.814us
 * - prescale 256: 15.256us
 * - prescale 1024: 61.025us
 */

static void timer_intr(void);


void reset_msec_timer(void)
{
	REG_TM0CNT_H &= ~TMCNT_EN;
	interrupt(INTR_TIMER0, timer_intr);
	timer_msec = 0;
	REG_TM0CNT_L = 65535 - 16779;
	REG_TM0CNT_H |= TMCNT_IE | TMCNT_EN;
	unmask(INTR_TIMER0);
}

void delay(unsigned long ms)
{
	unsigned long end = timer_msec + ms;
	while(timer_msec < end);
}

static void timer_intr(void)
{
	timer_msec++;
}
