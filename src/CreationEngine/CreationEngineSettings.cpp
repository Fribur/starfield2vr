#include "CreationEngineSettings.h"
#include "utility/RTTI.hpp"
#include <utility/Module.hpp>
#include "RE/I/INISettingCollection.h"

template<typename T>
uintptr_t find(const uintptr_t haystack, const uintptr_t endptr, uint64_t * const offset, const T needle) {
    const uintptr_t start(haystack + *offset);
    if (start >= endptr) return 0;
    const auto b = (const T*) start;
    const auto count = (unsigned)((endptr - start) / sizeof(T));
    for (unsigned i = 0; i < count; ++i) {
        if (b[i] == needle) {
            *offset += (i * sizeof(T));
            return *offset;
        }
    }
    return 0;
}

RE::Setting* CreationEngineSettings::get_setting(std::string_view id, CreationEngineSettings::SettingType type)
{
    auto scan_settings = [&](auto& vtable) {
        std::unordered_map<std::string, RE::Setting*> settings_map {};

        auto module = utility::get_executable();
        auto vt_ptr = utility::rtti::find_vtable(module, vtable);
        if (!vt_ptr) {
            spdlog::error("Failed to find vtable: {}", vtable);
            return settings_map;
        }
        spdlog::debug("Found vtable: {} -> {:x}", vtable, vt_ptr.value());
        auto begin = reinterpret_cast<uintptr_t>(module);
        auto end = begin + *utility::get_module_size(module);
        uintptr_t offset = 0;
        uintptr_t instance_ptr = *vt_ptr;
        while (find(begin, end, &offset, instance_ptr)) {
            auto setting = reinterpret_cast<RE::Setting*>(begin + offset);
            auto val = setting->to_string();
            spdlog::debug("[{}] ptr={}: {}={}", vtable, fmt::ptr(setting), setting->GetKey().data(), val.data());
            std::string key = setting->GetKey().data();
            settings_map[key] = setting;
            offset += sizeof(RE::Setting);
        }
        return settings_map;
    };
    switch (type) {
    case SettingType::kINISetting: {
        auto settings = RE::INISettingCollection::GetSingleton();
        return settings->GetSetting(id.data());
    }
    case SettingType::kINIPrefSetting: {
        static auto init_perf_settings = scan_settings(".?AVINIPrefSetting@@");
        return init_perf_settings[id.data()];
    }
//    case SettingType::kRegSetting: {
//        auto settings = RE::RegSettingCollection::GetSingleton();
//        return settings->GetSetting(id.data());
//    }
    case SettingType::kGameSetting: {
        static auto game_settings = scan_settings(".?AV?$SettingT@VGameSettingCollection@@@@");
        return game_settings[id.data()];
    }
    case SettingType::kRendererQualitySetting: {
        static auto renderer_quality_settings = scan_settings(".?AVRendererQualitySetting@CreationRenderer@@");
        return renderer_quality_settings[id.data()];
    }
    case SettingType::kRendererPerfSetting: {
        static auto renderer_quality_perf_settings = scan_settings(".?AVRendererQualityPref@CreationRenderer@@");
        return renderer_quality_perf_settings[id.data()];
    }
    default:
        return nullptr;

    }
}