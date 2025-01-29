#pragma once
#include <safetyhook/inline_hook.hpp>
#include <sl.h>
#include <sl_reflex.h>
#include <utility/PointerHook.hpp>
#include <utils/FunctionHook.h>

class CreationEngineGameLoop
{
public:
    inline static std::shared_ptr<CreationEngineGameLoop>& Get()
    {
    static std::shared_ptr<CreationEngineGameLoop> instance{ std::make_shared<CreationEngineGameLoop>() };
    return instance;
    }
    inline CreationEngineGameLoop() {
        install_hooks();
    }
    ~CreationEngineGameLoop() = default;
private:
    void install_hooks();

    safetyhook::InlineHook m_worldTick_hook{};
    safetyhook::InlineHook m_setReflexMarkerInternalHook{};
    std::unique_ptr<FunctionHook> m_onCopyMatrixHook{};

    std::unique_ptr<PointerHook>  m_onSlReflexSetMarkerHook{};

    static uintptr_t worldTick(uintptr_t );
    static sl::Result             onSlReflexSetMarker(sl::ReflexMarker marker, const sl::FrameToken& frame);
};