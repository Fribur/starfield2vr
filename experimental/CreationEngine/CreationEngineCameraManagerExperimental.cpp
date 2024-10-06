////
//// Created by sergp on 6/23/2024.
////
//
//#include "CreationEngineCameraManager.h"
//#include "CreationEngineConstants.h"
//#include "CreationEngineGameModule.h"
//#include "CreationEngineRendererModule.h"
//#include <CreationEngine/RE2/offsets.h>
//#include <REL/Relocation.h>
//#include <cstdlib>
//#include <mods/VR.hpp>
//
//constexpr unsigned int djb2Hash(const char* str, int index = 0)
//{
//    return !str[index] ? 0x1505 : (djb2Hash(str, index + 1) * 0x21) ^ str[index];
//}
//
//constexpr unsigned int operator"" _DJB(const char str[], size_t size)
//{
//    return djb2Hash(str);
//}
//
//void onUpdateNiCameraDetour(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
//{
//    CreationEngineCameraManager::Get()->onUpdateNiCamera(a_camera, a_data);
//}
//
//void onPerformInputProcessingDetour(const RE::PlayerCamera* a_camera, const RE::InputEvent* a_queueHead)
//{
//    CreationEngineCameraManager::Get()->onPerformInputProcessing(a_camera, a_queueHead);
//}
//
//RE::BSEventNotifyControl onCameraCutProcessEventDetour(void* a_event, void* source)
//{
//    return CreationEngineCameraManager::Get()->onCameraCutEvent(a_event, source);
//}
//
//void onSetNimFrustumDetour(RE::NiCamera* camera, RE::NiFrustum* frustum)
//{
//    CreationEngineCameraManager::Get()->onSetNimFrustum(camera, frustum);
//}
//
//void onCalcNimFrustumDetour(RE::NiCamera* camera, float fov, float aspectRatio, float nearDist, float farDist, char lodAdjust)
//{
//    CreationEngineCameraManager::Get()->onCalcNiFrustum(camera, fov, aspectRatio, nearDist, farDist, lodAdjust);
//}
//
//void onScaleformSetViewPortDetour(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
//{
//    CreationEngineCameraManager::Get()->onScaleformSetViewPort(thisMovie, viewport);
//}
//
//void onScaleformMovieSetProjectionMatrix3DDetour(uintptr_t* thisMovie, Matrix4x4f& matrix)
//{
//    CreationEngineCameraManager::Get()->onScaleformMovieSetProjectionMatrix3D(thisMovie, matrix);
//}
//
//Matrix4x4f* onUpdateWorldToCamDetour(RE::NiPoint4* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, Matrix4x4f* out)
//{
//    return CreationEngineCameraManager::Get()->onUpdateWorldToCam(translate, rotate, pFrustum, orpho, out);
//}
//
//uintptr_t onUpdateCollisionMatrixDetour(Matrix6x4f* points, RE::NiFrustum* frustum, char orpho, Matrix4x4f* transform)
//{
//    return CreationEngineCameraManager::Get()->onUpdateCollisionMatrix(points, frustum, orpho, transform);
//}
//
//void onSetViewportDetour(RE::NiCamera* camera, CreationEngine::Viewport* viewport)
//{
//    CreationEngineCameraManager::Get()->onSetViewport(camera, viewport);
//}
//
//void onSetCameraScissorDetour(RE::NiCamera* camera, CreationEngine::Viewport* scissor)
//{
//    CreationEngineCameraManager::Get()->onSetCameraScissor(camera, scissor);
//}
//
//uint64_t onUpdateWorldDetour(RE::NiAVObject* niObject, RE::NiUpdateData* niUpdateData)
//{
//    return CreationEngineCameraManager::Get()->onUpdateWorld(niObject, niUpdateData);
//}
//
//bool isValidCamera(RE::NiCamera* pCamera)
//{
//    auto sceneGraphRoot = CreationEngineGameModule::GetSceneGraphRoot();
//    return pCamera == sceneGraphRoot->worldCamera || pCamera == sceneGraphRoot->starfieldScene.pStarFieldCamera || sceneGraphRoot->starfieldScene.pGalaxyCamera == pCamera;
//}
//
//void CreationEngineCameraManager::InstallHooks()
//{
////    REL::Relocation<uintptr_t> niCameraUpdateVFuncAddr{ REL::ID(210425) };
//    REL::Relocation<uintptr_t> niCameraUpdateVFuncAddr{ RE2::MemoryOffsets::NiCamera::UpdateWorldVfunc()};
//    m_onUpdateNiCameraHook = std::make_unique<PolyHook2FunctionHook>(niCameraUpdateVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onUpdateNiCameraDetour));
//    m_onUpdateNiCameraHook->create();
//
////    REL::Relocation<uintptr_t> niAvObjectUpdateVFuncAddr{ RE2::MemoryOffsets::NiAVObject::UpdateWorld() };
////    m_onUpdateWorldHook = std::make_unique<PolyHook2FunctionHook>(niAvObjectUpdateVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onUpdateWorldDetour));
////    m_onUpdateWorldHook->create();
//
//    //    REL::Relocation<uintptr_t> cameraCutProcessEventAddr{ REL::ID(129813) };
//    //    m_onCameraCutProcessEventHook = std::make_unique<PolyHook2FunctionHook>(cameraCutProcessEventAddr.address(), reinterpret_cast<uintptr_t>(&onCameraCutProcessEventDetour));
//    //    m_onCameraCutProcessEventHook->create();
////    REL::Relocation<uintptr_t> setNimFrustumVFuncAddr{ RE::ID::NiCamera::setFrustum };
//    REL::Relocation<uintptr_t> setNimFrustumVFuncAddr{ RE2::MemoryOffsets::NiCamera::SetFrustumVfunc() };
//    m_onSetNimFrustumHook = std::make_unique<PolyHook2FunctionHook>(setNimFrustumVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onSetNimFrustumDetour));
//    m_onSetNimFrustumHook->create();
//
////    REL::Relocation<uintptr_t> calcNimFrustumVFuncAddr{ REL::ID(RE::ID::NiCamera::CalcFrustum) };
//    REL::Relocation<uintptr_t> calcNimFrustumVFuncAddr{ RE2::MemoryOffsets::NiCamera::CalcFrustumVfunc() };
//    m_onCalcNimFrustumHook = std::make_unique<PolyHook2FunctionHook>(calcNimFrustumVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onCalcNimFrustumDetour));
//    m_onCalcNimFrustumHook->create();
//
////    REL::Relocation<uintptr_t> scaleformSetViewPortAddr{ REL::ID(50603) };
//    REL::Relocation<uintptr_t> scaleformSetViewPortAddr{ RE2::MemoryOffsets::Scaleform::Movie::SetViewportVFunc() };
//    m_onScaleformSetViewPortHook = std::make_unique<PolyHook2FunctionHook>(scaleformSetViewPortAddr.address(), reinterpret_cast<uintptr_t>(&onScaleformSetViewPortDetour));
//    m_onScaleformSetViewPortHook->create();
//
//    //    m_onSetViewportHook = std::make_unique<PolyHook2FunctionHook>(RE::ID::NiCamera::SetViewport.address(), reinterpret_cast<uintptr_t>(&onSetViewportDetour));
//    //    m_onSetViewportHook->create();
//    //    m_onSetCameraScissorHook = std::make_unique<PolyHook2FunctionHook>(RE::ID::NiCamera::SetScissors.address(), reinterpret_cast<uintptr_t>(&onSetCameraScissorDetour));
//    //    m_onSetCameraScissorHook->create();
//
//    //    REL::Relocation<uintptr_t> scaleformMovieSetProjectionMatrix3DVFuncAddr{ REL::ID(50722) };
//    //    m_onScaleformMovieSetProjectionMatrix3DHook = std::make_unique<PolyHook2FunctionHook>(scaleformMovieSetProjectionMatrix3DVFuncAddr.address(),
//    //    reinterpret_cast<uintptr_t>(&onScaleformMovieSetProjectionMatrix3DDetour)); m_onScaleformMovieSetProjectionMatrix3DHook->create();
//
//    // kind of help with text labels but not enough to make it work
////    REL::Relocation<uintptr_t> updateWorldToCamAddr{ REL::ID(210432) };
////    m_onUpdateWorldToCamHook = std::make_unique<PolyHook2FunctionHook>(updateWorldToCamAddr.address(), reinterpret_cast<uintptr_t>(&onUpdateWorldToCamDetour));
////    m_onUpdateWorldToCamHook->create();
//    //
//    //    REL::Relocation<uintptr_t> updateCollisionMatrixAddr{ REL::ID(210663) };
//    //    m_onUpdateCollisionMatrixHook = std::make_unique<PolyHook2FunctionHook>(updateCollisionMatrixAddr.address(), reinterpret_cast<uintptr_t>(&onUpdateCollisionMatrixDetour));
//    //    m_onUpdateCollisionMatrixHook->create();
////    REL::Relocation<unsigned int*> globalFrameCountAddr{ REL::ID(886292) };
//    //TODO ID=885900 is real framecounter used in render graph
//    REL::Relocation<unsigned int*> globalFrameCountAddr{ RE2::MemoryOffsets::GlobalFrameCounter() };
//    m_globalFrameCount = globalFrameCountAddr.get();
//
////    REL::Relocation<float*> worldFovAddr{ REL::ID(1047819) }; // someFov
////    m_globalWorldFov = worldFovAddr.get();
//}
//
//RE::NiTransform past_transform{};
//
//void CreationEngineCameraManager::onUpdateNiCamera(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
//{
//    UpdateCamera(a_camera, a_data);
//    using func_t       = decltype(onUpdateNiCameraDetour);
//    auto original_func = m_onUpdateNiCameraHook->get_original<func_t>();
//    original_func(a_camera, a_data);
//}
//
//void CreationEngineCameraManager::UpdateCamera(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
//{
//    //    && a_camera != CreationEngineGameModule::GetSceneGraphRoot()->starfieldScene.pStarFieldCamera
//    if (a_camera != CreationEngineGameModule::GetSceneGraphRoot()->worldCamera) {
//        return;
//    }
////    g_smaa_constants.gFarPlane = a_camera->viewFrustum._far;
////    g_smaa_constants.gNearPlane = a_camera->viewFrustum._near;
//    if(Constants::fixedCameraRoll > 0.0f) {
//        auto rot      = glm::inverse(glm::eulerAngleY(-Constants::fixedCameraRoll));
//        auto mat_cast = (RE::NiMatrix3*)&rot;
//        rotateCamera(a_camera, mat_cast);
//        return;
//    }
//    auto vr = VR::get();
//    if (!vr->is_hmd_active() && !g_dlss_debug_info.cameraShake) {
//        return;
//    }
////    spdlog::info("Updating world  camera fc[{}]", vr->m_frame_count);
//
//    if(!g_dlss_debug_info.cameraShake) {
//        CreationEngineRendererModule::Get()->SetWindowSize(0, 0);
//    }
//
//    if (vr->get_current_render_eye() == VRRuntime::Eye::LEFT) {
//        a_camera->local.translate.x = 0.0f;
//    }
//    else {
//        a_camera->local.translate.x = Constants::separation_ipd;
//    }
//
//    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT || g_dlss_debug_info.cameraShake) {
//        return;
//    }
//
//    auto       head_rotation        = vr->get_rotation(0);
//    auto       hmd_rotation         = glm::quat{ glm::extractMatrixRotation(head_rotation) };
//    auto       rot_offset           = vr->get_rotation_offset();
//    const auto current_hmd_rotation = glm::normalize(rot_offset * hmd_rotation);
//    auto       euler                = euler_angles_from_steamvr(current_hmd_rotation);
//
//    auto playerCamera = CreationEngineGameModule::GetPlayerCameraSingleton();
//    // TODO detect if person is in space or ship
//    if (playerCamera->IsInFirstPerson() && playerCamera->pFirstPersonModeState->pitchFlightCameraJoy == 0 && a_camera == CreationEngineGameModule::GetSceneGraphRoot()->worldCamera) {
//        RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
//        //        RE::TransformsManager* transformsManager = RE::TransformsManager::GetSingleton();
//        auto pitch_multiplier = get_pitch_multiplier();
//        player->data.angle.x = player->data.angle.x + prev_euler.x* pitch_multiplier - euler.x* pitch_multiplier;
//        auto yaw_multiplier = get_yaw_multiplier();
//        float yaw = player->data.angle.z + euler.y * yaw_multiplier - prev_euler.y * yaw_multiplier + 2.0f * glm::pi<float>();
//        player->data.angle.z = std::fmod(yaw, 2.0f * glm::pi<float>());
//        auto rot      = glm::inverse(glm::eulerAngleY(-euler.z));
//        prev_euler = euler;
//        if(!Constants::preventCameraRoll) {
//            auto mat_cast = (RE::NiMatrix3*)&rot;
//            rotateCamera(a_camera, mat_cast);
//        }
//
//    }
//    else {
//        auto rot      = glm::inverse(glm::eulerAngleXYZ(-euler.x, -euler.z, euler.y));
//        auto mat_cast = (RE::NiMatrix3*)&rot;
//        rotateCamera(a_camera, mat_cast);
//    }
//}
//
//void CreationEngineCameraManager::onScaleformSetViewPort(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
//{
//    using func_t       = decltype(onScaleformSetViewPortDetour);
//    auto original_func = m_onScaleformSetViewPortHook->get_original<func_t>();
//    onScaleformSetViewPortInternal(thisMovie, viewport);
//    original_func(thisMovie, viewport);
//}
//
//bool adjust_fov = false;
//
//void CreationEngineCameraManager::onScaleformSetViewPortInternal(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
//{
//    auto vr = VR::get();
//    if (!vr->is_hmd_active() || vr->m_left_trigger_down) {
//        return;
//    }
//    auto hmd_width              = vr->get_hmd_width();
//    auto hmd_height             = vr->get_hmd_height();
//    auto viewport_buffer_width  = viewport->bufferWidth;
//    auto viewport_buffer_height = viewport->bufferHeight;
//
//    int   offset_left       = 0;
//    int   offset_top        = 0;
//    float with_multiplier   = 1.0f;
//    float height_multiplier = 1.0f;
//    auto  cc                = reinterpret_cast<RE::Scaleform::GFx::MovieImpl*>(thisMovie);
//    auto  file_url          = cc->GetMovieDef()->GetFileURL();
//    switch (djb2Hash(file_url)) {
//    case "Interface/HUDMenu.gfx"_DJB:
//    case "Interface/HUDMessagesMenu.gfx"_DJB:
//    case "Interface/WorkshopMenu.swf"_DJB:
//        offset_left       = vr->get_current_render_eye() == VRRuntime::Eye::RIGHT ? -150 : 0;
//        with_multiplier   = 0.45f;
//        height_multiplier = 0.45f;
//        break;
//    case "Interface/ScopeMenu.swf"_DJB:
//        offset_left       = vr->get_current_render_eye() == VRRuntime::Eye::RIGHT ? -400 : 0;
//        with_multiplier   = 0.7f;
//        height_multiplier = 0.7f;
//        break;
//    case "Interface/MonocleMenu.swf"_DJB:
//        offset_left       = vr->get_current_render_eye() == VRRuntime::Eye::RIGHT ? -200 : 0;
//        with_multiplier   = 0.7f;
//        height_multiplier = 0.7f;
//        break;
//    case "Interface/GalaxyStarMapMenu.swf"_DJB:
//        with_multiplier   = 0.6f;
//        height_multiplier = 0.6f;
//        //offset_top        = 100;
//        break;
//    case "Interface/FaderMenu.swf"_DJB:
//        break;
//    case "Interface/DialogueMenu.swf"_DJB:
//        offset_left       = vr->get_current_render_eye() == VRRuntime::Eye::RIGHT ? -100 : 0;
//        with_multiplier   = 0.6f;
//        height_multiplier = 0.6f;
//        break;
//    case "Interface/MainMenu.swf"_DJB:
//    case "Interface/DataMenu.swf"_DJB:
//    case "Interface/InventoryMenu.swf"_DJB:
//    case "Interface/LoadingMenu.swf"_DJB:
//    case "Interface/PauseMenu.swf"_DJB:
//    case "Interface/CursorMenu.swf"_DJB:
//    default:
//        with_multiplier   = 0.4f;
//        height_multiplier = 0.35f;
//        break;
//    }
////    with_multiplier *= 2.f;
////    height_multiplier *= 2.f;
//    auto visible_width  = std::min((int)(hmd_width * with_multiplier), viewport_buffer_width);
//    auto visible_height = std::min((int)(hmd_height * height_multiplier), viewport_buffer_height);
//    viewport->width     = visible_width;
//    viewport->height    = visible_height;
//    viewport->left      = std::max(0, (int)(viewport_buffer_width - visible_width) / 2 + offset_left);
//    viewport->top       = std::max(0, (int)(viewport_buffer_height - visible_height) / 2 + offset_top);
////    spdlog::info("movie {} [{}]", fmt::ptr(thisMovie), file_url);
//}
//
//void CreationEngineCameraManager::rotateCamera(RE::NiCamera* a_camera, const RE::NiMatrix3* a_rot)
//{
//    // TODO perhaps need to move this logic somewhere else
//    if (originalRotations.find((uintptr_t)a_camera) == originalRotations.end()) {
//        originalRotations[(uintptr_t)a_camera] = a_camera->local.rotate;
//    }
//
//    auto originalRotation  = originalRotations[(uintptr_t)a_camera];
//    a_camera->local.rotate = originalRotation * *a_rot;
//}
//
//void CreationEngineCameraManager::onSetNimFrustum(RE::NiCamera* pCamera, RE::NiFrustum* pFrustum)
//{
//    using func_t       = decltype(onSetNimFrustumDetour);
//    auto original_func = m_onSetNimFrustumHook->get_original<func_t>();
//    onSetNiFrustumInternal(pCamera, pFrustum);
//    original_func(pCamera, pFrustum);
//}
//
//void aiming_adjustments(Vector4f& frustum, float fov_adjustment) {
//    frustum[0] = std::tanf(std::atanf(frustum[0]) - fov_adjustment);
//    frustum[1] = std::tanf(std::atanf(frustum[1]) + fov_adjustment);
//    frustum[2] = std::tanf(std::atanf(frustum[2]) + fov_adjustment);
//    frustum[3] = std::tanf(std::atanf(frustum[3]) - fov_adjustment);
//}
//
//void CreationEngineCameraManager::onSetNiFrustumInternal(RE::NiCamera* pCamera, RE::NiFrustum* pFrustum)
//{
//    if (!isValidCamera(pCamera)) {
//        return;
//    }
//    auto vr = VR::get();
//    if (!vr->is_hmd_active()) {
//        return;
//    }
//    auto eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;
//    auto runtime         = vr->get_runtime();
//    auto backbuffer_size = vr->get_backbuffer_size();
//    auto widthAdjust     = adjust_fov ? ((float)backbuffer_size[0] / (float)runtime->get_width()) : 1.0f;
//    auto heightAdjust    = adjust_fov ? ((float)backbuffer_size[1] / (float)runtime->get_height()) : 1.0f;
//    Vector4f frustum         = runtime->frustums[eye];
//    aiming_adjustments(frustum, get_fov_adjustment());
//    pFrustum->left       = frustum[0] * widthAdjust;
//    pFrustum->right      = frustum[1] * widthAdjust;
//    pFrustum->top        = frustum[2] * heightAdjust;
//    pFrustum->bottom     = frustum[3] * heightAdjust;
//}
//
//void CreationEngineCameraManager::onScaleformMovieSetProjectionMatrix3D(uintptr_t* pInt, Matrix4x4f& mat)
//{
//    using func_t       = decltype(onScaleformMovieSetProjectionMatrix3DDetour);
//    auto original_func = m_onScaleformMovieSetProjectionMatrix3DHook->get_original<func_t>();
//    spdlog::info("Setting projection matrix[0] {} {} {} {}", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
//    spdlog::info("Setting projection matrix[1] {} {} {} {}", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
//    spdlog::info("Setting projection matrix[2] {} {} {} {}", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
//    spdlog::info("Setting projection matrix[3] {} {} {} {}", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
//    original_func(pInt, mat);
//}
//
//RE::NiMatrix3 left_rotation;
//RE::NiPoint4  left_translate;
//Matrix4x4f    left_projection;
//
//Matrix4x4f* CreationEngineCameraManager::onUpdateWorldToCam(RE::NiPoint4* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, Matrix4x4f* out)
//{
//    using func_t       = decltype(onUpdateWorldToCamDetour);
//    auto original_func = m_onUpdateWorldToCamHook->get_original<func_t>();
//    auto vr = VR::get();
//    auto worldCam = CreationEngineGameModule::GetSceneGraphRoot()->worldCamera;
//    if(worldCam == nullptr) {
//        return original_func(translate, rotate, pFrustum, orpho, out);
//    }
//////    if(vr->get_runtime()->ready()) {
////    //        spdlog::info("Updating world to cam");
////    //        if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
////    //            auto result = original_func(translate, rotate, pFrustum, orpho, out);
////    //            // copy left_eye projection to out
////    //            memcpy(out, &left_projection, sizeof(Matrix4x4f));
////    //            return result;
////    //        }
////    //        else {
////    //            left_translate = *translate;
////    //            left_rotation = *rotate;
////    //            auto result = original_func(translate, rotate, pFrustum, orpho, out);
////    //            left_projection = *out;
////    //            return result;
//////}
////    //    }
//    auto current_frame = m_globalFrameCount == nullptr? 0: *m_globalFrameCount;
////    if (&worldCam->world.rotate == rotate) {
////        if(current_frame %2 == 0) {
//////            translate->pt[0] += Constants::separation_ipd;
////            worldCam->offsetMatrix[3][0] = Constants::separation_ipd;
////        } else {
////            worldCam->offsetMatrix[3][0] = 0;
////
////        }
////
////    }
////    auto result =  original_func(translate, rotate, pFrustum, orpho, out);
////    if (vr->get_current_render_eye() == VRRuntime::Eye::LEFT && &worldCam->world.rotate == rotate) {
//////        translate->pt[0] -= Constants::separation_ipd;
////    }
//    auto result =  original_func(translate, rotate, pFrustum, orpho, out);
//    if(current_frame %2 == 0 && &worldCam->world.rotate == rotate) {
////        worldCam->world.translate.x += Constants::separation_ipd;
//    }
//    return result;
//}
//
//Matrix6x4f lest_collision_points;
//
//uintptr_t CreationEngineCameraManager::onUpdateCollisionMatrix(Matrix6x4f* points, RE::NiFrustum* frustum, char orpho, Matrix4x4f* transform)
//{
//    using func_t       = decltype(onUpdateCollisionMatrixDetour);
//    auto original_func = m_onUpdateCollisionMatrixHook->get_original<func_t>();
//
//    auto vr       = VR::get();
//    auto worldCam = CreationEngineGameModule::GetSceneGraphRoot()->worldCamera;
//    if (vr->get_runtime()->ready() && (Matrix4x4f*)&worldCam->world == transform) {
//        spdlog::info("Updating collision matrix");
//        if (vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
//            auto result = original_func(points, frustum, orpho, transform);
//            *points     = lest_collision_points;
//            return result;
//        }
//        else {
//            auto result           = original_func(points, frustum, orpho, transform);
//            lest_collision_points = *points;
//            return result;
//        }
//    }
//    return original_func(points, frustum, orpho, transform);
//}
//
//void CreationEngineCameraManager::onSetViewport(RE::NiCamera* pCamera, CreationEngine::Viewport* pViewport)
//{
//    using func_t       = decltype(onSetViewportDetour);
//    auto original_func = m_onSetViewportHook->get_original<func_t>();
//    //    spdlog::info("Setting viewport for camera [{}] {}, l{} r{} t{} b{}",fmt::ptr(pCamera), pCamera->name, pViewport->left, pViewport->right, pViewport->top,
//    //    pViewport->bottom);
//
//    original_func(pCamera, pViewport);
//}
//
//void CreationEngineCameraManager::onSetCameraScissor(RE::NiCamera* pCamera, CreationEngine::Viewport* pViewport)
//{
//    using func_t       = decltype(onSetCameraScissorDetour);
//    auto original_func = m_onSetCameraScissorHook->get_original<func_t>();
//    //    spdlog::info("Setting scissor for camera [{}] {}, l{} r{} t{} b{}",fmt::ptr(pCamera), pCamera->name, pViewport->left, pViewport->right, pViewport->top,
//    //    pViewport->bottom);
//    original_func(pCamera, pViewport);
//}
//
///**
// TODO need to save previous world NiObject from frame before
//      criteria:
//           if NiACObject::lastUpdatedFrame == -1 or (NiUpdateData::frameCount || globalFrameCount)
// * - NiACObject::lastUpdatedFrame > 1
//           then update NiACObject
//       Perhaps it will solve motion vectors issue
// */
//uint64_t CreationEngineCameraManager::onUpdateWorld(RE::NiAVObject* pObject, RE::NiUpdateData* pData)
//{
//    using func_t                = decltype(onUpdateWorldDetour);
//    auto original_func          = m_onUpdateWorldHook->get_original<func_t>();
//    bool should_copy_prev_world = false;
//    auto temp                   = pObject->world;
//    auto frame_count            = pData ? pData->frameCounter : *m_globalFrameCount;
//    if (pObject->lastUpdatedFrame == -1 || pObject->lastUpdatedFrame < frame_count) {
//        should_copy_prev_world = true;
//    }
//    auto result = original_func(pObject, pData);
//    if (should_copy_prev_world && frame_count > 1) {
//        auto obj_pointer = (uintptr_t)pObject;
//        //        if(twoPastFramesPrevTransforms.find(obj_pointer) != twoPastFramesPrevTransforms.end()) {
//        //            pObject->previousWorld = twoPastFramesPrevTransforms[obj_pointer];
//        //        }
//        twoPastFramesPrevTransforms[obj_pointer] = temp;
//    }
//    return result;
//}
//
//void CreationEngineCameraManager::onCalcNiFrustum(RE::NiCamera* pCamera, float fov, float aspectRatio, float nearz, float farz, char lodAdjust) {
//    using func_t = decltype(onCalcNimFrustumDetour);
//    auto original_func = m_onCalcNimFrustumHook->get_original<func_t>();
//    auto playerCamera = CreationEngineGameModule::GetPlayerCameraSingleton();
//
////    spdlog::info("Setting frustum for camera [{}] {} camera[{} {}] setFov[{}]", fmt::ptr(pCamera), pCamera->name, playerCamera->fov, playerCamera->notViewFov, fov);
//    auto  vr = VR::get();
//
//    if(vr->is_hmd_active() && CreationEngineGameModule::GetSceneGraphRoot()->worldCamera == pCamera) {
////        fov = fov + Constants::lodAdjustFov;
//        m_fov_adjust = fov - playerCamera->fov;
//        vr->m_nearz = nearz;
//        vr->m_farz = farz;
//
//    }
//    original_func(pCamera, fov, aspectRatio, nearz, farz, lodAdjust);
//    if(lodAdjust) {
//        pCamera->m_fLODAdjust = ((fov + Constants::lodAdjustFov)/ 90.0f);
//    }
//
//}
//
//float CreationEngineCameraManager::get_fov_adjustment() const
//{
//    return (m_fov_adjust * Constants::DEG_TO_RAD) / 2;
//}
//
//float CreationEngineCameraManager::get_pitch_multiplier() const
//{
//    auto playerCamera = CreationEngineGameModule::GetPlayerCameraSingleton();
//    auto vr = VR::get();
//    auto eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;
//
//    auto frustum = vr->get_runtime()->frustums[eye];
//    auto multiplier = ((playerCamera->fov + m_fov_adjust) * Constants::DEG_TO_RAD)/ tanf(frustum[3] - frustum[2]);
////    spdlog::info("pitch multiplier {}", multiplier);
//    return multiplier;
//}
//
//float CreationEngineCameraManager::get_yaw_multiplier() const
//{
//    auto playerCamera = CreationEngineGameModule::GetPlayerCameraSingleton();
//    auto vr = VR::get();
//    auto eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;
//
//    auto frustum = vr->get_runtime()->frustums[eye];
//    auto multiplier = ((playerCamera->fov + m_fov_adjust) * Constants::DEG_TO_RAD)/ tanf(frustum[0] - frustum[1]);
////    spdlog::info("yaw multiplier {}", multiplier);
//    return multiplier;
//}
