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
#include "intr.h"
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


static void dsound_intr(void);

static uint32_t ds_ldata_start, ds_rdata_start;
static uint32_t ds_ldata_next, ds_rdata_next;
static uint32_t ds_nsamples, ds_nsamples_left;
static uint16_t ds_mode, ds_rate;
static uint16_t ds_scnt;


void init_sound(void)
{
	REG_SOUNDCNT_X = SCNT_MASTER_EN;
	REG_SOUNDCNT_H = SCNT_SS_VOL_FULL;
}

void play_dsound(void *rdata, void *ldata, unsigned long offs, unsigned long len,
		unsigned short rate, unsigned short mode)
{
	unsigned long count;

	ds_mode = mode;
	ds_rate = rate;
	ds_nsamples = len;

	len -= offs;
	count = len > 65536 ? 65536 : len;
	ds_nsamples_left = len - count;

	ds_rdata_start = (uint32_t)rdata;
	ds_rdata_next = ds_rdata_start + count;

	ds_scnt = SCNT_SS_VOL_FULL;
	if(mode & DS_STEREO) {
		ds_ldata_start = (uint32_t)ldata;
		ds_ldata_next = ds_ldata_start + count;

		ds_scnt |= SCNT_DSA_VOL_HALF | SCNT_DSA_EN_R | SCNT_DSA_TIMER1 | SCNT_DSA_CLRFIFO;
		ds_scnt |= SCNT_DSB_VOL_HALF | SCNT_DSB_EN_L | SCNT_DSB_TIMER1 | SCNT_DSA_CLRFIFO;
	} else {
		ds_scnt |= SCNT_DSA_VOL_HALF | SCNT_DSA_EN_R | SCNT_DSA_EN_L | SCNT_DSA_TIMER1 | SCNT_DSA_CLRFIFO;
	}
	REG_SOUNDCNT_H = ds_scnt;

	/* program DMA */
	REG_DMA1SAD = ds_rdata_start + offs;
	REG_DMA1DAD = (uint32_t)FIFO_A_PTR;
	REG_DMA1CNT_H = DMACNT_EN | DMACNT_SOUND | DMACNT_32BIT | DMACNT_REPEAT;

	if(mode & DS_STEREO) {
		REG_DMA2SAD = ds_ldata_start + offs;
		REG_DMA2DAD = (uint32_t)FIFO_B_PTR;
		REG_DMA2CNT_H = DMACNT_EN | DMACNT_SOUND | DMACNT_32BIT | DMACNT_REPEAT;
	}

	mask(INTR_TIMER2);
	REG_TM2CNT_H = 0;
	REG_TM2CNT_L = 65536 - count;
	REG_TM2CNT_H = TMCNT_EN | TMCNT_IE | TMCNT_CASCADE;
	interrupt(INTR_TIMER2, dsound_intr);
	unmask(INTR_TIMER2);

	init_timer(1, rate, 0);
	enable_timer(1);
}

void stop_dsound(void)
{
	REG_SOUNDCNT_H = SCNT_DSA_CLRFIFO | SCNT_DSB_CLRFIFO | SCNT_SS_VOL_FULL;

	disable_timer(1);
	REG_DMA1CNT_H = 0;
	if(ds_mode & DS_STEREO) {
		REG_DMA2CNT_H = 0;
	}
}

static unsigned long pause_offs;

void pause_dsound(void)
{
	pause_offs = ds_nsamples - (ds_nsamples_left + (65536 - REG_TM2CNT_L));
	stop_dsound();
}

void resume_dsound(void)
{
	play_dsound((void*)ds_rdata_start, (void*)ds_ldata_start, pause_offs, ds_nsamples,
			ds_rate, ds_mode);
}

static void dsound_intr(void)
{
	unsigned long count;

	if(!ds_nsamples_left && !(ds_mode & DS_LOOP)) {
		stop_dsound();
		return;
	}

	if(!ds_nsamples_left) {
		/* loop */
		ds_nsamples_left = ds_nsamples;
		ds_rdata_next = ds_rdata_start;
		ds_ldata_next = ds_ldata_start;
	}

	count = ds_nsamples_left > 65536 ? 65536 : ds_nsamples_left;
	ds_nsamples_left -= count;

	/* reprogram DMA and write the first sample */
	REG_DMA1CNT_H = 0;
	REG_DMA1SAD = ds_rdata_next;
	REG_DMA1CNT_H = DMACNT_EN | DMACNT_SOUND | DMACNT_32BIT | DMACNT_REPEAT;
	*FIFO_A_PTR = *((unsigned char*)ds_rdata_next);
	ds_rdata_next += count;

	if(ds_mode & DS_STEREO) {
		REG_DMA2CNT_H = 0;
		REG_DMA2SAD = ds_ldata_next;
		REG_DMA2CNT_H = DMACNT_EN | DMACNT_SOUND | DMACNT_32BIT | DMACNT_REPEAT;
		*FIFO_B_PTR = *((unsigned char*)ds_ldata_next);

		ds_ldata_next += count;
	}

	REG_TM2CNT_H = 0;
	REG_TM2CNT_L = 65536 - count;
	REG_TM2CNT_H = TMCNT_EN | TMCNT_IE | TMCNT_CASCADE;
}

#define TOK_FREQ	800
#define TOK_DUTY	DUTY_50
void snd_stick(void)
{
	REG_SOUNDCNT_L = SCNT_SS_EN(SCNT_SS2) | SCNT_SS_EN(SCNT_SS4) | SCNT_SS_VOL(7);
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
	REG_SOUNDCNT_L = SCNT_SS_EN(SCNT_SS2) | SCNT_SS_VOL(2);
	REG_SOUND2CNT_L = RAMP_START(12) | RAMP_STEP_DUR(1) | SHIFT_DUTY | 25;
	REG_SOUND2CNT_H = INIT | PLAY_ONCE | SHIFT_FREQ;
}

#define ROT_DUTY	DUTY_50
#define ROT_FREQ	1600
void snd_rot(void)
{
	REG_SOUNDCNT_L = SCNT_SS_EN(SCNT_SS1) | SCNT_SS_VOL(3);
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

	REG_SOUNDCNT_L = SCNT_SS_EN(SCNT_SS3) | SCNT_SS_VOL(7);
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
		REG_SOUNDCNT_L = SCNT_SS_EN(SCNT_SS3) | SCNT_SS_VOL(6 - i);
		delay(20);
	}
	REG_SOUNDCNT_L = 0;
}
