#pragma once
#include <utility/Module.hpp>
#include <utility/Scan.hpp>

namespace MemoryScan {
    const auto mod      = utility::get_executable();
    const size_t mod_size = utility::get_module_size(mod).value_or(0);
    const auto mod_end  = (uintptr_t)mod + mod_size - 0x100;

    inline uintptr_t FuncRelocation(const char* pattern, uintptr_t static_offset = 0 , uintptr_t a_id = 0) {
        uintptr_t val = 0;
#ifdef _DEBUG
        auto ref = utility::scan(mod, pattern);
        if (!ref) {
            spdlog::error("FuncRelocation pattern not found for id={}", a_id);
            return 0;
        }
        val = ref.value();
        if(static_offset > 0) {
            auto offset = val - (uintptr_t)mod;
            if(offset != static_offset) {
                spdlog::info("FuncRelocation offset does not match static offset for id={}", a_id);
            }
        }
#else
        if(static_offset > 0) {
            val =  static_offset + (uintptr_t)mod;
        } else {
            spdlog::error("no offset is provided id={} ", a_id);
        }
#endif
#ifndef USE_STARFIELD_SDK_LITE
        if(a_id > 0) {
            auto offset = val - (uintptr_t)mod;
            uintptr_t library_offset = REL::ID{ a_id }.offset();
            if(offset != library_offset) {
                spdlog::info("FuncRelocation offset does not match offset by ID for id={} scan={:x} lib={:x}", a_id, offset, library_offset);
            }
        }
#endif
        return val;
    };
    inline uintptr_t InstructionRelocation(const char* pattern, UINT offset_begin, UINT instruction_size,  uintptr_t static_offset = 0 , uintptr_t a_id = 0) {
        uintptr_t val = 0;
#ifdef _DEBUG
        auto ref = utility::scan(mod, pattern);
        if (!ref) {
            spdlog::error("AsmCodeRelocation pattern not found for id={}", a_id);
            return 0;
        }
        auto addr = ref.value();
        val = addr + *(int32_t*)(addr + offset_begin) + instruction_size;
        if(static_offset > 0) {
            auto offset = val - (uintptr_t)mod;
            if(offset != static_offset) {
                spdlog::info("AsmCodeRelocation offset does not match static offset for id={}", a_id);
            }
        }
#else
        if(static_offset > 0) {
            val = static_offset + (uintptr_t)mod;
        } else {
            spdlog::error("no offset is provided id={} ", a_id);
        }
#endif
#ifndef USE_STARFIELD_SDK_LITE
        if(a_id > 0) {
            auto offset = val - (uintptr_t)mod;
            uintptr_t library_offset = REL::ID{ a_id }.offset();
            if(offset != library_offset) {
                spdlog::info("AsmCodeRelocation offset does not match offset by ID for id={} scan={:x} lib={:x}", a_id, offset, library_offset);
            }
        }
#endif
        return val;
    };
}