#pragma once

namespace OffsetsTable
{
    struct OffsetMapping
    {
        int       ID;
        uintptr_t STEAM_OFFSET;
        uintptr_t XBOX_OFFSET;
    };

    inline OffsetMapping offsetMappings[] = {
        {  885866, 0x6A4FF00, 0x6A8C080 },
        {  885862, 0x6A4FEE0, 0x6A8C060 },
        {  210419, 0x37E9DC8, 0x3858058 },
        {  210404, 0x37E8BEC, 0x3856E7C },
        {  210425, 0x37E9FB4, 0x3858244 },
        {  210234, 0x37DE144, 0x384C3D4 },
        {  179249, 0x2F7ABA4, 0x2F6E704 },
        {   50603, 0x0C6E590, 0x0C6FDB0 },
        { 1079045, 0x36A1E3C, 0x371004C },
        {  202136, 0x3552560, 0x35C0760 },
        { 1079146, 0x370038C, 0x376E62C },
        {  206429, 0x36BE83C, 0x372CA4C },
        {  885900, 0x6A50108, 0x6A8C288 },
        {  204129, 0x361602C, 0x36842EC },
        {  208939, 0x37B4B7C, 0x3822E0C },
        {  878523, 0x6A2E148, 0x6A6A090 },
        { 1171973, 0x6A52388, 0x6A8E4F8 },
        {  772639, 0x6134828, 0x616B748 },
        {  886292, 0x6A540E0, 0x6A90254 },
        { 1171750, 0x66A8E58, 0x66E4D98 },
        {  878340, 0x682A770, 0x6866680 },
        {  886000, 0x6A52358, 0x6A8E4D0 },
        {  885886, 0x6A50080, 0x6A8C218 },
        {  873288, 0x67126B8, 0x674E678 },
        {  887308, 0x6A71BB0, 0x6AADAF0 },
        {  865059, 0x66C5238, 0x6701178 },
        {  166081, 0x2ACF680, 0x2AC62E0 },
        {  865059, 0x66C5238, 0x6701178 },
    };

    inline std::unordered_map<int, OffsetMapping> offsetMap = [] {
        std::unordered_map<int, OffsetMapping> map;
        for (const auto& mapping : offsetMappings) {
            map[mapping.ID] = mapping;
        }
        return map;
    }();

    inline uintptr_t GetOffset(int ID)
    {
        auto offset_map = offsetMap[ID];
        auto offset     = offset_map.STEAM_OFFSET;

#ifdef XBOX_STORE
        offset = offset_map.XBOX_OFFSET;
#endif

#ifndef USE_STARFIELD_SDK_LITE
        uintptr_t library_offset = REL::ID{ a_id }.offset();
        if (offset != library_offset) {
            spdlog::error("Offset does not match offset by ID for id={} scan={:x} lib={:x}", a_id, offset, library_offset);
        }
    }
#endif
    return offset;
}
} // namespace MemoryOffsets
