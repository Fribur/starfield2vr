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
        {  885866, 0x6A50060, 0x6A8C0A0 },
        {  885862, 0x6A50040, 0x6A8C080 },
        {  210419, 0x37EA208, 0x3858468 },
        {  210404, 0x37E902C, 0x385728C },
        {  210425, 0x37EA3F4, 0x3858654 },
        {  210234, 0x37DE584, 0x384C7E4 },
        {  179249, 0x2F7AF34, 0x2F6EA94 },
        {   50603, 0x0C6E5F0, 0x0C6FE10 },
        { 1079045, 0x36A227C, 0x371045C },
        {  202136, 0x35529A0, 0x35C0B70 },
        { 1079146, 0x37007CC, 0x376EA3C },
        {  206429, 0x36BEC7C, 0x372CE5C },
        {  885900, 0x6A50260, 0x6A8C2B0 },
        {  204129, 0x361646C, 0x36846FC },
        {  208939, 0x37B4FBC, 0x382321C },
        {  878523, 0x6A2E350, 0x6A6A150 },
        { 1171973, 0x6A524D0, 0x6A8E530 },
        {  772639, 0x6134798, 0x616B7A8 },
        {  886292, 0x6A5421C, 0x6A90294 },
        { 1171750, 0x66A9018, 0x66E4E58 },
        {  878340, 0x682A940, 0x6866740 },
        {  886000, 0x6A524A0, 0x6A8E500 },
        {  885886, 0x6A501F8, 0x6A8C220 },
        {  873288, 0x6712878, 0x674E738 },
        {  887308, 0x6A71CF0, 0x6AADB30 },
        {  865059, 0x66C53F8, 0x6701238 },
        {  166081, 0x2ACF930, 0x2AC6570 },
        {  865059, 0x66C53F8, 0x6701238 },
        { 1391721, 0x3582490, 0x35F06A0 },
        {  428853, 0x55454F8, 0x556CA28 },
        {  165928, 0x2ac7918, 0x2ABE558 }
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
