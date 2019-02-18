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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scoredb.h"
#include "gbaregs.h"

#define MAGIC "GBATRIS1"
#define SCORE_OFFS	8

void read_sram(int sram_offs, void *buf, int size);
void write_sram(int sram_offs, void *buf, int size);


int load_scores(void)
{
	int i;
	char magic[8];

	read_sram(0, magic, 8);
	if(memcmp(magic, MAGIC, sizeof magic) != 0) {
		for(i=0; i<10; i++) {
			memcpy(scores[i].name, "--- ", NAME_SIZE + 1);
		}
		return -1;
	}

	read_sram(SCORE_OFFS, scores, sizeof scores);
	return 0;
}

void save_scores(void)
{
	write_sram(0, MAGIC, 8);
	write_sram(SCORE_OFFS, scores, sizeof scores);
}

void save_score(char *name, int score, int lines, int level)
{
	int i, rank = -1;

	for(i=0; i<10; i++) {
		if(scores[i].score < score) {
			rank = i;
			break;
		}
	}
	last_score_rank = rank;

	if(rank == -1) return;

	memmove(scores + rank + 1, scores + rank, (10 - rank) * sizeof(struct score_entry));

	if(strlen(name) > NAME_SIZE) {
		name[NAME_SIZE] = 0;
	}
	sprintf(scores[rank].name, "%4s", name);
	scores[rank].score = score;
	scores[rank].lines = lines;
	scores[rank].level = level;

	save_scores();
}
