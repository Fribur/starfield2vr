#pragma once

namespace Constants
{
    inline float minimumCameraRotationAngle =  0.0000099999997f;
    inline float minimumCameraRotationAngle2 = 0.00000011920929f; //epsilon float

    inline float yawMultiplier = 1.0f;
    inline float pitchMultiplier = 1.0f;
    inline float headTrackingMultiplier = 1.0f;

    inline float scaleHUDX = 0.25f;
    inline float scaleHUDY = 0.25f;
    inline constexpr float DEG_TO_RAD = std::numbers::pi / 180;
    inline float lodAdjustFov = 0.0f;
    inline float fixedCameraRoll = 0.0f;
    inline bool preventCameraRoll{false};
    inline bool cameraShake{false};

    inline bool enableAFRCameraMotionFix{true};
    static constexpr int NUM_STAGES = 12;
    inline std::array<bool, NUM_STAGES> enabledResourcesToCopy{false, true, true, true, false, false, false, false, false, false, false, false};
    inline std::array<int, NUM_STAGES> rgResourceCount{2,2,2,2,2,2,2,1,2,2,2,2};
    inline int dominantEye{0};
}
