#pragma once
#include <RE/N/NiAVObject.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include "memory/FunctionHook.h"

namespace CEDebug {
    struct RayCastQuery {
        char pad[0x30];
        glm::vec4 point30;
        glm::vec4 point40;
        glm::vec4 point50;
        glm::vec4 point60;
        glm::vec3 point70;
        glm::vec3 point80;

    };
}

class HavokModule
{
public:
    inline static std::shared_ptr<HavokModule>& Get()
    {
        static std::shared_ptr<HavokModule> instance{ std::make_shared<HavokModule>() };
        return instance;
    }
    HavokModule();
    ~HavokModule() = default;
private:
    void install_hooks();
    std::unique_ptr<FunctionHook> ni_pointer_set_hook{};
    std::unique_ptr<FunctionHook> m_havok_raycast_weapon_hook{};
    std::unique_ptr<FunctionHook> m_havok_raycast_weapon2_hook{};
    static uintptr_t onNiPointerSet(uintptr_t a_pointer, uintptr_t a_value);
    static RE::NiAVObject* onHavokRaycast(uintptr_t* testObjectCEll, CEDebug::RayCastQuery* rayCastQuery);
    static char onHavokRaycast2(uintptr_t* source, CEDebug::RayCastQuery* rayCastQuery);

};

