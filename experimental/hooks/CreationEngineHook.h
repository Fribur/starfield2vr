#pragma once
//#include "RE/MultiCamervaViewModule.h"
#include "utils/FunctionHook.hpp"
#include <memory>

typedef void* (*updateProjectionMatrixF)(void* translate, void* rotate, void* pFrustum, char orpho, void* out);
//typedef float* (*updateProjectionMatrixF)(RE::NiPoint4* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, float* out);
//typedef void (__cdecl* updateProjectionMatrixF2)(RE::NiPoint4* out,RE::NiFrustum *frustum, char orpho, RE::NiTransform *transform);



class CreationEngineHook {
public:
    virtual ~CreationEngineHook();
    static CreationEngineHook* Get();
    void on_frame_d3d12();
//    RE::MultiCameraViewModule* m_multiCameraViewModule{nullptr};
    bool flip_camera{ false};
//    virtual void onUpdateProjectionMatrix(RE::NiPoint4* out,RE::NiFrustum *frustum, char orpho, RE::NiTransform *transform);
//    bool is_hooked() const { return m_hooked; }
//
//    virtual void onUpdateProjectionMatrix2(RE::NiPoint3A* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, float* out);
private:
    CreationEngineHook();
    bool m_hooked{false};
//    std::unique_ptr<FunctionHook> updateProjectionMatrixHook{};
//    updateProjectionMatrixF2 m_original{nullptr};
};