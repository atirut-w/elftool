#pragma once
#include <cstdint>
#include <vector>

namespace ELFTool
{
    enum class SegmentType
    {
        NONE,
        LOAD,
        DYNAMIC,
        INTERP,
        NOTE,
        SHLIB,
        PHDR,
    };

    enum SegmentFlags
    {
        PF_X = 0x1,
        PF_W = 0x2,
        PF_R = 0x4,
    };

    struct Segment
    {
        SegmentType type;
        uint64_t flags;
        uint64_t virtual_address;
        uint64_t physical_address;
        std::vector<uint8_t> data;
        uint64_t alignment;
    };
}
