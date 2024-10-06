#include "CreationEngineEntry.h"
#include "CreationEngineConstants.h"
#include <mods/VR.hpp>

std::optional<std::string> CreationEngineEntry::on_initialize()
{
    CreationEngineCameraManager::Get()->InstallHooks();
    CreationEngineRendererModule::Get()->InstallHooks();
    UpscalerAfrNvidiaModule::Get();
    return Mod::on_initialize();
}

void CreationEngineEntry::on_draw_ui()
{
    if (!ImGui::CollapsingHeader(get_name().data())) {
        return;
    }
    auto vr = VR::get();
    ImGui::Text("IPD: %f", vr->get_runtime()->get_ipd());
    ImGui::Text("DominantEye Reflect %d", Constants::dominantEye);
    if(m_dominant_eye->draw("Dominant Eye"))
    {
        Constants::dominantEye = m_dominant_eye->value();
    }
}

void CreationEngineEntry::on_config_load(const utility::Config& cfg) {
    for (IModValue& option : m_options) {
        option.config_load(cfg);
    }
    Constants::dominantEye = m_dominant_eye->value();
}

void CreationEngineEntry::on_config_save(utility::Config& cfg)
{
    for (IModValue& option : m_options) {
        option.config_save(cfg);
    }
}
