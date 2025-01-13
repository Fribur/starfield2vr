#pragma once

#include <RE/T/TESObjectREFR.h>

namespace RE {

    class PlayerCharacter : public TESObjectREFR
    {
    public:
        char pad0x80[0xE08];
        NiAVObject* crosshairHit; // EE0
    };

    static_assert(offsetof(PlayerCharacter, crosshairHit) == 0xEE0);
}