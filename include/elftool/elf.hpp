#pragma once
#include <istream>

namespace ELFTool
{
    class ELF
    {
    public:
        ELF(std::istream& stream);
    };
}
