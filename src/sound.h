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
#ifndef SOUND_H_
#define SOUND_H_

#define DS_ONCE		0
#define DS_LOOP		1
#define DS_MONO		0
#define DS_STEREO	2

void init_sound(void);

void play_dsound(void *rdata, void *ldata, unsigned long offs, unsigned long len,
		unsigned short rate, unsigned short mode);
void stop_dsound(void);

void pause_dsound(void);
void resume_dsound(void);

void snd_stick(void);
void snd_shift(void);
void snd_rot(void);

void snd_test(void);

#endif	/* SOUND_H_ */
