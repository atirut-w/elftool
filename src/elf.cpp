#include <elftool/elf.hpp>
#include <stdexcept>
#include <string>

using namespace std;
using namespace ELFTool;

ELF::ELF(istream& stream)
{
    if (!stream)
    {
        throw invalid_argument("invalid input stream");
    }

    string magic;
    magic.resize(4);
    stream.read(&magic[0], 4);
    if (magic != "\x7f""ELF")
    {
        throw invalid_argument("invalid ELF magic");
    }

    ei_class = static_cast<ELFClass>(stream.get());
    ei_data = static_cast<ELFFormat>(stream.get());
    ei_version = static_cast<ELFVersion>(stream.get());
}
