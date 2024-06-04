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
}
