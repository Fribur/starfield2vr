//
// Created by sergp on 6/23/2024.
//

#include "CreationEngineCameraManager.h"
#include "CreationEngineConstants.h"
#include "CreationEngineRendererModule.h"
#include "CreationEngineSingletonManager.h"
#include "RE/P/PlayerCamera.h"
#include "RE/S/ScaleformGFxMovie.h"
#include <CreationEngine/memory/offsets.h>
#include <CreationEngine/models/GameFlow.h>
#include <CreationEngine/models/ModSettingsStore.h>
#include <REL/Relocation.h>
#include <cstdlib>
#include <glm/gtx/vector_angle.hpp>
#include <mods/VR.hpp>
/*

void onUpdateNiCameraDetour(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
{
    CreationEngineCameraManager::Get()->onUpdateNiCamera(a_camera, a_data);
}
*/

void onSetNimFrustumDetour(RE::NiCamera* camera, RE::NiFrustum* frustum)
{
    CreationEngineCameraManager::Get()->onSetNimFrustum(camera, frustum);
}

void onCalcNimFrustumDetour(RE::NiCamera* camera, float fov, float aspectRatio, float nearDist, float farDist, char lodAdjust)
{
    CreationEngineCameraManager::Get()->onCalcNiFrustum(camera, fov, aspectRatio, nearDist, farDist, lodAdjust);
}

void onScaleformSetViewPortDetour(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
{
    CreationEngineCameraManager::Get()->onScaleformSetViewPort(thisMovie, viewport);
}
/*

void onScaleformMovieSetProjectionMatrix3DDetour(uintptr_t* thisMovie, Matrix4x4f& matrix)
{
    CreationEngineCameraManager::Get()->onScaleformMovieSetProjectionMatrix3D(thisMovie, matrix);
}
*/

bool isValidCamera(RE::NiCamera* pCamera)
{
    static auto sceneGraphRoot = CreationEngineSingletonManager::GetSceneGraphRoot();
    return pCamera == sceneGraphRoot->worldCamera || pCamera == sceneGraphRoot->starfieldScene.pStarFieldCamera || sceneGraphRoot->starfieldScene.pGalaxyCamera == pCamera;
}

void CreationEngineCameraManager::InstallHooks()
{
//    REL::Relocation<uintptr_t> niCameraUpdateVFuncAddr{ GameStore::MemoryOffsets::NiCamera::UpdateWorldVfunc() };
//    m_onUpdateNiCameraHook = std::make_unique<FunctionHook>(niCameraUpdateVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onUpdateNiCameraDetour));
//    m_onUpdateNiCameraHook->create();

    REL::Relocation<uintptr_t> setNimFrustumVFuncAddr{ GameStore::MemoryOffsets::NiCamera::SetFrustumVfunc() };
    m_onSetNimFrustumHook = std::make_unique<FunctionHook>(setNimFrustumVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onSetNimFrustumDetour));
    m_onSetNimFrustumHook->create();

    REL::Relocation<uintptr_t> calcNimFrustumVFuncAddr{ GameStore::MemoryOffsets::NiCamera::CalcFrustumVfunc() };
    m_onCalcNimFrustumHook = std::make_unique<FunctionHook>(calcNimFrustumVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onCalcNimFrustumDetour));
    m_onCalcNimFrustumHook->create();

    REL::Relocation<uintptr_t> scaleformSetViewPortAddr{ GameStore::MemoryOffsets::Scaleform::Movie::SetViewportVFunc() };
    m_onScaleformSetViewPortHook = std::make_unique<FunctionHook>(scaleformSetViewPortAddr.address(), reinterpret_cast<uintptr_t>(&onScaleformSetViewPortDetour));
    m_onScaleformSetViewPortHook->create();
}
/*
void CreationEngineCameraManager::onUpdateNiCamera(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
{
//    UpdateCamera(a_camera, a_data);
    using func_t              = decltype(onUpdateNiCameraDetour);
    static auto original_func = m_onUpdateNiCameraHook->get_original<func_t>();
    original_func(a_camera, a_data);
}*/

glm::quat rotationDifference(const glm::quat& q1, const glm::quat& q2, float animationTime = 1.0f) {
    // Calculate the relative rotation
    auto endRotation = glm::slerp(q1, q2, animationTime);
    //TODO try switch
    glm::quat relativeRotation = endRotation * glm::inverse(q1);

    // Ensure the rotation is along the shortest arc
    if (relativeRotation.w < 0) {
        relativeRotation = -relativeRotation;
    }

    return relativeRotation;
}
/*
glm::quat extractZRotation(const glm::quat& q) {
    // Convert quaternion to rotation matrix
    glm::mat4 rotMat = glm::mat4_cast(q);

    // Extract the Z-axis rotation angle
    float angle = std::atan2(rotMat[0][1], rotMat[0][0]);

    // Create a new quaternion with only Z-axis rotation
    return glm::angleAxis(angle, glm::vec3(0, 0, 1));
}


void CreationEngineCameraManager::UpdateCamera(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
{
    //    && a_camera != CreationEngineGameModule::GetSceneGraphRoot()->starfieldScene.pStarFieldCamera
    if (a_camera == nullptr || a_camera != CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera) {
        return;
    }
    static auto vr = VR::get();

    if (!vr->is_hmd_active() || ModConstants::cameraShake || GameFlow::shouldShowFlatScreen()) {
        return;
    }
    //    spdlog::info("Updating world  camera fc[{}]", vr->m_frame_count);

    bool  isLeftEye    = vr->get_current_render_eye() == VRRuntime::Eye::LEFT;
    auto  playerCamera = CreationEngineSingletonManager::GetPlayerCameraSingleton();
    bool  isHeadGun    = ModConstants::headTrackingType == 0 && playerCamera->IsInFirstPerson() && playerCamera->pFirstPersonModeState->pitchFlightCameraJoy == 0;

    if (isHeadGun) {
        auto left_eye_pos = vr->get_runtime()->eyes[0][3];
        auto right_eye_pos = vr->get_runtime()->eyes[1][3];
        auto eye_delta = isLeftEye ? left_eye_pos - right_eye_pos : right_eye_pos - left_eye_pos;
        if (ModConstants::dominantEye == isLeftEye) {
            a_camera->local.translate.x = 0.0f;
            a_camera->local.translate.y = 0.0f;
            a_camera->local.translate.z = 0.0f;
        } else {
            a_camera->local.translate.x = eye_delta.x;
            a_camera->local.translate.y = eye_delta.y;
            a_camera->local.translate.z = eye_delta.z;
        }
    }
    else {
        auto eye_pos                = vr->get_runtime()->eyes[isLeftEye ? 0 : 1][3];
        a_camera->local.translate.x = eye_pos.x;
        a_camera->local.translate.y = eye_pos.y;
        a_camera->local.translate.z = eye_pos.z;
    }

    // TODO move update matrices logic to VR runtime so matrix calculation would be done once and only every other frame
    if (!isLeftEye || ModConstants::cameraShake) {
        return;
    }

    auto       head_rotation        = vr->get_rotation(0);
    auto       hmd_rotation         = glm::quat{ glm::extractMatrixRotation(head_rotation) };
    auto       rot_offset           = vr->get_rotation_offset();
    const auto current_hmd_rotation = glm::normalize(rot_offset * hmd_rotation);

    // TODO detect if person is in space or ship
    if (isHeadGun) {
        //TODO possible it is better to move this logic before camera update I suspect it has one frame off
        auto player = CreationEngineSingletonManager::GetPlayerRef();
        //        RE::TransformsManager* transformsManager = RE::TransformsManager::GetSingleton();
        auto pitch_multiplier = get_head_tracking_multiplier();
        auto rotation_diff = rotationDifference(prev_quat, current_hmd_rotation, pitch_multiplier);
        auto euler_diff    = euler_angles_from_steamvr(rotation_diff);
        //TODO use lerp insead of multiplication
        player->data.angle.x  = player->data.angle.x - euler_diff.x;
        float yaw             = player->data.angle.z + euler_diff.y + 2.0f * glm::pi<float>();
        player->data.angle.z  = std::fmod(yaw, 2.0f * glm::pi<float>());
        prev_quat             = current_hmd_rotation;
        auto roll_quat        = extractZRotation(current_hmd_rotation);
        auto left_handed_coord      = glm::quat(-roll_quat.w, roll_quat.x, -roll_quat.z, roll_quat.y);
        auto rot                    = glm::transpose(glm::mat4_cast(left_handed_coord));
        if (!ModConstants::preventCameraRoll) {
            auto mat_cast = (RE::NiMatrix3*)&rot;
            rotateCamera(a_camera, mat_cast);
        }
    }
    else {
        //        auto rot      = glm::inverse(glm::eulerAngleXYZ(-euler.x, -euler.z, euler.y));
        auto left_handed_coord      = glm::quat(-current_hmd_rotation.w, current_hmd_rotation.x, -current_hmd_rotation.z, current_hmd_rotation.y);
        auto rot                    = glm::transpose(glm::mat4_cast(left_handed_coord));
        auto mat_cast               = (RE::NiMatrix3*)&rot;
        rotateCamera(a_camera, mat_cast);
    }
}
*/

void CreationEngineCameraManager::onScaleformSetViewPort(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
{
    using func_t              = decltype(onScaleformSetViewPortDetour);
    static auto original_func = m_onScaleformSetViewPortHook->get_original<func_t>();
    onScaleformSetViewPortInternal(thisMovie, viewport);
    original_func(thisMovie, viewport);
}

struct MenuSettings
{
    float width_multiplier;
    float height_multiplier;
    int   offset_value;
};

const std::unordered_map<uint32_t, MenuSettings> menu_settings = {
    {           "Interface/HUDMenu.gfx"_DJB, { 0.45f, 0.45f, 150 } },
    {           "Interface/HUDMenu_LRG.gfx"_DJB, { 0.45f, 0.45f, 150 } },
    {   "Interface/HUDMessagesMenu.gfx"_DJB, { 0.45f, 0.45f, 150 } },
    {   "Interface/HUDMessagesMenu_LRG.gfx"_DJB, { 0.45f, 0.45f, 150 } },
    {      "Interface/WorkshopMenu.swf"_DJB, { 0.45f, 0.45f, 150 } },
    {      "Interface/WorkshopMenu_LRG.swf"_DJB, { 0.45f, 0.45f, 150 } },
    {         "Interface/ScopeMenu.swf"_DJB,   { 0.7f, 0.7f, 400 } },
    {         "Interface/ScopeMenu_LRG.swf"_DJB,   { 0.7f, 0.7f, 400 } },
    {       "Interface/MonocleMenu.swf"_DJB,   { 0.7f, 0.7f, 200 } },
    {       "Interface/MonocleMenu_LRG.swf"_DJB,   { 0.7f, 0.7f, 200 } },
    { "Interface/GalaxyStarMapMenu.swf"_DJB,     { 0.6f, 0.6f, 0 } },
    { "Interface/GalaxyStarMapMenu_LRG.swf"_DJB,     { 0.6f, 0.6f, 0 } },
    {      "Interface/DialogueMenu.swf"_DJB,   { 0.6f, 0.6f, 100 } },
    {      "Interface/DialogueMenu_LRG.swf"_DJB,   { 0.6f, 0.6f, 100 } },
    // Default settings
    {          "Interface/MainMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {          "Interface/MainMenu_LRG.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {          "Interface/DataMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {          "Interface/DataMenu_LRG.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {     "Interface/InventoryMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {     "Interface/InventoryMenu_LRG.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {       "Interface/LoadingMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {       "Interface/LoadingMenu_LRG.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {         "Interface/PauseMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {         "Interface/PauseMenu_LRG.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {        "Interface/CursorMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {        "Interface/CursorMenu_LRG.swf"_DJB,    { 0.4f, 0.35f, 0 } },
};

void CreationEngineCameraManager::onScaleformSetViewPortInternal(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
{

    static auto vr = VR::get();
    auto  cc                = reinterpret_cast<RE::Scaleform::GFx::MovieImpl*>(thisMovie);
    auto  file_url          = cc->GetMovieDef()->GetFileURL();
    auto  hash              = djb2Hash(file_url);
    GameFlow::renderMenu(file_url);
    if (GameFlow::shouldShowFlatScreen()|| !vr->is_hmd_active()) {
        return;
    }
    auto hmd_width              = vr->get_hmd_width();
    auto hmd_height             = vr->get_hmd_height();
    auto viewport_buffer_width  = viewport->bufferWidth;
    auto viewport_buffer_height = viewport->bufferHeight;

    int   offset_left       = 0;
    int   offset_top        = 0;
    float width_multiplier  = 1.0f;
    float height_multiplier = 1.0f;

    MenuSettings settings = { 0.4f, 0.35f, 0 }; // Default settings
    auto         it       = menu_settings.find(hash);
    if (it != menu_settings.end()) {
        settings = it->second;
    }

    width_multiplier  = settings.width_multiplier;
    height_multiplier = settings.height_multiplier;

    // Implement offset based on dominant eye and menu-specific offset_value
    auto current_eye = vr->get_current_render_eye();
    if (ModConstants::dominantEye == 1) {
        // Dominant eye is right
        offset_left = (current_eye == VRRuntime::Eye::RIGHT) ? -settings.offset_value : 0;
    }
    else {
        // Dominant eye is left
        offset_left = (current_eye == VRRuntime::Eye::LEFT) ? settings.offset_value : 0;
    }

    auto visible_width  = std::min((int)(hmd_width * width_multiplier), viewport_buffer_width);
    auto visible_height = std::min((int)(hmd_height * height_multiplier), viewport_buffer_height);
    viewport->width     = visible_width;
    viewport->height    = visible_height;
    viewport->left      = std::max(0, (int)(viewport_buffer_width - visible_width) / 2 + offset_left);
    viewport->top       = std::max(0, (int)(viewport_buffer_height - visible_height) / 2 + offset_top);
    //    spdlog::info("movie {} [{}]", fmt::ptr(thisMovie), file_url);
}
/*

void CreationEngineCameraManager::rotateCamera(RE::NiCamera* a_camera, const RE::NiMatrix3* a_rot)
{
    // TODO perhaps need to move this logic somewhere else
    if (originalRotations.find((uintptr_t)a_camera) == originalRotations.end()) {
        originalRotations[(uintptr_t)a_camera] = a_camera->local.rotate;
    }

    auto originalRotation  = originalRotations[(uintptr_t)a_camera];
    a_camera->local.rotate = originalRotation * *a_rot;
}
*/

void CreationEngineCameraManager::onSetNimFrustum(RE::NiCamera* pCamera, RE::NiFrustum* pFrustum)
{
    using func_t              = decltype(onSetNimFrustumDetour);
    static auto original_func = m_onSetNimFrustumHook->get_original<func_t>();
    onSetNiFrustumInternal(pCamera, pFrustum);
    original_func(pCamera, pFrustum);
}

void aiming_adjustments(Vector4f& frustum, float fov_adjustment)
{
    frustum[0] = std::tanf(std::atanf(frustum[0]) - fov_adjustment);
    frustum[1] = std::tanf(std::atanf(frustum[1]) + fov_adjustment);
    frustum[2] = std::tanf(std::atanf(frustum[2]) + fov_adjustment);
    frustum[3] = std::tanf(std::atanf(frustum[3]) - fov_adjustment);
}

void CreationEngineCameraManager::onSetNiFrustumInternal(RE::NiCamera* pCamera, RE::NiFrustum* pFrustum)
{
    if (!isValidCamera(pCamera)) {
        return;
    }
    static auto vr = VR::get();
    if (!vr->is_hmd_active()) {
        return;
    }
    auto     eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;
    auto     runtime         = vr->get_runtime();
    Vector4f frustum         = runtime->frustums[eye];
    aiming_adjustments(frustum, get_fov_adjustment());
    pFrustum->left   = frustum[0];
    pFrustum->right  = frustum[1];
    pFrustum->top    = frustum[2];
    pFrustum->bottom = frustum[3];
}

void CreationEngineCameraManager::onCalcNiFrustum(RE::NiCamera* pCamera, float fov, float aspectRatio, float nearz, float farz, char lodAdjust)
{
    using func_t              = decltype(onCalcNimFrustumDetour);
    static auto original_func = m_onCalcNimFrustumHook->get_original<func_t>();
    auto        playerCamera  = CreationEngineSingletonManager::GetPlayerCameraSingleton();

    //    spdlog::info("Setting frustum for camera [{}] {} camera[{} {}] setFov[{}]", fmt::ptr(pCamera), pCamera->name, playerCamera->fov, playerCamera->notViewFov, fov);
    static auto vr = VR::get();

    if (vr->is_hmd_active() && CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera == pCamera) {
        //        fov = fov + Constants::lodAdjustFov;
        m_fov_adjust = fov - playerCamera->fov;
        vr->m_nearz  = nearz;
        vr->m_farz   = farz;
    }
    original_func(pCamera, fov, aspectRatio, nearz, farz, lodAdjust);
}

float CreationEngineCameraManager::get_fov_adjustment() const
{
    return tanf((m_fov_adjust * ModConstants::DEG_TO_RAD) / 2.0f);
}

float CreationEngineCameraManager::get_head_tracking_multiplier() const
{
    static auto playerCamera = CreationEngineSingletonManager::GetPlayerCameraSingleton();
    auto multiplier = (playerCamera->fov + m_fov_adjust) / playerCamera->fov;
    return multiplier * ModConstants::headTrackingMultiplier;
}

void CreationEngineCameraManager::UpdateWorldCamera() {
    static auto vr = VR::get();
    auto worldCamera = CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera;

    if(!worldCamera) {
        return;
    }

    static auto originalRotation = worldCamera->local.rotate;
    static auto originalPosition = worldCamera->local.translate;

    if (!vr->is_hmd_active() || ModConstants::cameraShake || GameFlow::shouldShowFlatScreen()) {
        worldCamera->local.rotate = originalRotation;
        worldCamera->local.translate = originalPosition;
        return;
    }

    static auto prev_rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};

    if(!ModConstants::headTrackingType && GameFlow::isAimingDownSights() && !GameFlow::isImmovable() && !GameFlow::isControlledByAI()) {
        auto p_player = CreationEngineSingletonManager::GetPlayerRef();

        auto hmd_transform = vr->get_rotation(0);
        auto eye = vr->get_current_eye_transform();
        hmd_transform      = eye * hmd_transform;

        auto current_rotation = mat_cast_to_havok_coord(glm::quat{ glm::extractMatrixRotation(hmd_transform) });

        auto diff = rotationDifference(prev_rotation, current_rotation, get_head_tracking_multiplier());

        prev_rotation = current_rotation;
        if(diff.w < 0) {
            diff = -diff;
        }
        float yaw_diff, pitch_diff, roll_diff;
        // YXZ, YZX, ZYX, XYZ
        // ZXY, XZY better
        glm::extractEulerAngleXZY(glm::mat4_cast(diff), pitch_diff, yaw_diff, roll_diff);

        auto new_yaw = p_player->data.angle.z + yaw_diff + 2.0f * glm::pi<float>();
        new_yaw = std::fmod(new_yaw, 2.0f * glm::pi<float>());
        p_player->data.angle.x = p_player->data.angle.x + pitch_diff;
        p_player->data.angle.z = new_yaw;

        auto offset = glm::mat4_cast(glm::quat{current_rotation.w, current_rotation.x, 0, current_rotation.z});
        auto transform = glm::mat4_cast(current_rotation) * glm::inverse(offset);
        transform = glm::rowMajor4(transform);

        worldCamera->local.rotate = originalRotation * *(RE::NiMatrix3*) &transform;


        auto isDominantEye = vr->get_current_render_eye() == (ModConstants::dominantEye == 0 ? VRRuntime::Eye::RIGHT: VRRuntime::Eye::LEFT);
        if(ModConstants::dominantEye == 2) {
            worldCamera->local.translate.x = hmd_transform[3][0];
            worldCamera->local.translate.y = -hmd_transform[3][2];
            worldCamera->local.translate.z = hmd_transform[3][1];
        } else if (isDominantEye) {
            worldCamera->local.translate.x = 0.0f;
            worldCamera->local.translate.y = 0.0f;
            worldCamera->local.translate.z = 0.0f;
        } else {
            auto left_eye_pos = vr->get_runtime()->eyes[0][3];
            auto right_eye_pos = vr->get_runtime()->eyes[1][3];
            auto eye_delta = ModConstants::dominantEye == 0 ? left_eye_pos - right_eye_pos : right_eye_pos - left_eye_pos;
            worldCamera->local.translate.x = eye_delta.x;
            worldCamera->local.translate.y = -eye_delta.z;
            worldCamera->local.translate.z = eye_delta.y;
        }
    } else {
        auto head_rotation = vr->get_transform(0);
        auto eye = vr->get_current_eye_transform();

        head_rotation = eye * head_rotation;
        auto rotation_quat = glm::normalize(glm::quat{head_rotation});
        rotation_quat = {rotation_quat.w, -rotation_quat.x, rotation_quat.z, -rotation_quat.y};
        worldCamera->local.translate.x = head_rotation[3][0];
        worldCamera->local.translate.y = -head_rotation[3][2];
        worldCamera->local.translate.z = head_rotation[3][1];
        head_rotation = glm::mat4_cast(rotation_quat);
        head_rotation = glm::rowMajor4(head_rotation);

        worldCamera->local.rotate = originalRotation * *(RE::NiMatrix3*) & head_rotation;
        if(GameFlow::gStore.internalSettings.headAimingAbsolute) {
            prev_rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
        } else {
            prev_rotation = rotation_quat;
        }
    }
}
