#include <string.h>
#include "gbaregs.h"
#include "game.h"
#include "tileset.h"
#include "timer.h"
#include "intr.h"

#define SCR_BASE_BLOCK	0
#define CHR_BASE_BLOCK	2

static int handle_keys(unsigned long msec);
void splash_screen(void);

int main(void)
{
	unsigned long msec;
	uint16_t next_upd = 0;

	intr_init();
	reset_msec_timer();
	set_intr();

	splash_screen();

	/* set mode 0 and enable BG0 */
	REG_DISPCNT = DISPCNT_BG0;
	/* BG0: scr/chr base blocks */
	REG_BG0CNT = BGCNT_CHR_BASE(CHR_BASE_BLOCK) | BGCNT_SCR_BASE(SCR_BASE_BLOCK);

	scrmem = (uint16_t*)VRAM_SCR_BLOCK_ADDR(SCR_BASE_BLOCK);
	chrmem = (uint16_t*)VRAM_CHR_BLOCK_ADDR(CHR_BASE_BLOCK);

	setup_tileset(chrmem);
	init_game();

	for(;;) {
		while(REG_VCOUNT < 160);	/* wait vsync */

		msec = timer_msec;

		if(handle_keys(msec) || msec >= next_upd) {
			next_upd = update(timer_msec) + msec;
		}
	}
	return 0;
}


#define PRESS(key)	((keystate & (key)) && (keydelta & (key)))

#define REPEAT_START		500
#define REPEAT_INTERVAL		75

static int handle_keys(unsigned long msec)
{
	static uint16_t prevstate;
	static unsigned long first_press[4], last_inp[4];
	static const char input[] = {'d', 'a', '\n', 's'};

	int i, upd = 0;
	uint16_t keystate, keydelta;

	keystate = ~REG_KEYINPUT;
	keydelta = keystate ^ prevstate;
	prevstate = keystate;

	if(PRESS(KEY_A)) {
		game_input('w');
		upd = 1;
	}
	if(PRESS(KEY_B)) {
		game_input('\n');
		upd = 1;
	}
	if(PRESS(KEY_START)) {
		game_input('p');
		upd = 1;
	}
	if(PRESS(KEY_SELECT)) {
		game_input('\b');
		upd = 1;
	}

	for(i=0; i<4; i++) {
		uint16_t bit = KEY_RIGHT << i;

		if(keystate & bit) {
			if(keydelta & bit) {
				game_input(input[i]);
				upd = 1;
				first_press[i] = msec;
			} else {
				if(msec - first_press[i] >= REPEAT_START && msec - last_inp[i] >= REPEAT_INTERVAL) {
					game_input(input[i]);
					upd = 1;
					last_inp[i] = msec;
				}
			}
		}
	}

	return upd;
}
