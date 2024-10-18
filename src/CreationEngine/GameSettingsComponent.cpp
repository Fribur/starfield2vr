#include "CreationEngineSettings.h"
#include "GameSettingsComponent.h"

std::optional<std::string> GameSettingsComponent::on_initialize()
{
    return Mod::on_initialize();
}

void GameSettingsComponent::on_draw_ui()
{
    if (!ImGui::CollapsingHeader(get_name().data())) {
        return;
    }
    ImGui::Separator();
    auto creation_engine_settings = CreationEngineSettings::Get();
    {

        static float weaponFov = creation_engine_settings->get_setting("fFPGeometryFOV:Camera", CreationEngineSettings::SettingType::kINISetting, 90.0f);
        if (ImGui::SliderFloat("fFPGeometryFOV:Camera", &weaponFov, 10.0f, 270.0f)) {
            creation_engine_settings->set_setting("fFPGeometryFOV:Camera", CreationEngineSettings::SettingType::kINISetting, weaponFov);
        }
    }
    {
        //"fNarrowAspectLimit:Display"
        static float wideAspectLimit = creation_engine_settings->get_setting("fWideAspectLimit:Display", CreationEngineSettings::SettingType::kINISetting, 1.5f);
        if (ImGui::SliderFloat("fWideAspectLimit:Display", &wideAspectLimit, 0.8f, 1.5f)) {
            creation_engine_settings->set_setting("fWideAspectLimit:Display", CreationEngineSettings::SettingType::kINISetting, wideAspectLimit);
        }
    }
#ifdef _DEBUG
    {
        static int iSizeW = creation_engine_settings->get_setting("iSize W:Display", CreationEngineSettings::SettingType::kINIPrefSetting, 1920);
        if(ImGui::InputInt("iSize W:Display", &iSizeW)) {
            creation_engine_settings->set_setting("iSize W:Display", CreationEngineSettings::SettingType::kINIPrefSetting, iSizeW);
        }
    }
    {
        static bool scan = false;
        if(ImGui::Checkbox("Scan Options", &scan)) {
            if(scan) {
                CreationEngineSettings::get_setting("test", CreationEngineSettings::SettingType::kGameSetting);
                CreationEngineSettings::get_setting("test", CreationEngineSettings::SettingType::kRendererQualitySetting);
                CreationEngineSettings::get_setting("test", CreationEngineSettings::SettingType::kRendererPerfSetting);
                CreationEngineSettings::get_setting("test", CreationEngineSettings::SettingType::kINISetting);
                CreationEngineSettings::get_setting("test", CreationEngineSettings::SettingType::kINIPrefSetting);
            }
        }
    }
#endif
}

void GameSettingsComponent::on_config_load(const utility::Config& cfg) {
    for (IModValue& option : m_options) {
        option.config_load(cfg);
    }
}

void GameSettingsComponent::on_config_save(utility::Config& cfg)
{
    for (IModValue& option : m_options) {
        option.config_save(cfg);
    }
}