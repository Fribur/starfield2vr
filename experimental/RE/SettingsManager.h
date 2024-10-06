/*
#pragma once
#include "RE/Starfield.h"

namespace RE {
    struct alignas(1) GlobalSettings {
    public:
        int32_t N00000100; //0x0000
        int32_t N00000127; //0x0004
        int32_t leftX; //0x0008
        int32_t leftY; //0x000C
        int32_t displayAvailableW; //0x0010
        int32_t displayAvailableH; //0x0014
        int32_t N00000103; //0x0018
        int32_t N00000133; //0x001C
        int32_t displayWidth; //0x0020
        int32_t displayHeight; //0x0024
        int32_t N00000105; //0x0028
        int32_t N00000137; //0x002C
        int32_t windowH; //0x0030
        int32_t windowW; //0x0034
        uint32_t letterBoxHeight;
        uint32_t letterBoxWidth;
        uint8_t pad68[32];
        uint64_t features;
        uint8_t trainling[1024];

        static GlobalSettings* Get() {
            REL::Relocation<GlobalSettings**> instance{ REL::ID(878340) };
            return *instance;
        }
    };

    void SetAspectRatio(int width, int height) {
        REL::Relocation<float*> globalAspect{ REL::ID(873288) };
        *globalAspect = ((float)width)/((float)height);
    }
    static_assert(offsetof(GlobalSettings, letterBoxHeight) == 0x38);
}
*/
