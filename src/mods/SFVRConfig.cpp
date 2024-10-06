#include "../SFVR.hpp"
#include "SFVRConfig.hpp"
#include <CreationEngine/CreationEngineConstants.h>

std::shared_ptr<SFVRConfig>& SFVRConfig::get() {
     static std::shared_ptr<SFVRConfig> instance{std::make_shared<SFVRConfig>()};
     return instance;
}

std::optional<std::string> SFVRConfig::on_initialize() {
    return Mod::on_initialize();
}

void renderMultiSelect() {
    ImGui::Text("Select Stages:");
    for (int i = 0; i < Constants::NUM_STAGES; ++i) {
        std::string id = "StaCopyResourceIdge " + std::to_string(i);

        ImGui::PushID(id.c_str());
        if (ImGui::Checkbox(std::to_string(i).c_str(), &Constants::enabledResourcesToCopy[i])) {
            // Checkbox state changed
        }
        ImGui::PopID();
        if (i < Constants::NUM_STAGES - 1) ImGui::SameLine();
    }
}

void SFVRConfig::on_draw_ui() {
    if (ImGui::CollapsingHeader("Configuration")) {
        ImGui::TreePush("Configuration");

        m_menu_key->draw("Menu Key");
//        m_show_cursor_key->draw("Show Cursor Key");
//        m_remember_menu_state->draw("Remember Menu Open/Closed State");
//        m_always_show_cursor->draw("Draw Cursor With Menu Open");

//        if (m_font_size->draw("Font Size")) {
//            g_framework->set_font_size(m_font_size->value());
//        }

        ImGui::TreePop();
    }
#if defined(_DEBUG) && defined(SFVR_EXPERIMENTAL)
    if(ImGui::CollapsingHeader("Creation Engine")) {
        ImGui::TreePush("Engine");
        ImGui::DragFloat("Lod adjust", &Constants::lodAdjustFov, 1.0f, -180.0f, 180.0f);
        ImGui::SliderFloat("Fixed Angle", &Constants::fixedCameraRoll, 0.0f, (float)(std::numbers::pi/2.0f));
        ImGui::Checkbox("Prevent Camera Roll", &Constants::preventCameraRoll);
        ImGui::Checkbox("Enable AFR camera motion fix", &Constants::enableAFRCameraMotionFix);
        renderMultiSelect();
        ImGui::TreePop();
    }
    if(ImGui::CollapsingHeader("TAA twicks")) {
        ImGui::TreePush("TAA twicks");
        ImGui::Separator();
        ImGui::Checkbox("Enable Debug TAA", &g_taa_settings.enabled);
        ImGui::SliderInt("TAA Resources", &g_taa_settings.debugOutput, -1, 11);
        ImGui::SliderInt("TAA Resources Comp", &g_taa_settings.debugOutput2, -1, 11);
        ImGui::SliderFloat("TAA gamma correction", &g_taa_settings.gamma, 1.0f, 3.0f);
        ImGui::SliderFloat("TAA contrast", &g_taa_settings.contrast, 0.0f, 2.0f);
        ImGui::SliderFloat("TAA temperature", &g_taa_settings.temperature, -1.0f, 5.0f);
        ImGui::SliderFloat("TAA exposure", &g_taa_settings.exposure, -1.0f, 5.0f);
        ImGui::TreePop();
    }

    if(ImGui::CollapsingHeader("DLSS twicks")) {
        ImGui::TreePush("DLSS twicks");
        ImGui::Separator();
        ImGui::Checkbox("Enable Camera Pos fix", &g_dlss_debug_info.enabled);
        ImGui::Checkbox("Enable Camera Shake", &g_dlss_debug_info.cameraShake);
        ImGui::SliderInt("Motion Vector Index fix [0-7]", &g_dlss_debug_info.motionVectorIndex, 0, 7);
        Matrix4x4f& mat = g_dlss_debug_info.currentProjectionMatrix;
        ImGui::Text("Current Projection Matrix");
        for(int row=0; row<4; row++) {
            ImGui::Text("Row[%d]{%f,%f,%f,%f}", row, mat[row][0], mat[row][1], mat[row][2], mat[row][3]);
        }
        ImGui::Text("Previous Projection Matrix");
        mat = g_dlss_debug_info.previousProjectionMatrix;
        for(int row=0; row<4; row++) {
            ImGui::Text("Row[%d]{%f,%f,%f,%f}", row, mat[row][0], mat[row][1], mat[row][2], mat[row][3]);
        }
        ImGui::Text("current position: {%f,%f,%f,%f}", g_dlss_debug_info.currentCameraPosition.x, g_dlss_debug_info.currentCameraPosition.y, g_dlss_debug_info.currentCameraPosition.z, g_dlss_debug_info.currentCameraPosition.w);
        ImGui::Text("previous position: {%f,%f,%f,%f}", g_dlss_debug_info.previousCameraPosition.x, g_dlss_debug_info.previousCameraPosition.y, g_dlss_debug_info.previousCameraPosition.z, g_dlss_debug_info.previousCameraPosition.w);
        ImGui::Separator();

        ImGui::Checkbox("Enable DLSS", &g_dlss_settings.enabled);
        ImGui::Separator();
        ImGui::Text("Debug Output"); ImGui::SameLine();
        ImGui::RadioButton("DLSS pass-through", &g_dlss_settings.debugOutput, 0); ImGui::SameLine();
        ImGui::RadioButton("Motion Vectors", &g_dlss_settings.debugOutput, 1);
        ImGui::TreePop();

    }

    if (ImGui::CollapsingHeader("Anti Aliasing")) {
        ImGui::TreePush("Anti Aliasing");
        ImGui::Checkbox("Enable SMAA", &g_smaa_settings.enabled);
        ImGui::Separator();
        ImGui::Text("Edge Detection"); ImGui::SameLine();
        ImGui::RadioButton("Depth", &g_smaa_settings.edgeDetectionMethod, 0); ImGui::SameLine();
        ImGui::RadioButton("Luma", &g_smaa_settings.edgeDetectionMethod, 1); ImGui::SameLine();
        ImGui::RadioButton("Color", &g_smaa_settings.edgeDetectionMethod, 2);
        ImGui::Checkbox("Predication", &g_smaa_settings.predication);
        ImGui::Text("Debug Output"); ImGui::SameLine();
        ImGui::RadioButton("None", &g_smaa_settings.debugOutput, 0); ImGui::SameLine();
        ImGui::RadioButton("Edge", &g_smaa_settings.debugOutput, 1); ImGui::SameLine();
        ImGui::RadioButton("Blending", &g_smaa_settings.debugOutput, 2);
        ImGui::RadioButton("Area", &g_smaa_settings.debugOutput, 3); ImGui::SameLine();
        ImGui::RadioButton("Search", &g_smaa_settings.debugOutput, 4); ImGui::SameLine();
        ImGui::RadioButton("Depth Debug", &g_smaa_settings.debugOutput, 5);
        ImGui::Separator();
        ImGui::SliderFloat("Threshold", &g_smaa_constants.gThreshold, 0.0f, 0.5f, "%.4f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("Depth Threshold", &g_smaa_constants.gDepthThreshold, 0.0f, 0.5f, "%.4f", ImGuiSliderFlags_Logarithmic);

        ImGui::DragInt("Max Search Steps [0-112]", &g_smaa_constants.gMaxSearchSteps, 1.0f, 0, 250);
        ImGui::DragInt("Max Search Steps Diag [0-20]", &g_smaa_constants.gMaxSearchStepsDiag, 1.0f, 0, 40);
        ImGui::DragInt("Corner Rounding [0-100]", &g_smaa_constants.gCornerRounding, 1.0f, 0, 100);
        ImGui::Separator();
        ImGui::SliderFloat("Contrast Factor", &g_smaa_constants.gContrastFactor, 1.0f, 5.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("Predication Threshold(magic number)", &g_smaa_constants.gPredicationThreshold, 0.005f, 1.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("Predication Scale [1-5]", &g_smaa_constants.gPredicationScale, 1.0f, 5.0f);
        ImGui::SliderFloat("Predication Strength [0-1]", &g_smaa_constants.gPredicationStrength, 0.0f, 1.0f);
        ImGui::TreePop();
    }
#endif
}

void SFVRConfig::on_frame() {
    if (m_show_cursor_key->is_key_down_once()) {
        m_always_show_cursor->toggle();
    }
}

void SFVRConfig::on_config_load(const utility::Config& cfg) {
    for (IModValue& option : m_options) {
        option.config_load(cfg);
    }

    if (m_remember_menu_state->value()) {
        g_framework->set_draw_ui(m_menu_open->value(), false);
    }
    
    g_framework->set_font_size(m_font_size->value());
}

void SFVRConfig::on_config_save(utility::Config& cfg) {
    for (IModValue& option : m_options) {
        option.config_save(cfg);
    }
}
