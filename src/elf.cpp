#include <elftool/elf.hpp>
#include <stdexcept>
#include <string>
#include <cstdint>

using namespace std;
using namespace ELFTool;

uint64_t ELF::read_address(istream &stream)
{
    if (bitness == Bitness::BITS32)
    {
        return read<uint32_t>(stream);
    }
    else
    {
        return read<uint64_t>(stream);
    }
}

ELF::ELF(istream &stream)
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

    bitness = static_cast<Bitness>(stream.get());
    endianness = static_cast<Endianness>(stream.get());
    ei_version = static_cast<ELFVersion>(stream.get());
    abi = static_cast<ABI>(stream.get());
    abi_version = stream.get();
    stream.ignore(7);

    type = static_cast<ELFType>(read<uint16_t>(stream));
    machine = static_cast<MachineType>(read<uint16_t>(stream));
    e_version = read<uint32_t>(stream);
    entry_point = read_address(stream);
    auto e_phoff = read_address(stream);
    auto e_shoff = read_address(stream);
    flags = read<uint32_t>(stream);
    auto e_ehsize = read<uint16_t>(stream);
    auto e_phentsize = read<uint16_t>(stream);
    auto e_phnum = read<uint16_t>(stream);
    auto e_shentsize = read<uint16_t>(stream);
    auto e_shnum = read<uint16_t>(stream);
    auto e_shstrndx = read<uint16_t>(stream);

    // TODO: Sanity checks
}
