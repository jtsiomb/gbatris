#include <string.h>
#include "gbaregs.h"
#include "tileset.h"

#define SCR_WIDTH	240
#define SCR_HEIGHT	160
#define VIRT_WIDTH	256
#define VIRT_HEIGHT	256

#define SCR_ROWS	(SCR_HEIGHT / 8)
#define SCR_COLS	(SCR_WIDTH / 8)
#define VIRT_ROWS	(VIRT_HEIGHT / 8)
#define VIRT_COLS	(VIRT_WIDTH / 8)

#define SCR_BASE_BLOCK	0
#define CHR_BASE_BLOCK	2
static uint16_t *scrmem = (uint16_t*)VRAM_SCR_BLOCK_ADDR(SCR_BASE_BLOCK);
static uint16_t *chrmem = (uint16_t*)VRAM_CHR_BLOCK_ADDR(CHR_BASE_BLOCK);

int main(void)
{
	/* set mode 0 and enable BG0 */
	REG_DISPCNT = DISPCNT_BG0;
	/* BG0: scr/chr base blocks */
	REG_BG0CNT = BGCNT_CHR_BASE(CHR_BASE_BLOCK) | BGCNT_SCR_BASE(SCR_BASE_BLOCK);

	setup_tileset(chrmem);

	/* DBG fill the screen */
	memset(scrmem, 0, VIRT_ROWS * VIRT_COLS * 2);
	scrmem[10 * VIRT_COLS + 10] = 1 | BGTILE_PAL(1);
	scrmem[10 * VIRT_COLS + 12] = 1 | BGTILE_PAL(2);

	for(;;) {
	}
	return 0;
}
