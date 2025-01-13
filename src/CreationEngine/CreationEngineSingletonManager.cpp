#include "CreationEngineSingletonManager.h"
#include <CreationEngine/memory/offsets.h>
#include <REL/Relocation.h>

RE::Main::SceneGraphRoot* CreationEngineSingletonManager::GetSceneGraphRoot()
{
    static REL::Relocation<RE::Main::SceneGraphRoot**> singleton{ GameStore::MemoryOffsets::GlobalSceneGraphRoot()};
    return *singleton;
}

RE::StorageTable::RenderGraphHandle* CreationEngineSingletonManager::GetRenderGraphHandle()
{
    static REL::Relocation<RE::StorageTable::RenderGraphHandle**> singleton{ GameStore::MemoryOffsets::StorageTable::RenderGraphHandle::Singleton()};
    return *singleton;
}

RE::PlayerCamera* CreationEngineSingletonManager::GetPlayerCameraSingleton()
{
    static REL::Relocation<RE::PlayerCamera**> singleton{ GameStore::MemoryOffsets::PlayerCamera::Singleton()};
    return *singleton;
}

RE::PlayerCharacter* CreationEngineSingletonManager::GetPlayerRef()
{
    static REL::Relocation<RE::PlayerCharacter**> singleton{ GameStore::MemoryOffsets::GlobalPlayerRef()};
    return *singleton;
}
