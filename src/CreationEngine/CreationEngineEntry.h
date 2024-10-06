#pragma once
#include <CreationEngine/CreationEngineCameraManager.h>
#include <CreationEngine/CreationEngineRendererModule.h>
#include <CreationEngine/UpscalerAfrNvidiaModule.h>
#include <Mod.hpp>

class CreationEngineEntry : public Mod
{
public:
    inline static std::shared_ptr<CreationEngineEntry>& Get()
    {
        static std::shared_ptr<CreationEngineEntry> instance{ std::make_shared<CreationEngineEntry>() };
        return instance;
    }

    [[nodiscard]] inline std::string_view get_name() const override { return "CreationEngine"; }

    std::optional<std::string> on_initialize() override;
    void                       on_draw_ui() override;
    void                       on_config_load(const utility::Config& cfg) override;
    void                       on_config_save(utility::Config& cfg) override;

private:
    inline static const std::vector<std::string> s_dominant_eye{
        "Right",
        "Left",
    };
    const ModCombo::Ptr m_dominant_eye{ ModCombo::create(generate_name("DominantEye"), s_dominant_eye) };

    ValueList m_options{ *m_dominant_eye };
};
