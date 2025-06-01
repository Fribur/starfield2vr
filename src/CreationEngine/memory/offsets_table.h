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
        {  147392, 0x2b65900, 0x3858468 },
        {  147416, 0x2b68830, 0x385728C },
        {  470133, 0x4CAEC98, 0x2F6EA94 },
        {  303817, 0x3b2cee8, 0x0C6FE10 },
        {  143812, 0x29D5B30, 0x371045C },
        {  145355, 0x2A81CE0, 0x376EA3C },
        {  144161, 0x2A009B0, 0x372CE5C },
        {  883900, 0x5D56484, 0x6A8C2B0 },
        {  142800, 0x29802D0, 0x36846FC },
        {  497712, 0x4E37C78, 0x382321C },
        {  937788, 0x60b9c70, 0x6A6A150 },
        {  944432, 0x60D7508, 0x6A8E530 },
        {  937583, 0x5EB6390, 0x6866740 },
        {  944397, 0x60D73D8, 0x6A8C220 },
        {  936470, 0x5E8D158, 0x6AADB30 },
        {  922868, 0x5E200A0, 0x6701238 },
        {  141825, 0x290e010, 0x35F06A0 },
        {  459617, 0x4c57c88, 0x556CA28 },
        {  472039, 0x4cbd600, 0x55D4A88 }
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
