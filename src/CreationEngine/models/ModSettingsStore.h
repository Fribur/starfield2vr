#pragma once
#include "glm/glm.hpp"

namespace ModSettingsStore {
    struct DebugData {
        std::vector<glm::vec3> points{};
        std::vector<std::string_view> ui_parts{};
    };

    struct StatefullData {
        struct PlayerState {
            int dummy{0};
        } playerState{};
        struct RenderData {
            int rendered_menus_count[2]{};
            int modulino{0};
        } renderData{};
        struct RayCastData {
            glm::vec3 weaponRayCastOrigin{};
            glm::vec3 weaponRayCastDestination{};
        } rayCastData{};
    };

    struct Settings
    {
        struct HudSettings {
            float scaleX{1.0f};
            float scaleY{1.0f};
        } hudSettings{};
        struct InternalSettings {
            bool enforceFlatScreen{false};
            float flatScreenDistance{1.5f};

        } internalSettings{};
        StatefullData g_statefullData{};
        DebugData debugData{};
    };

    extern Settings gStore;

    inline void resetGameState() {
        gStore.debugData.ui_parts.clear();
        gStore.g_statefullData.renderData.modulino++;
        gStore.g_statefullData.renderData.rendered_menus_count[gStore.g_statefullData.renderData.modulino % 2] = 0;
    }

    inline void renderMenu(std::string_view menuNameHash) {
        switch (djb2Hash(menuNameHash.data())) {
            case "Interface/HUDMenu.gfx"_DJB:
            case "Interface/SpaceshipHudMenu.swf"_DJB:
//            case "Interface/HUDMessagesMenu.gfx"_DJB:
            case "Interface/ScopeMenu.swf"_DJB:
            case "Interface/FavoritesMenu.swf"_DJB:
            case "Interface/MonocleMenu.swf"_DJB:
            case "Interface/DialogueMenu.swf"_DJB:
                gStore.g_statefullData.renderData.rendered_menus_count[gStore.g_statefullData.renderData.modulino % 2]--;
                break;
            case "Interface/WorkshopMenu.swf"_DJB:
            case "Interface/ResearchMenu.swf"_DJB:
            case "Interface/WeaponCraftingMenu.swf"_DJB:
            case "Interface/ArmorCraftingMenu.swf"_DJB:
            case "Interface/IndustrialCraftingMenu.swf"_DJB:
            case "Interface/DrugsCraftingMenu.swf"_DJB:
            case "Interface/SpaceShipEditorMenu.swf"_DJB:
            case "Interface/ShipCrewMenu.swf"_DJB:
            case "Interface/SkillsMenu.swf"_DJB:
            case "Interface/MainMenu.swf"_DJB:
            case "Interface/CursorMenu.swf"_DJB:
            case "Interface/DataMenu.swf"_DJB:
            case "Interface/InventoryMenu.swf"_DJB:
            case "Interface/LoadingMenu.swf"_DJB:
            case "Interface/PauseMenu.swf"_DJB:
            case "Interface/GalaxyStarMapMenu.swf"_DJB:
            case "Interface/StarMapMenu.swf"_DJB:
                gStore.g_statefullData.renderData.rendered_menus_count[gStore.g_statefullData.renderData.modulino % 2]++;
                break;
            default:
                break;

        }
        gStore.debugData.ui_parts.push_back(menuNameHash);
    }

    inline bool shouldShowFlatScreen() {
        if(gStore.internalSettings.enforceFlatScreen) {
            return true;
        }

        return gStore.g_statefullData.renderData.rendered_menus_count[(gStore.g_statefullData.renderData.modulino + 1) % 2] >= 0;
    }
}

