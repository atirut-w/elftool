#pragma once
#include <istream>

namespace ELFTool
{
    enum class ELFClass
    {
        ELFCLASSNONE = 0,
        ELFCLASS32 = 1,
        ELFCLASS64 = 2
    };

    enum class ELFFormat
    {
        ELFDATANONE = 0,
        ELFDATA2LSB = 1,
        ELFDATA2MSB = 2
    };

    enum class ELFVersion
    {
        EV_NONE = 0,
        EV_CURRENT = 1
    };
    
    class ELF
    {
    public:
        ELF(std::istream& stream);

        ELFClass ei_class;
        ELFFormat ei_data;
        ELFVersion ei_version;
    };
}
