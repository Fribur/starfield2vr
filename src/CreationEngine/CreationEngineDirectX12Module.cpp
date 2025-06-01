#include "CreationEngineDirectX12Module.h"
#include <CreationEngine/memory/offsets.h>
#include <REL/Relocation.h>

void CreationEngineDirectX12Module::InstallHooks() {
//    static REL::Relocation<CreationEngine::Nvidia::DirectX12Module**> g_sl_directX12Module{ REL::ID(886000) };
//    static REL::Relocation<CreationEngine::Nvidia::DirectX12Module**> g_sl_directX12Module{ GameStore::MemoryOffsets::GlobalDirectX12Module() };
//    m_sl_directX12Module = g_sl_directX12Module.get();
//    static REL::Relocation<CreationEngine::DirectX12Module**> g_directX12Module{ REL::ID(885886) };
    static REL::Relocation<RE::CreationRendererPrivate::ServiceLocator**> g_directX12Module{ GameStore::MemoryOffsets::GlobalDirectX12Module2() };
    g_ServiceLocator = g_directX12Module.get();
}
