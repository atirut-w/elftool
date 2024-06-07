#pragma once
#include <cstdint>
#include <vector>

namespace ELFTool
{
    enum class SectionType
    {
        NONE = 0x00,
        PROGBITS = 0x01,
        SYMTAB = 0x02,
        STRTAB = 0x03,
        RELA = 0x04,
        HASH = 0x05,
        DYNAMIC = 0x06,
        NOTE = 0x07,
        NOBITS = 0x08,
        REL = 0x09,
        SHLIB = 0x0a,
        DYNSYM = 0x0b,
        INIT_ARRAY = 0x0e,
        FINI_ARRAY = 0x0f,
        PREINIT_ARRAY = 0x10,
        GROUP = 0x11,
        SYMTAB_SHNDX = 0x12,
        NUM = 0x13,
    };

    enum SectionFlags
    {
        SHF_WRITE = 0x0001,
        SHF_ALLOC = 0x0002,
        SHF_EXECINSTR = 0x0004,
        SHF_MERGE = 0x0010,
        SHF_STRINGS = 0x0020,
        SHF_INFO_LINK = 0x0040,
        SHF_LINK_ORDER = 0x0080,
        SHF_OS_NONCONFORMING = 0x0100,
        SHF_GROUP = 0x0200,
        SHF_TLS = 0x400,
    };

    struct Section
    {
        int name_index;
        SectionType type;
        uint64_t flags;
        uint64_t address;
        std::vector<uint8_t> data;
        int link;
        int info;
        uint64_t alignment;
        uint64_t entry_size;
    };
}
