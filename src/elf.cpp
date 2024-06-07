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

    if (e_phoff != 0)
    {
        vector<uint64_t> segment_data_offsets;

        stream.seekg(e_phoff);
        for (int i = 0; i < e_phnum; i++)
        {
            Segment segment;
            segment.type = static_cast<SegmentType>(read<uint32_t>(stream));
            if (bitness == Bitness::BITS64)
            {
                segment.flags = read<uint32_t>(stream);
            }
            auto p_offset = read_address(stream);
            segment.virtual_address = read_address(stream);
            segment.physical_address = read_address(stream);
            auto p_filesz = read_address(stream);
            auto p_memsz = read_address(stream); // TODO: Skip?
            if (bitness == Bitness::BITS32)
            {
                segment.flags = read<uint32_t>(stream);
            }
            segment.alignment = read_address(stream);

            segment.data.resize(p_filesz);
            segment_data_offsets.push_back(p_offset);
        }

        for (int i = 0; i < e_phnum; i++)
        {
            stream.seekg(segment_data_offsets[i]);
            stream.read(reinterpret_cast<char *>(&segments[i].data[0]), segments[i].data.size());
        }
    }
}
