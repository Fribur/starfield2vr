#pragma once
#include <RE/N/NiAVObject.h>
#include <RE/P/PlayerCharacter.h>
#include <utils/FunctionHook.h>

class CreationEngineWeaponModule
{
public:
    inline static std::shared_ptr<CreationEngineWeaponModule>& Get()
    {
        static std::shared_ptr<CreationEngineWeaponModule> instance{ std::make_shared<CreationEngineWeaponModule>() };
        return instance;
    }
    inline CreationEngineWeaponModule() {
        install_hooks();
    }
    ~CreationEngineWeaponModule() = default;
private:
    void install_hooks();

    static uintptr_t onUpdateWorld(RE::NiAVObject* a_object, RE::NiUpdateData* a_data);
    static char shouldCastFromMuzzle(RE::EquippedWeapon::Data* a_equippedWeapon, RE::PlayerCharacter* a_player);
    // 0x5507648
    static char getEquippedWeapon(RE::PlayerCharacter* a_player, unsigned int slot, RE::EquippedItem* a_equippedItem);

    std::unique_ptr<FunctionHook> m_onUpdateWorldHook{};
    std::unique_ptr<FunctionHook> m_shouldCastFromMuzzleHook{};
    std::unique_ptr<FunctionHook> m_getEquippedWeaponHook{};

};