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
