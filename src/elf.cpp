#include <elftool/elf.hpp>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <algorithm>
#include <map>

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

void ELF::write_address(ostream &stream, uint64_t address)
{
    if (bitness == Bitness::BITS32)
    {
        write<uint32_t>(stream, address);
    }
    else
    {
        write<uint64_t>(stream, address);
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
    e_shstrndx = read<uint16_t>(stream);

    // TODO: Sanity checks
    // TODO: Load program headers

    if (e_shoff != 0)
    {
        vector<uint64_t> section_data_offsets;

        stream.seekg(e_shoff);
        for (int i = 0; i < e_shnum; i++)
        {
            Section section;
            section.name_index = read<uint32_t>(stream);
            section.type = static_cast<SectionType>(read<uint32_t>(stream));
            section.flags = read_address(stream);
            section.address = read_address(stream);
            section_data_offsets.push_back(read_address(stream));
            auto data_size = read_address(stream);
            section.link = read<uint32_t>(stream);
            section.info = read<uint32_t>(stream);
            section.alignment = read_address(stream);
            section.entry_size = read_address(stream);

            section.data.resize(data_size);
            sections.push_back(section);
        }

        for (int i = 0; i < e_shnum; i++)
        {
            stream.seekg(section_data_offsets[i]);
            stream.read(reinterpret_cast<char *>(&sections[i].data[0]), sections[i].data.size());
        }
    }
}

void ELF::write_elf(ostream &stream)
{
    if (!stream)
    {
        throw invalid_argument("invalid output stream");
    }
    
    stream.write("\x7f""ELF", 4);
    write<uint8_t>(stream, static_cast<uint8_t>(bitness));
    write<uint8_t>(stream, static_cast<uint8_t>(endianness));
    write<uint8_t>(stream, static_cast<uint8_t>(ei_version));
    write<uint8_t>(stream, static_cast<uint8_t>(abi));
    write<uint8_t>(stream, abi_version);
    stream.write("\0\0\0\0\0\0\0", 7);

    write<uint16_t>(stream, static_cast<uint16_t>(type));
    write<uint16_t>(stream, static_cast<uint16_t>(machine));
    write<uint32_t>(stream, e_version);
    write_address(stream, entry_point);
    auto e_phoff_offset = stream.tellp();
    write_address(stream, 0);
    auto e_shoff_offset = stream.tellp();
    write_address(stream, 0);
    write<uint32_t>(stream, flags);

    if (bitness == Bitness::BITS32) // e_ehsize
    {
        write<uint16_t>(stream, 52);
    }
    else
    {
        write<uint16_t>(stream, 64);
    }
    
    write<uint16_t>(stream, 0); // TODO: Segment entry size
    write<uint16_t>(stream, 0); // TODO: Segment count

    if (bitness == Bitness::BITS32)
    {
        write<uint16_t>(stream, 40);
    }
    else
    {
        write<uint16_t>(stream, 64);
    }

    write<uint16_t>(stream, sections.size());
    write<uint16_t>(stream, e_shstrndx);

    // TODO: Write segments

    auto e_shoff = stream.tellp();
    stream.seekp(e_shoff_offset);
    write_address(stream, e_shoff);
    stream.seekp(e_shoff);

    vector<int> section_data_refs;
    for (auto &section : sections)
    {
        write<uint32_t>(stream, section.name_index);
        write<uint32_t>(stream, static_cast<uint32_t>(section.type));
        write_address(stream, section.flags);
        write_address(stream, section.address);
        section_data_refs.push_back(stream.tellp());
        write_address(stream, 0); // Data offset
        write_address(stream, section.data.size());
        write<uint32_t>(stream, section.link);
        write<uint32_t>(stream, section.info);
        write_address(stream, section.alignment);
        write_address(stream, section.entry_size);
    }

    // Write section data
    for (int i = 0; i < sections.size(); i++)
    {
        auto ref_offset = section_data_refs[i];
        auto data_offset = stream.tellp();
        stream.seekp(ref_offset);
        write_address(stream, data_offset);
        stream.seekp(data_offset);
        stream.write(reinterpret_cast<char *>(&sections[i].data[0]), sections[i].data.size());
    }
}
