#pragma once
#include <Mod.hpp>

class GameSettingsComponent : public Mod
{
public:

    inline static std::shared_ptr<GameSettingsComponent>& Get()
    {
        static std::shared_ptr<GameSettingsComponent> instance{ std::make_shared<GameSettingsComponent>() };
        return instance;
    }

    [[nodiscard]] inline std::string_view get_name() const override { return "StarFieldSettings"; }

    std::optional<std::string> on_initialize() override;
    void                       on_draw_ui() override;
    void                       on_config_load(const utility::Config& cfg) override;
    void                       on_config_save(utility::Config& cfg) override;
    void                       on_frame() override;
    void                       on_device_reset() override;
    std::optional<std::string> on_initialize_d3d_thread() override;

private:
    const ModSlider::Ptr m_weapon_fov{ ModSlider::create(generate_name("WeaponFov"), 10.0, 179.0, 120.0) };
    const ModToggle::Ptr m_override_weapon_fov{ ModToggle::create(generate_name("OverrideWeaponFov"), false) };
    ValueList m_options{ *m_weapon_fov, *m_override_weapon_fov };
};
