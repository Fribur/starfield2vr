#include "CreationEngineHook.h"
#include "D3D12Hook.hpp"
//#include "RE/N/NiRect.h"
//#include "REL/Offset.h"
//#include "framework.h"
#include "polyhook2/Detour/x64Detour.hpp"
#include <CreationEngine/CreationEngineCameraManager.h>
#include <CreationEngine/CreationEngineRendererModule.h>
#include <CreationEngine/TAADebugView.h>
#include <CreationEngine/UpscalerAfrNvidiaModule.h>
#include <algorithm>
#include <map>
#include <mods/VR.hpp>

// void detourUpdateProjectionMatrix(RE::NiPoint4* out,RE::NiFrustum *frustum, char orpho, RE::NiTransform *transform) {
//     g_creationEngineHook->onUpdateProjectionMatrix(out, frustum, orpho, transform);
// }
//
// typedef void (*GraphUpdateFunc)(RE::IAnimationGraphManagerHolder*, RE::BSAnimationUpdateData*, void*);
// GraphUpdateFunc OriginalGraphUpdate;
// void UpdateGraph(RE::IAnimationGraphManagerHolder* a_graphHolder, RE::BSAnimationUpdateData* a_updateData, void* a_graph)
//{
//     OriginalGraphUpdate(a_graphHolder, a_updateData, a_graph);
// }
/*

typedef float* (*OriginalProjType)(RE::NiPoint3A* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, float* out);

OriginalProjType originalCalcMVP;
// SafetyHookInline g_projMatrixH{};
std::unique_ptr<PLH::x64Detour> ExecuteCommandDetour = nullptr;

// std::unique_ptr<FunctionHook> originalFH;

float* updateProjectionMatrix2(RE::NiPoint3A* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, float* out)
{
    // VRFramework::get_callback_manager()->onUpdateProjectionMatrix(translate, rotate, pFrustum, orpho, out);
    return originalCalcMVP(translate, rotate, pFrustum, orpho, out);
}

typedef void (*SetFrustumType)(RE::NiCamera* camera, RE::NiFrustum* frustum);
SetFrustumType                  originalCallSetNiFrustum;
std::unique_ptr<PLH::x64Detour> onSetNimFrustumInternalDetour = nullptr;

typedef void (*SetFrustumType2)(RE::NiCamera* a1, float fov, char requireLodAdjust);
SetFrustumType2                 originalCallSetNiFrustum2;
std::unique_ptr<PLH::x64Detour> onSetNimFrustumInternalDetour2 = nullptr;

void onSetNimFrustumInternal2(RE::NiCamera* camera, float fov, char requireLodAdjust)
{
    return originalCallSetNiFrustum2(camera, fov, requireLodAdjust);
}

std::unique_ptr<PLH::x64Detour> updateWorldHook = nullptr;
typedef void (*UpdateWorldType)(RE::NiAVObject* niObject, RE::NiUpdateData* niUpdateData);
UpdateWorldType                      onUpdateWorldOriginal;
std::map<uint64_t, RE::NiTransform*> prePreviousWorld;

void onUpdateWorld(RE::NiAVObject* niObject, RE::NiUpdateData* niUpdateData)
{
    onUpdateWorldOriginal(niObject, niUpdateData);
}

std::unique_ptr<PLH::x64Detour> updateRenderGraphHook = nullptr;
typedef uint8_t (*UpdateRenderGraphType)(RE::CreationRendererPrivate::MainRenderGraph* rcx);
UpdateRenderGraphType onUpdateRenderGraphOriginal;

uint8_t onUpdateRenderGraph(RE::CreationRendererPrivate::MainRenderGraph* rcx)
{
//    g_creationEngineHook->m_multiCameraViewModule->Update(rcx->renderGraphID);
    */
/*auto shouldRecreateResources = 0;
    if (rcx->renderGraphID == RE::Main::WorldRoot()->renderHandleID) {
        spdlog::info("onUpdateRenderGraph rcx->renderGraphID {}", rcx->renderGraphID);
        spdlog::info("pre onUpdateRenderGraph mv {}", rcx->multiViewDataArray.size());
        for (int i = 0; i < rcx->multiViewDataArray.size(); i++) {
            spdlog::info("onUpdateRenderGraph id {}", rcx->multiViewDataArray._data.local[i]);
        }
        if (rcx->multiViewDataArray.size() == 1) {
            shouldRecreateResources = 1;
        }
        spdlog::info("pre onUpdateRenderGraph sub graph names {}", rcx->subGraphNames.size());
        for (int i = 0; i < rcx->subGraphNames.size(); i++) {
            spdlog::info("pre onUpdateRenderGraph sub graph name {}", rcx->subGraphNames._data.local[i].c_str());
        }
    }*//*

    uint8_t result = onUpdateRenderGraphOriginal(rcx);
    */
/*if (rcx->renderGraphID == RE::Main::WorldRoot()->renderHandleID) {
        spdlog::info("post onUpdateRenderGraph mv {}", rcx->multiViewDataArray.size());
        for (int i = 0; i < rcx->multiViewDataArray.size(); i++) {
            spdlog::info("onUpdateRenderGraph id {}", rcx->multiViewDataArray._data.local[i]);
        }
//        if (shouldRecreateResources == 1 && rcx->multiViewDataArray.size() == 2) {
//            g_creationEngineHook->m_multiCameraViewModule->shouldRecreateResources.exchange(true);
//            if (g_creationEngineHook->m_multiCameraViewModule->shouldRecreateResources.exchange(false)) {
//                spdlog::info("onStartRenderPipeline RecompileAllRenderGraphParams");
//                 // RE::MultiCameraViewModule::RecompileAllRenderGraphParams();
//                spdlog::info("onStartRenderPipeline RecompileAllRenderGraphParams done");
//            }
//        }
        spdlog::info("post onUpdateRenderGraph sub graph names {}", rcx->subGraphNames.size());
        for (int i = 0; i < rcx->subGraphNames.size(); i++) {
            spdlog::info("post onUpdateRenderGraph sub graph name {}", rcx->subGraphNames._data.local[i].c_str());
        }
    }*//*

    return result;
}

std::unique_ptr<PLH::x64Detour> setMultiViewDataHook = nullptr;
typedef void (*SetMultiViewData)(uint32_t renderGraphId, RE::StorageTable::MultiCameraViewData* multiViewData);
SetMultiViewData originalSetMultiViewData;

void onSetMultiViewData(uint32_t renderGraphId, RE::StorageTable::MultiCameraViewData* multiViewData)
{
//    if (renderGraphId == RE::Main::WorldRoot()->renderHandleID) {
//        g_creationEngineHook->m_multiCameraViewModule->Init();
//        auto heap = multiViewData->multiViewDataArray._data.heap;
//        if (multiViewData->multiViewDataArray._local) {
//            heap = multiViewData->multiViewDataArray._data.local;
//        }
//        if (multiViewData->multiViewDataArray._size == 1) {
//            auto index  = multiViewData->multiViewDataArray._size++;
//            heap[index] = g_creationEngineHook->m_multiCameraViewModule->m_rightEyeScene->sceneHandleID;
//        }
//    }
    return originalSetMultiViewData(renderGraphId, multiViewData);
}

std::unique_ptr<PLH::x64Detour> updateViewportHook = nullptr;
typedef void (*SetViewport)(RE::NiCamera* camera, RE::NiRect<float>* viewport);
SetViewport originalCallSetViewport;

void onSetViewport(RE::NiCamera* camera, RE::NiRect<float>* viewport)
{
//    spdlog::info("onSetViewport [{}] {}: {} {} {} {}", camera->name.c_str(), fmt::ptr(camera), viewport->left, viewport->right, viewport->top, viewport->bottom);
    */
/*auto sceneGraphRoot = RE::Main::WorldRoot();
    if ((void*)camera == (void*)sceneGraphRoot->starfieldScene.pGalaxyCamera || (void*)camera == (void*)sceneGraphRoot->worldCamera) {
        viewport->right = 0.5f * viewport->right;
        spdlog::info("overriding viewport right to {}", viewport->right);

    }*//*

    return originalCallSetViewport(camera, viewport);
}

std::unique_ptr<PLH::x64Detour> updateViewport2Hook = nullptr;
typedef void (*SetViewport2)(RE::NiCamera* camera, RE::NiRect<float>* viewport);
SetViewport2 originalSetCameraScissors;

void onSetCameraScissors(RE::NiCamera* camera, RE::NiRect<float>* viewport)
{
//    spdlog::info("onSetCameraScissors [{}] {}: {} {} {} {}", camera->name.c_str(), fmt::ptr(camera), viewport->left, viewport->right, viewport->top, viewport->bottom);

    */
/* auto sceneGraphRoot = RE::Main::WorldRoot();

    if ((void*)camera == (void*)sceneGraphRoot->starfieldScene.pGalaxyCamera || (void*)camera == (void*)sceneGraphRoot->worldCamera) {
        viewport->right = 0.5f * viewport->right;
        spdlog::info("overriding scissors right to {}", viewport->right);
    }*//*

    return originalSetCameraScissors(camera, viewport);
}

std::unique_ptr<PLH::x64Detour> setViewAndScissorHook = nullptr;
typedef void (*SetViewAndScissors)(uintptr_t d3dContextHolder, uintptr_t rdx, RE::NiRect<float>* viewport, RE::NiRect<float>* scissors, float minDepth, float maxDepth);
SetViewAndScissors originalSetViewAndScissors;

void onSetViewAndScissors(uintptr_t d3dContextHolder, uintptr_t w_h, RE::NiRect<float>* viewport, RE::NiRect<float>* scissors, float minDepth, float maxDepth)
{
    return originalSetViewAndScissors(d3dContextHolder, w_h, viewport, scissors, minDepth, maxDepth);
}

std::unique_ptr<PLH::x64Detour> getCommnadListContextHolderHook = nullptr;
typedef uint64_t* (*GetCommandListContextHolder)(uint64_t rcx);
GetCommandListContextHolder originalGetCommandListContextHolder;

uint64_t* onGetCommandListContextHolder(uint64_t rcx)
{
    uint64_t* result =  originalGetCommandListContextHolder(rcx);
    */
/*if (rcx == 0) {
        return result;
    }
    auto renderGraphPointer = reinterpret_cast<uint64_t*>(rcx + 0x158);
    if(renderGraphPointer == nullptr || IsBadReadPtr(renderGraphPointer, sizeof(uint64_t)) || *renderGraphPointer == 0) {
        return result;
    }
    uint32_t* sceneHandleId = reinterpret_cast<uint32_t*>(*renderGraphPointer + 0xD28);
    //TODO check vtable addr to properly cast;
    if (g_creationEngineHook->m_multiCameraViewModule->initialized) {
        if(!IsBadReadPtr(sceneHandleId, 4) && *sceneHandleId == g_creationEngineHook->m_multiCameraViewModule->m_rightEyeScene->sceneHandleID) {
            spdlog::info("onGetCommandListContextHolder hit sceneHandleId {}", *sceneHandleId);
            g_creationEngineHook->m_multiCameraViewModule->commandContextHolderAddr = result;
        }
    }*//*

    return result;
}

std::unique_ptr<PLH::x64Detour> registerAsRendererCameraHook = nullptr;
typedef void (*RegisterAsRendererCamera)(RE::NiCamera* camera);
RegisterAsRendererCamera originalRegisterAsRendererCamera;

void onRegisterAsRendererCamera(RE::NiCamera* camera)
{
    originalRegisterAsRendererCamera(camera);
}

std::unique_ptr<PLH::x64Detour> cameraHandleRegisterHook = nullptr;
typedef void (*CameraHandleRegister)(RE::StorageTable::CameraHandle* cameraHandle, uint32_t* idOut);
CameraHandleRegister originalCameraHandleRegister;

void onCameraHandleRegister(RE::StorageTable::CameraHandle* cameraHandle, uint32_t* idOut)
{
    originalCameraHandleRegister(cameraHandle, idOut);
//    ids[idIndex++] = *idOut;
}

std::unique_ptr<PLH::x64Detour> calcFrustumFunctionHook = nullptr;
typedef void (*CalcFrustum)(RE::NiCamera* camera, float fov, float aspectRatio, float nearDist, float farDist, char lodAdjust);
CalcFrustum originalCalcFrustum;

void onCalcFrustum(RE::NiCamera* camera, float fov, float aspectRatio, float nearDist, float farDist, char lodAdjust)
{
    */
/*if(camera->name != "RightEyeCamera") {
        RE::NiRect<float> viewport = {0.0, 0.5, 1.0, 0.0};
        camera->SetViewport(&viewport);
        camera->SetScissors(&viewport);
    }*//*

//    spdlog::info("onCalcFrustum hit[{}:{}] {} {} {} {} {}", camera->name.c_str(), fmt::ptr(camera), fov, aspectRatio, nearDist, farDist, lodAdjust);
    return originalCalcFrustum(camera, fov, aspectRatio, nearDist, farDist, lodAdjust);
}


std::unique_ptr<PLH::x64Detour> setCommandQueueViewportHook = nullptr;
typedef void (*SetCommandQueueViewport)(uint64_t* commandListContextHolder, float x, float y, float w, float h, float minDepth, float maxDepth);
SetCommandQueueViewport originalCommandQueueViewport;

void onCommandQueueViewport(uint64_t* commandListContextHolder, float x, float y, float w, float h, float minDepth, float maxDepth)
{
    */
/*if(g_creationEngineHook->m_multiCameraViewModule->initialized && commandListContextHolder == g_creationEngineHook->m_multiCameraViewModule->commandContextHolderAddr) {
        spdlog::info("onCommandQueueViewport hit {} {} {} {}",x, y, w, h);
        return originalCommandQueueViewport(commandListContextHolder, x+w, y, w, h, minDepth, maxDepth);
    }*//*

    return originalCommandQueueViewport(commandListContextHolder, x, y, w, h, minDepth, maxDepth);
}

std::unique_ptr<PLH::x64Detour> setCommandQueueScissorsHook = nullptr;
typedef void (*SetCommandQueueScissors)(uint64_t* commandListContextHolder,  uint32_t x, uint32_t y, uint32_t w, uint32_t h);
SetCommandQueueScissors originalCommandQueueScissors;

void onCommandQueueScissors(uint64_t* commandListContextHolder, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    */
/*if(g_creationEngineHook->m_multiCameraViewModule->initialized && commandListContextHolder == g_creationEngineHook->m_multiCameraViewModule->commandContextHolderAddr) {
        spdlog::info("onCommandQueueScissors hit {} {} {} {}", x, y, w, h);
        return originalCommandQueueScissors(commandListContextHolder, x+w, y, w, h);
    }*//*

    return originalCommandQueueScissors(commandListContextHolder, x, y, w, h);
}

std::unique_ptr<PLH::x64Detour> getRenderGraphViewportMetatdataHook = nullptr;
typedef __int64 (*ScaleformSetScissors)(RE::CreationRendererPrivate::RenderGraph* renderGraph, __int64 rdx);
ScaleformSetScissors originalGetRenderGraphViewportMetatdata;

__int64  onGetRenderGraphViewportMetatdata(RE::CreationRendererPrivate::RenderGraph* renderGraph, __int64 rdx)
{
    auto data =  originalGetRenderGraphViewportMetatdata(renderGraph, rdx);
//    auto vport = reinterpret_cast<float*>(data);
//    spdlog::info("onScaleformSetScissors hit rgid[{}] {} {} {:x}", renderGraph->renderGraphID, fmt::ptr(renderGraph), renderGraph->name, data);
//    spdlog::info("onScaleformSetScissors hit rgid[{}] ptr{} {:x}", renderGraph->renderGraphID, fmt::ptr(renderGraph), data);
    return data;
}
*/

//std::unique_ptr<D3D12Hook> g_d3d12_hook{};

CreationEngineHook::CreationEngineHook()
{
//    m_multiCameraViewModule = new RE::MultiCameraViewModule();

//    REL::Relocation<uintptr_t> hookLoc3{ REL::ID(210405) };
//    onSetNimFrustumInternalDetour2 = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(hookLoc3.address()), reinterpret_cast<uint64_t>(&onSetNimFrustumInternal2),
//                                                                      reinterpret_cast<uint64_t*>(&originalCallSetNiFrustum2));
//    onSetNimFrustumInternalDetour2->hook();

//    REL::Relocation<uintptr_t> pWroldFov{ REL::ID(1171750) };
//
//    auto worldFov = reinterpret_cast<float*>(pWroldFov.address());
//    *worldFov     = 97.0f;

//    REL::Relocation<uintptr_t> update_world{ REL::ID(210239) };
//    updateWorldHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(update_world.address()), reinterpret_cast<uint64_t>(&onUpdateWorld),
//                                                       reinterpret_cast<uint64_t*>(&onUpdateWorldOriginal));
//    updateWorldHook->hook();

//    REL::Relocation<uintptr_t> updateRenderGraph{ REL::ID(208563) };
//    updateRenderGraphHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(updateRenderGraph.address()), reinterpret_cast<uint64_t>(&onUpdateRenderGraph),
//                                                             reinterpret_cast<uint64_t*>(&onUpdateRenderGraphOriginal));
//    updateRenderGraphHook->hook();

//    REL::Relocation<uintptr_t> setViewAndScissorAddr{ REL::ID(204137) };
//    setViewAndScissorHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(setViewAndScissorAddr.address()), reinterpret_cast<uint64_t>(&onSetViewAndScissors),
//                                                             reinterpret_cast<uint64_t*>(&originalSetViewAndScissors));
//    setViewAndScissorHook->hook();

//    REL::Relocation<uintptr_t> registerAsRendererCameraFuncAddr{ RE::ID::NiCamera::registerAsRendererCamera };
//    registerAsRendererCameraHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(registerAsRendererCameraFuncAddr.address()), reinterpret_cast<uint64_t>(&onRegisterAsRendererCamera),
//                                                                    reinterpret_cast<uint64_t*>(&originalRegisterAsRendererCamera));
//    registerAsRendererCameraHook->hook();

//    REL::Relocation<uintptr_t> calcFrustumFuncAddr{ RE::ID::NiCamera::CalcFrustum};
//    calcFrustumFunctionHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(calcFrustumFuncAddr.address()), reinterpret_cast<uint64_t>(&onCalcFrustum),
//                                                               reinterpret_cast<uint64_t*>(&originalCalcFrustum));
//    calcFrustumFunctionHook->hook();

//    REL::Relocation<uintptr_t> cameraHandleRegisterAddr{ REL::ID(94969) };
//    cameraHandleRegisterHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(cameraHandleRegisterAddr.address()), reinterpret_cast<uint64_t>(&onCameraHandleRegister),
//                                                                reinterpret_cast<uint64_t*>(&originalCameraHandleRegister));
//    cameraHandleRegisterHook->hook();

//    REL::Relocation<uintptr_t> scaleformMovieSetViewportFuncAddr{ REL::ID(50603) };
//    scaleformSetViewPortFuncHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(scaleformMovieSetViewportFuncAddr.address()), reinterpret_cast<uint64_t>(&onScaleformSetViewPort),
//                                                                    reinterpret_cast<uint64_t*>(&originalScaleformSetViewPort));
//    scaleformSetViewPortFuncHook->hook();


//    REL::Relocation<uintptr_t> GetRenderGraphViewportMetatdataFuncAddr{ REL::ID(206173) };
//    getRenderGraphViewportMetatdataHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(GetRenderGraphViewportMetatdataFuncAddr.address()), reinterpret_cast<uint64_t>(&onGetRenderGraphViewportMetatdata),
//                                                                    reinterpret_cast<uint64_t*>(&originalGetRenderGraphViewportMetatdata));
//    getRenderGraphViewportMetatdataHook->hook();

    /*
    REL::Relocation<uintptr_t> getCommandListContextHolder{ REL::ID(206429) };
    getCommnadListContextHolderHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(getCommandListContextHolder.address()), reinterpret_cast<uint64_t>(&onGetCommandListContextHolder),
                                                                      reinterpret_cast<uint64_t*>(&originalGetCommandListContextHolder));
    getCommnadListContextHolderHook->hook();*/

//    REL::Relocation<uintptr_t> commandQueueSetViewportFuncAddr{ REL::ID(204425) };
//    setCommandQueueViewportHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(commandQueueSetViewportFuncAddr.address()), reinterpret_cast<uint64_t>(&onCommandQueueViewport),
//                                                                      reinterpret_cast<uint64_t*>(&originalCommandQueueViewport));
//    setCommandQueueViewportHook->hook();

//    REL::Relocation<uintptr_t> commandQueueSetScissorsFuncAddr{ REL::ID(204421) };
//    setCommandQueueScissorsHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(commandQueueSetScissorsFuncAddr.address()), reinterpret_cast<uint64_t>(&onCommandQueueScissors),
//                                                                      reinterpret_cast<uint64_t*>(&originalCommandQueueScissors));
//    setCommandQueueScissorsHook->hook();

    CreationEngineCameraManager::Get()->InstallHooks();
    CreationEngineRendererModule::Get()->InstallHooks();
    UpscalerAfrNvidiaModule::Get();
//    TAADebugView::Get();

    /*
    REL::Relocation<uintptr_t> getScreenBoundsFromViewportFuncAddr{ REL::ID(203281) };
    getScreenBoundsFromViewportFuncHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(getScreenBoundsFromViewportFuncAddr.address()), reinterpret_cast<uint64_t>(&onGetScreenBoundsFromViewport),
                                                                           reinterpret_cast<uint64_t*>(&originalGetScreenBoundsFromViewport));
    getScreenBoundsFromViewportFuncHook->hook();
     */
//
//    REL::Relocation<uintptr_t> setMultiViewDataFuncAddr{ REL::ID(201997) };
//    setMultiViewDataHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(setMultiViewDataFuncAddr.address()), reinterpret_cast<uint64_t>(&onSetMultiViewData),
//                                                            reinterpret_cast<uint64_t*>(&originalSetMultiViewData));
//    setMultiViewDataHook->hook();
}

void CreationEngineHook::on_frame_d3d12()
{
    // spdlog::info("on_frame_d3d12");
}

CreationEngineHook::~CreationEngineHook()
{
    // g_d3d12_hook->unhook();
    delete m_multiCameraViewModule;
}

CreationEngineHook* CreationEngineHook::Get()
{
    static auto instance(new CreationEngineHook);
    return instance;
}
