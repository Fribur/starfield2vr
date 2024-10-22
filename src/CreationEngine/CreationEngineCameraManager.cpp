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
#include <REL/Relocation.h>
#include <cstdlib>
#include <mods/VR.hpp>

constexpr unsigned int djb2Hash(const char* str, int index = 0)
{
    return !str[index] ? 0x1505 : (djb2Hash(str, index + 1) * 0x21) ^ str[index];
}

constexpr unsigned int operator"" _DJB(const char str[], size_t size)
{
    return djb2Hash(str);
}

void onUpdateNiCameraDetour(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
{
    CreationEngineCameraManager::Get()->onUpdateNiCamera(a_camera, a_data);
}

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

void onScaleformMovieSetProjectionMatrix3DDetour(uintptr_t* thisMovie, Matrix4x4f& matrix)
{
    CreationEngineCameraManager::Get()->onScaleformMovieSetProjectionMatrix3D(thisMovie, matrix);
}

bool isValidCamera(RE::NiCamera* pCamera)
{
    auto sceneGraphRoot = CreationEngineSingletonManager::GetSceneGraphRoot();
    return pCamera == sceneGraphRoot->worldCamera || pCamera == sceneGraphRoot->starfieldScene.pStarFieldCamera || sceneGraphRoot->starfieldScene.pGalaxyCamera == pCamera;
}

void CreationEngineCameraManager::InstallHooks()
{
    REL::Relocation<uintptr_t> niCameraUpdateVFuncAddr{ GameStore::MemoryOffsets::NiCamera::UpdateWorldVfunc() };
    m_onUpdateNiCameraHook = std::make_unique<FunctionHook>(niCameraUpdateVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onUpdateNiCameraDetour));
    m_onUpdateNiCameraHook->create();
    REL::Relocation<uintptr_t> setNimFrustumVFuncAddr{ GameStore::MemoryOffsets::NiCamera::SetFrustumVfunc() };
    m_onSetNimFrustumHook = std::make_unique<FunctionHook>(setNimFrustumVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onSetNimFrustumDetour));
    m_onSetNimFrustumHook->create();

    REL::Relocation<uintptr_t> calcNimFrustumVFuncAddr{ GameStore::MemoryOffsets::NiCamera::CalcFrustumVfunc() };
    m_onCalcNimFrustumHook = std::make_unique<FunctionHook>(calcNimFrustumVFuncAddr.address(), reinterpret_cast<uintptr_t>(&onCalcNimFrustumDetour));
    m_onCalcNimFrustumHook->create();

    REL::Relocation<uintptr_t> scaleformSetViewPortAddr{ GameStore::MemoryOffsets::Scaleform::Movie::SetViewportVFunc() };
    m_onScaleformSetViewPortHook = std::make_unique<FunctionHook>(scaleformSetViewPortAddr.address(), reinterpret_cast<uintptr_t>(&onScaleformSetViewPortDetour));
    m_onScaleformSetViewPortHook->create();

    // TODO ID=885900 is real framecounter used in render graph
    REL::Relocation<unsigned int*> globalFrameCountAddr{ GameStore::MemoryOffsets::GlobalFrameCounter() };
    m_globalFrameCount = globalFrameCountAddr.get();
}

void CreationEngineCameraManager::onUpdateNiCamera(RE::NiCamera* a_camera, RE::NiUpdateData* a_data)
{
    UpdateCamera(a_camera, a_data);
    using func_t              = decltype(onUpdateNiCameraDetour);
    static auto original_func = m_onUpdateNiCameraHook->get_original<func_t>();
    original_func(a_camera, a_data);
}

glm::quat rotationDifference(const glm::quat& q1, const glm::quat& q2) {
    // Calculate the relative rotation
    glm::quat relativeRotation = glm::inverse(q1) * q2;

    // Ensure the rotation is along the shortest arc
    if (relativeRotation.w < 0) {
        relativeRotation = -relativeRotation;
    }

    return relativeRotation;
}

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
    if (a_camera != CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera) {
        return;
    }
    auto vr = VR::get();
    if (!vr->is_hmd_active() && !Constants::cameraShake) {
        return;
    }
    //    spdlog::info("Updating world  camera fc[{}]", vr->m_frame_count);

    if (!Constants::cameraShake) {
        CreationEngineRendererModule::Get()->SetWindowSize(0, 0);
    }

    float ipd          = vr->get_runtime()->get_ipd();
    bool  isLeftEye    = vr->get_current_render_eye() == VRRuntime::Eye::LEFT;
    auto  playerCamera = CreationEngineSingletonManager::GetPlayerCameraSingleton();
    bool  isHeadGun    = Constants::headTrackingType == 0 && playerCamera->IsInFirstPerson() && playerCamera->pFirstPersonModeState->pitchFlightCameraJoy == 0;

    if (isHeadGun) {
        if (Constants::dominantEye == 1) {
            a_camera->local.translate.x = isLeftEye ? 0.0f : ipd;
        }
        else {
            a_camera->local.translate.x = isLeftEye ? -ipd : 0.0f;
        }
        a_camera->local.translate.y = 0.0f;
        a_camera->local.translate.z = 0.0f;
    }
    else {
        auto eye_pos                = vr->get_runtime()->eyes[isLeftEye ? 0 : 1][3];
        a_camera->local.translate.x = eye_pos.x;
        a_camera->local.translate.y = eye_pos.y;
        a_camera->local.translate.z = eye_pos.z;
    }

    // TODO move update matrices logic to VR runtime so matrix calculation would be done once and only every other frame
    if (!isLeftEye || Constants::cameraShake) {
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
        auto rotation_diff = rotationDifference(prev_quat, current_hmd_rotation);
        auto euler_diff    = euler_angles_from_steamvr(rotation_diff);
        auto pitch_multiplier = get_pitch_multiplier();
        //TODO use lerp insead of multiplication
        player->data.angle.x  = player->data.angle.x - euler_diff.x * pitch_multiplier;
        auto  yaw_multiplier  = get_yaw_multiplier();
        float yaw             = player->data.angle.z + euler_diff.y * yaw_multiplier + 2.0f * glm::pi<float>();
        player->data.angle.z  = std::fmod(yaw, 2.0f * glm::pi<float>());
        prev_quat             = current_hmd_rotation;
        auto roll_quat        = extractZRotation(current_hmd_rotation);
        auto left_handed_coord      = glm::quat(-roll_quat.w, roll_quat.x, -roll_quat.z, roll_quat.y);
        auto rot                    = glm::transpose(glm::mat4_cast(left_handed_coord));
        if (!Constants::preventCameraRoll) {
            auto mat_cast = (RE::NiMatrix3*)&rot;
            rotateCamera(a_camera, mat_cast);
        }
    }
    else {
        //        auto rot      = glm::inverse(glm::eulerAngleXYZ(-euler.x, -euler.z, euler.y));
        auto left_handed_coord      = glm::quat(-current_hmd_rotation.w, current_hmd_rotation.x, -current_hmd_rotation.z, current_hmd_rotation.y);
        auto rot                    = glm::transpose(glm::mat4_cast(left_handed_coord));
        auto mat_cast               = (RE::NiMatrix3*)&rot;
        auto eye_pos                = vr->get_runtime()->eyes[(isLeftEye ? 0 : 1)][3];
        a_camera->local.translate.x = eye_pos.x;
        a_camera->local.translate.y = eye_pos.y;
        a_camera->local.translate.z = eye_pos.z;
        rotateCamera(a_camera, mat_cast);
    }
}

void CreationEngineCameraManager::onScaleformSetViewPort(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
{
    using func_t              = decltype(onScaleformSetViewPortDetour);
    static auto original_func = m_onScaleformSetViewPortHook->get_original<func_t>();
    onScaleformSetViewPortInternal(thisMovie, viewport);
    original_func(thisMovie, viewport);
}

bool adjust_fov = false;

struct MenuSettings
{
    float width_multiplier;
    float height_multiplier;
    int   offset_value;
};

const std::unordered_map<uint32_t, MenuSettings> menu_settings = {
    {           "Interface/HUDMenu.gfx"_DJB, { 0.45f, 0.45f, 150 } },
    {   "Interface/HUDMessagesMenu.gfx"_DJB, { 0.45f, 0.45f, 150 } },
    {      "Interface/WorkshopMenu.swf"_DJB, { 0.45f, 0.45f, 150 } },
    {         "Interface/ScopeMenu.swf"_DJB,   { 0.7f, 0.7f, 400 } },
    {       "Interface/MonocleMenu.swf"_DJB,   { 0.7f, 0.7f, 200 } },
    { "Interface/GalaxyStarMapMenu.swf"_DJB,     { 0.6f, 0.6f, 0 } },
    {      "Interface/DialogueMenu.swf"_DJB,   { 0.6f, 0.6f, 100 } },
    // Default settings
    {          "Interface/MainMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {          "Interface/DataMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {     "Interface/InventoryMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {       "Interface/LoadingMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {         "Interface/PauseMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
    {        "Interface/CursorMenu.swf"_DJB,    { 0.4f, 0.35f, 0 } },
};

void CreationEngineCameraManager::onScaleformSetViewPortInternal(uintptr_t* thisMovie, Scaleform::Gfx::Viewport* viewport)
{
    auto vr = VR::get();
    if (!vr->is_hmd_active() || vr->m_left_trigger_down) {
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
    auto  cc                = reinterpret_cast<RE::Scaleform::GFx::MovieImpl*>(thisMovie);
    auto  file_url          = cc->GetMovieDef()->GetFileURL();
    auto  hash              = djb2Hash(file_url);

    MenuSettings settings = { 0.4f, 0.35f, 0 }; // Default settings
    auto         it       = menu_settings.find(hash);
    if (it != menu_settings.end()) {
        settings = it->second;
    }

    width_multiplier  = settings.width_multiplier;
    height_multiplier = settings.height_multiplier;

    // Implement offset based on dominant eye and menu-specific offset_value
    auto current_eye = vr->get_current_render_eye();
    if (Constants::dominantEye == 1) {
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

void CreationEngineCameraManager::rotateCamera(RE::NiCamera* a_camera, const RE::NiMatrix3* a_rot)
{
    // TODO perhaps need to move this logic somewhere else
    if (originalRotations.find((uintptr_t)a_camera) == originalRotations.end()) {
        originalRotations[(uintptr_t)a_camera] = a_camera->local.rotate;
    }

    auto originalRotation  = originalRotations[(uintptr_t)a_camera];
    a_camera->local.rotate = originalRotation * *a_rot;
}

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
    auto vr = VR::get();
    if (!vr->is_hmd_active()) {
        return;
    }
    auto     eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;
    auto     runtime         = vr->get_runtime();
    auto     backbuffer_size = vr->get_backbuffer_size();
    auto     widthAdjust     = adjust_fov ? ((float)backbuffer_size[0] / (float)runtime->get_width()) : 1.0f;
    auto     heightAdjust    = adjust_fov ? ((float)backbuffer_size[1] / (float)runtime->get_height()) : 1.0f;
    Vector4f frustum         = runtime->frustums[eye];
    aiming_adjustments(frustum, get_fov_adjustment());
    pFrustum->left   = frustum[0] * widthAdjust;
    pFrustum->right  = frustum[1] * widthAdjust;
    pFrustum->top    = frustum[2] * heightAdjust;
    pFrustum->bottom = frustum[3] * heightAdjust;
}

void CreationEngineCameraManager::onScaleformMovieSetProjectionMatrix3D(uintptr_t* pInt, Matrix4x4f& mat)
{
    using func_t              = decltype(onScaleformMovieSetProjectionMatrix3DDetour);
    static auto original_func = m_onScaleformMovieSetProjectionMatrix3DHook->get_original<func_t>();
    /*spdlog::info("Setting projection matrix[0] {} {} {} {}", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    spdlog::info("Setting projection matrix[1] {} {} {} {}",
     * mat[1][0],

     * * mat[1][1], mat[1][2], mat[1][3]);
    spdlog::info("Setting projection matrix[2] {} {} {} {}", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);

     * spdlog::info("Setting

     * * projection matrix[3] {} {} {} {}", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);*/
    original_func(pInt, mat);
}

void CreationEngineCameraManager::onCalcNiFrustum(RE::NiCamera* pCamera, float fov, float aspectRatio, float nearz, float farz, char lodAdjust)
{
    using func_t              = decltype(onCalcNimFrustumDetour);
    static auto original_func = m_onCalcNimFrustumHook->get_original<func_t>();
    auto        playerCamera  = CreationEngineSingletonManager::GetPlayerCameraSingleton();

    //    spdlog::info("Setting frustum for camera [{}] {} camera[{} {}] setFov[{}]", fmt::ptr(pCamera), pCamera->name, playerCamera->fov, playerCamera->notViewFov, fov);
    auto vr = VR::get();

    if (vr->is_hmd_active() && CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera == pCamera) {
        //        fov = fov + Constants::lodAdjustFov;
        m_fov_adjust = fov - playerCamera->fov;
        vr->m_nearz  = nearz;
        vr->m_farz   = farz;
    }
    original_func(pCamera, fov, aspectRatio, nearz, farz, lodAdjust);
    if (lodAdjust && Constants::lodAdjustFov > 0.0f) {
        pCamera->m_fLODAdjust = ((fov + Constants::lodAdjustFov) / 90.0f);
    }
}

float CreationEngineCameraManager::get_fov_adjustment() const
{
    return (m_fov_adjust * Constants::DEG_TO_RAD) / 2;
}

float CreationEngineCameraManager::get_pitch_multiplier() const
{
    auto playerCamera = CreationEngineSingletonManager::GetPlayerCameraSingleton();
    //    auto vr = VR::get();
    //    auto eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;

    //    auto frustum = vr->get_runtime()->frustums[eye];
    auto multiplier = (playerCamera->fov + m_fov_adjust) / playerCamera->fov;
    //    spdlog::info("pitch multiplier {}", multiplier);
    return multiplier * Constants::headTrackingMultiplier;
}

float CreationEngineCameraManager::get_yaw_multiplier() const
{
    auto playerCamera = CreationEngineSingletonManager::GetPlayerCameraSingleton();
    //    auto vr = VR::get();
    //    auto eye             = vr->get_current_render_eye() == VRRuntime::Eye::LEFT ? 0 : 1;

    //    auto frustum = vr->get_runtime()->frustums[eye];
    auto multiplier = (playerCamera->fov + m_fov_adjust) / playerCamera->fov;
    //    spdlog::info("yaw multiplier {}", multiplier);
    return multiplier * Constants::headTrackingMultiplier;
}
