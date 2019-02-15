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
#ifndef _TIMER_H_
#define _TIMER_H_

#include "gbaregs.h"

#define enable_timer(x) \
	do { REG_TMCNT_H(x) |= TMCNT_EN; } while(0)

#define disable_timer(x) \
	do { REG_TMCNT_H(x) &= ~TMCNT_EN; } while(0)

volatile unsigned long timer_msec;

void reset_msec_timer(void);

#endif	/* _TIMER_H_ */
