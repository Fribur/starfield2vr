#pragma once

#include <RE/T/TESObjectREFR.h>

namespace RE {

    enum class BOOL_FLAGS {
        kIsInScene = 0x8,
    };

    struct MiddleHighProcessData {
        char pad0x0[0x518];
        uint32_t isInIronSights; // 08
    };

    class AIProcess {
    public:
        char pad0x0[0x8];
        MiddleHighProcessData*          middleHigh;            // 08
    };

    class PlayerCharacter : public TESObjectREFR
    {
    public:
        char pad0x80[0x20];
        uint32_t                                       actorState;             // 0F8
        uint32_t actorState2; // 0fC
        char pad0x90[0x108];
        uint32_t boolFlags; // 0x208
        uint32_t unk; // 0x20C
        char pad0x210[0x18];
        AIProcess*                                       currentProcess;           // 228
        uint8_t pad0x230[0xCB0];
        NiAVObject* crosshairHit; // EE0


        // not really sure how this works
        inline bool IsInScene() const noexcept { return (boolFlags & static_cast<uint32_t>(BOOL_FLAGS::kIsInScene)) != 0; }
        inline bool IsInIronSights() const noexcept {
            if(!currentProcess || !currentProcess->middleHigh) return false;
            return currentProcess->middleHigh->isInIronSights;
        }
        inline bool IsWeaponDrawn() const noexcept { return (actorState2 & 7) >= 3; }
        // probably for zero gravity not for ship
        inline bool IsInFlyingState() const noexcept { return (actorState >> 14) & 7; }
        inline bool Immovable() const noexcept { return (actorState2 >> 12) & 1; }
        inline bool ControlledByAI() const noexcept { return (actorState2 >> 11) & 1; }

    };

    static_assert(offsetof(PlayerCharacter, crosshairHit) == 0xEE0);
    static_assert(offsetof(PlayerCharacter, actorState) == 0xF8);
    static_assert(offsetof(PlayerCharacter, actorState2) == 0xFC);
    static_assert(offsetof(PlayerCharacter, boolFlags) == 0x208);
    static_assert(offsetof(PlayerCharacter, currentProcess) == 0x228);
}