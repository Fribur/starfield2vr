#pragma once
#include "glm/glm.hpp"

namespace GameFlow {
    struct State
    {
        struct UiData {
            int rendered_menus_count[2]{};
            int modulino{0};
        } uiData{};

        struct RayCastData {
            glm::vec3 weaponRayCastOrigin{};
            glm::vec3 weaponRayCastDestination{};
        } rayCastData{};
    };

    extern State gState;

    void renderMenu(std::string_view menuNameHash);
    bool shouldShowFlatScreen();
    bool isAimingDownSights();
    bool isWeaponDrawn();
    bool isImmovable();
    bool isControlledByAI();
    bool isInFirstPerson();
    void resetGameState();
}

