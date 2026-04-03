#ifndef GBA_BIOS_FRLG_REVA_H
#define GBA_BIOS_FRLG_REVA_H

#include <mgba-util/common.h>

CXX_GUARD_START

struct GBA;

/* Handles SWI 0x48 — FireRed/LeafGreen RevA flash sector write.
 * r0 = sector number (u8, 0–13)
 * r1 = source data pointer (GBA address, 0x1000 bytes)
 * Returns 0 in r0 on success, 1 on error. */
void GBABiosFRLGRevAFlashWrite(struct GBA* gba);

CXX_GUARD_END

#endif
