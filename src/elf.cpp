#include <elftool/elf.hpp>
#include <stdexcept>
#include <string>
#include <cstdint>

using namespace std;
using namespace ELFTool;

uint64_t ELF::read_address(istream& stream)
{
    if (ei_class == ELFClass::CLASS32)
    {
        return read<uint32_t>(stream);
    }
    else
    {
        return read<uint64_t>(stream);
    }
}

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
    ei_osabi = static_cast<ELFOSABI>(stream.get());
    ei_abiversion = stream.get();
    stream.ignore(7);

    e_type = static_cast<ELFType>(read<uint16_t>(stream));
    e_machine = static_cast<ELFMachine>(read<uint16_t>(stream));
    e_version = read<uint32_t>(stream);
    e_entry = read_address(stream);
    auto e_phoff = read_address(stream);
    auto e_shoff = read_address(stream);
    flags = read<uint32_t>(stream);
    e_ehsize = read<uint16_t>(stream);
    e_phentsize = read<uint16_t>(stream);
    e_phnum = read<uint16_t>(stream);
    e_shentsize = read<uint16_t>(stream);
    e_shnum = read<uint16_t>(stream);
    e_shstrndx = read<uint16_t>(stream);

    // TODO: Sanity checks
}
