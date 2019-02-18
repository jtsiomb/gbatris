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
#include "sound.h"
#include "gbaregs.h"
#include "debug.h"

#define RAMP_STEPS(x)		(((x) & 7) << 8)
#define RAMP_INC			0x0800
#define RAMP_START(x)		((x) << 12)

#define INIT				0x8000
#define PLAY_ONCE			0x4000
#define SHIFT_CLK(x)		((x) << 4)
#define BITS7				0x0008
#define F_X2				0
#define F_X1				1
#define F_DIV2				2
#define F_DIV3				3
#define F_DIV4				4
#define F_DIV5				5
#define F_DIV6				6
#define F_DIV7				7

#define SND1		0x01
#define SND2		0x02
#define SND3		0x04
#define SND4		0x08
#define SND_MASTER_EN	0x80

#define SNDVOL_R(x)		(x)
#define SNDVOL_L(x)		((x) << 4)
#define SNDEN_R(x)		((x) << 8)
#define SNDEN_L(x)		((x) << 12)

#define SYNTH_RATIO_QUARTER	0
#define SYNTH_RATIO_HALF	1
#define SYNTH_RATIO_FULL	2

void snd_stick(void)
{
}

void snd_test(void)
{
	print_vba("sound test\n");
	REG_SOUNDCNT_X = SND_MASTER_EN;
	REG_SOUNDCNT_L = SNDEN_L(SND4) | SNDEN_R(SND4) | SNDVOL_L(7) | SNDVOL_R(7);
	REG_SOUNDCNT_H = SYNTH_RATIO_FULL;

	REG_SOUND4CNT_L = RAMP_START(7) | RAMP_STEPS(7) | 5;
	REG_SOUND4CNT_H = INIT | PLAY_ONCE | F_DIV3;
}
