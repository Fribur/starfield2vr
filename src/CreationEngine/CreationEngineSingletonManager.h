#pragma once
#include "RE/M/Main.h"
#include <RE/P/PlayerCharacter.h>
#include <RE/T/TESObjectREFR.h>
#include <SFVR.hpp>
#include <utility/Scan.hpp>

namespace RE {
    class PlayerCamera;
    namespace StorageTable
    {
        class RenderGraphHandle;
    }
}
class CreationEngineSingletonManager
{
public:
    static RE::Main::SceneGraphRoot* GetSceneGraphRoot();
    static RE::PlayerCamera* GetPlayerCameraSingleton();
    static RE::StorageTable::RenderGraphHandle* GetRenderGraphHandle();
    static RE::PlayerCharacter* GetPlayerRef();

private:
    CreationEngineSingletonManager() = default;
    ~CreationEngineSingletonManager() = default;
};
