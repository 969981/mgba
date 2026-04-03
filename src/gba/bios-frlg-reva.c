#include <mgba/internal/gba/bios-frlg-reva.h>

#include <mgba/internal/arm/arm.h>
#include <mgba/internal/defines.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/memory.h>
#include <mgba/internal/gba/savedata.h>
#include <mgba/core/log.h>

mLOG_DECLARE_CATEGORY(GBA_BIOS);

void GBABiosFRLGRevAFlashWrite(struct GBA* gba) {
	struct ARMCore* cpu = gba->cpu;
	uint8_t sectorNum = (uint8_t) cpu->gprs[0];
	uint32_t srcAddr  = cpu->gprs[1];

	struct GBASavedata* savedata = &gba->memory.savedata;

	/* Only act on flash save types */
	if (savedata->type != GBA_SAVEDATA_FLASH512 &&
	    savedata->type != GBA_SAVEDATA_FLASH1M) {
		mLOG(GBA_BIOS, GAME_ERROR,
		     "SWI 0x48 (FRLG RevA flash write): unexpected savedata type %d", savedata->type);
		cpu->gprs[0] = 1; /* non-zero = error */
		return;
	}

	/* Each GBA flash sector is 0x1000 (4096) bytes */
	uint32_t offset = (uint32_t) sectorNum * 0x1000;
	size_t maxSize = (savedata->type == GBA_SAVEDATA_FLASH1M)
	                 ? GBA_SIZE_FLASH1M : GBA_SIZE_FLASH512;

	if (offset + 0x1000 > maxSize) {
		mLOG(GBA_BIOS, GAME_ERROR,
		     "SWI 0x48 (FRLG RevA flash write): sector %u out of range", sectorNum);
		cpu->gprs[0] = 1;
		return;
	}

	/* Copy 0x1000 bytes from GBA address space into the save buffer */
	enum mMemoryAccessSource oldAccess = cpu->memory.accessSource;
	cpu->memory.accessSource = mACCESS_SYSTEM;
	uint32_t i;
	for (i = 0; i < 0x1000; ++i) {
		savedata->data[offset + i] = cpu->memory.load8(cpu, srcAddr + i, 0);
	}
	cpu->memory.accessSource = oldAccess;

	savedata->dirty |= mSAVEDATA_DIRT_NEW;
	cpu->gprs[0] = 0; /* success */
}
