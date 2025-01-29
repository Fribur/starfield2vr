#include "CreationEngineEntry.h"
#include "CreationEngineConstants.h"
#include "CreationEngineGameLoop.h"
#include "CreationEngineSingletonManager.h"
#include "HavokModule.h"
#include <CreationEngine/models/GameFlow.h>
#include <CreationEngine/models/ModSettingsStore.h>
#include <mods/VR.hpp>

std::optional<std::string> CreationEngineEntry::on_initialize()
{
//    CreationEngineGameLoop::Get();
    CreationEngineCameraManager::Get()->InstallHooks();
    CreationEngineRendererModule::Get()->InstallHooks();
    UpscalerAfrNvidiaModule::Get();
//    HavokModule::Get();
    return Mod::on_initialize();
}

void CreationEngineEntry::on_draw_ui()
{
    if (!ImGui::CollapsingHeader(get_name().data())) {
        return;
    }
    auto vr = VR::get();
    ImGui::Text("IPD: %f", vr->get_runtime()->get_ipd());
    ImGui::Text("DominantEye Reflect %d", ModConstants::dominantEye);
    if(m_dominant_eye->draw("Dominant Eye"))
    {
        ModConstants::dominantEye = m_dominant_eye->value();
    }
    if(m_head_tracking_type->draw("Head Tracking Type"))
    {
        ModConstants::headTrackingType = m_head_tracking_type->value();
    }
    if(m_head_tracking_multiplier->draw("Head Tracking Sensitivity"))
    {
        ModConstants::headTrackingMultiplier = m_head_tracking_multiplier->value();
    }

    {
        for(auto& ui_part : GameFlow::gStore.debugData.ui_parts)
        {
            ImGui::Text("UI: %s", ui_part.data());
        }
    }
#if 0
    {
        ImGui::Text("Frames Since Camera update %d", sdk::g_statefullData.renderData.framesSinceCameraUpdate);
        for(auto& ui_part : sdk::gSettings.debugData.ui_parts)
        {
            ImGui::Text("UI: %s", ui_part.data());
        }
        sdk::gSettings.debugData.ui_parts.clear();
    }

    {
        static auto camera = CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera;
        auto rt_w    = (float)g_framework->get_rendertarget_width_d3d12();
        auto rt_h    = (float)g_framework->get_rendertarget_height_d3d12();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(rt_w, rt_h));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Circle", nullptr,
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

        auto* draw_list = ImGui::GetWindowDrawList();
        int index = 0;

        {

            for (auto& point : sdk::gSettings.debugData.points) {

                ImVec2 center{};
                auto wts = camera->WorldToScreenNormalized({ point.x, point.y, point.z });

                // Draw filled circle in red
                center.x = wts.x * rt_w;
                center.y = wts.y * rt_h;
                draw_list->AddCircleFilled(center, 5.0f, IM_COL32(255, 0, 0, 255));
                char label[64];
                sprintf_s(label, "%d:[%.1f,%.1f,%.1f]", index, point.x, point.y, point.z);
                draw_list->AddText(ImVec2(center.x, center.y + 5), IM_COL32(255, 255, 255, 255), label);
                index++;
            }
        }
        index = 0;
        for(auto& node: sdk::gSettings.debugData.nodes)
        {
            ImVec2 center{};
            RE::NiPoint3& point = node->world.translate;
            auto wts = camera->WorldToScreenNormalized(point);

            // Draw filled circle in red
            center.x = wts.x * rt_w;
            center.y = wts.y * rt_h;
            draw_list->AddCircleFilled(center, 5.0f, IM_COL32(0, 255, 0, 255));
            char label[64];
            sprintf_s(label, "N:%d:[%.1f,%.1f,%.1f]", index, point.x, point.y, point.z);
            draw_list->AddText(ImVec2(center.x, center.y + 5), IM_COL32(255, 255, 255, 255), label);
            index++;

        }

        ImGui::End();
        ImGui::PopStyleVar();
        sdk::gSettings.debugData.points.clear();
        sdk::gSettings.debugData.nodes.clear();

    }
#endif
}

void CreationEngineEntry::on_config_load(const utility::Config& cfg) {
    for (IModValue& option : m_options) {
        option.config_load(cfg);
    }
    ModConstants::dominantEye = m_dominant_eye->value();
    ModConstants::headTrackingMultiplier = m_head_tracking_multiplier->value();
    ModConstants::headTrackingType = m_head_tracking_type->value();
}

void CreationEngineEntry::on_config_save(utility::Config& cfg)
{
    for (IModValue& option : m_options) {
        option.config_save(cfg);
    }
}
