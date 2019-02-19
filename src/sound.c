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
#include "timer.h"
#include "debug.h"

/* SOUNDxCNT_L bits */
#define DUTY_12				0
#define DUTY_25				0x0040
#define DUTY_50				0x0080
#define DUTY_75				0x00c0

#define RAMP_STEP_DUR(x)	(((x) & 7) << 8)
#define RAMP_INC			0x0800
#define RAMP_START(x)		((x) << 12)

#define WAVE_64SAMP			0x20
#define WAVE_BANK(x)		(((x) & 1) << 6)
#define WAVE_EN				0x80

#define SWEEP(x)			(((x) & 7) << 4)
#define SWEEP_INC			0
#define SWEEP_DEC			0x08

/* SOUNDxCNT_H bits */
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
#define WAVE_LEVEL_FULL		0x2000
#define WAVE_LEVEL_HALF		0x4000
#define WAVE_LEVEL_QUARTER	0x6000

/* SOUNDCNT_L bits */
#define SNDVOL_R(x)		((x) & 7)
#define SNDVOL_L(x)		(((x) & 7) << 4)
#define SNDVOL(x)		(SNDVOL_R(x) | SNDVOL_L(x))
#define SNDEN_R(x)		((x) << 8)
#define SNDEN_L(x)		((x) << 12)
#define SNDEN(x)		(SNDEN_R(x) | SNDEN_L(x))

#define SND1		0x01
#define SND2		0x02
#define SND3		0x04
#define SND4		0x08

/* SOUNDCNT_H bits */
#define SYNTH_RATIO_QUARTER	0
#define SYNTH_RATIO_HALF	1
#define SYNTH_RATIO_FULL	2

/* SOUNDCNT_X bits */
#define SND_MASTER_EN	0x80

#define TOK_FREQ	800
#define TOK_DUTY	DUTY_50

void init_sound(void)
{
	REG_SOUNDCNT_X = SND_MASTER_EN;
	REG_SOUNDCNT_H = SYNTH_RATIO_FULL;
}

void snd_stick(void)
{
	REG_SOUNDCNT_L = SNDEN(SND2) | SNDEN(SND4) | SNDVOL(7);
	REG_SOUND2CNT_L = RAMP_START(10) | RAMP_STEP_DUR(1) | TOK_DUTY | 25;
	REG_SOUND2CNT_H = INIT | PLAY_ONCE | TOK_FREQ;
	/* also add some noise */
	REG_SOUND4CNT_L = RAMP_START(8) | RAMP_STEP_DUR(1) | 25;
	REG_SOUND4CNT_H = INIT | PLAY_ONCE | F_DIV3 | SHIFT_CLK(4);
}

#define SHIFT_DUTY	DUTY_50
#define SHIFT_FREQ	1600
void snd_shift(void)
{
	REG_SOUNDCNT_L = SNDEN(SND2) | SNDVOL(2);
	REG_SOUND2CNT_L = RAMP_START(12) | RAMP_STEP_DUR(1) | SHIFT_DUTY | 25;
	REG_SOUND2CNT_H = INIT | PLAY_ONCE | SHIFT_FREQ;
}

#define ROT_DUTY	DUTY_50
#define ROT_FREQ	1600
void snd_rot(void)
{
	REG_SOUNDCNT_L = SNDEN(SND1) | SNDVOL(3);
	REG_SOUND1CNT_L = SWEEP(5) | SWEEP_INC | 4;
	REG_SOUND1CNT_H = RAMP_START(15) | RAMP_STEP_DUR(1) | ROT_DUTY | 5;
	REG_SOUND1CNT_X = INIT | PLAY_ONCE | ROT_FREQ;
	udelay(4);
	REG_SOUND1CNT_X = INIT | PLAY_ONCE | ROT_FREQ;
}

void snd_test(void)
{
	static const unsigned char wave[] = {
		0x02, 0x46, 0x8a, 0xce,
		0x02, 0x46, 0x8a, 0xce,
		0x02, 0x46, 0x8a, 0xce,
		0x02, 0x46, 0x8a, 0xce
	};
	int i;
	uint16_t *dptr, *sptr = (uint16_t*)wave;

	REG_SOUNDCNT_L = SNDEN(SND3) | SNDVOL(7);
	REG_SOUND3CNT_L = WAVE_BANK(1);
	dptr = (uint16_t*)WAVE_RAM_PTR;
	for(i=0; i<8; i++) {
		*dptr++ = *sptr++;
	}
	REG_SOUND3CNT_L = WAVE_EN | WAVE_BANK(0);
	REG_SOUND3CNT_H = WAVE_LEVEL_FULL;
	REG_SOUND3CNT_X = INIT | TOK_FREQ;

	delay(80);
	for(i=0; i<7; i++) {
		REG_SOUNDCNT_L = SNDEN(SND3) | SNDVOL(6 - i);
		delay(20);
	}
	REG_SOUNDCNT_L = 0;

	/*
	REG_SOUNDCNT_L = SNDEN(SND2) | SNDVOL(7);

	REG_SOUND2CNT_L = RAMP_START(15) | TOK_DUTY;
	REG_SOUND2CNT_H = INIT | TOK_FREQ;
	delay(100);
	REG_SOUND2CNT_L = RAMP_START(14) | RAMP_STEP_DUR(1) | TOK_DUTY;
	REG_SOUND2CNT_H = INIT | TOK_FREQ;
	delay(1500);
	*/

	/*
	REG_SOUNDCNT_L = SNDEN(SND4) | SNDVOL(7);
	REG_SOUND4CNT_L = RAMP_START(5) | RAMP_STEP_DUR(5) | RAMP_INC | 15;
	REG_SOUND4CNT_H = INIT | PLAY_ONCE | F_DIV7 | SHIFT_CLK(4);
	delay(1500);
	*/

	REG_SOUNDCNT_L = 0;
}
