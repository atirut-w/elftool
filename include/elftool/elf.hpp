#pragma once
#include <istream>
#include <bit>
#include <cstdint>
#include <vector>
#include <elftool/section.hpp>
#include <ostream>

namespace ELFTool
{
    enum class Bitness
    {
        NONE,
        BITS32,
        BITS64
    };

    enum class Endianness
    {
        NONE,
        LSB,
        MSB
    };

    enum class ELFVersion
    {
        NONE,
        CURRENT
    };

    enum class ABI
    {
        SYSV,
        HPUX,
        NETBSD,
        LINUX,
        HURD,
        SOLARIS,
        AIX,
        IRIX,
        FREEBSD,
        TRU64,
        MODESTO,
        OPENBSD,
        OPENVMS,
        NSK,
        AROS,
        FENIXOS,
        CLOUDABI,
        OPENVOS
    };

    enum class ELFType
    {
        NONE,
        REL,
        EXEC,
        DYN,
        CORE
    };

    enum class MachineType
    {
        NONE,
        X86 = 0x03,
        AMD64 = 0x3e,
        Z80 = 0xdc,
    };

    class ELF
    {
    private:
        template <typename T>
        T read(std::istream &stream)
        {
            T value;

            // If the ELF file is not in native endianness, swap the bytes
            // TODO: Check if this actually works?
            if ((endianness == Endianness::LSB && std::endian::native == std::endian::big) || (endianness == Endianness::MSB && std::endian::native == std::endian::little))
            {
                for (size_t i = 0; i < sizeof(T); i++)
                {
                    reinterpret_cast<char *>(&value)[i] = stream.get();
                }
            }
            else
            {
                stream.read(reinterpret_cast<char *>(&value), sizeof(T));
            }

            return value;
        }

        template <typename T>
        void write(std::ostream &stream, T value)
        {
            // If the ELF file is not in native endianness, swap the bytes
            // TODO: Check if this actually works?
            if ((endianness == Endianness::LSB && std::endian::native == std::endian::big) || (endianness == Endianness::MSB && std::endian::native == std::endian::little))
            {
                for (size_t i = 0; i < sizeof(T); i++)
                {
                    stream.put(reinterpret_cast<char *>(&value)[i]);
                }
            }
            else
            {
                stream.write(reinterpret_cast<char *>(&value), sizeof(T));
            }
        }

        uint64_t read_address(std::istream &stream);
        void write_address(std::ostream &stream, uint64_t address);

    public:
        ELF(std::istream &stream);

        Bitness bitness = Bitness::NONE;
        Endianness endianness = Endianness::LSB;
        ELFVersion ei_version = ELFVersion::CURRENT;
        ABI abi = ABI::SYSV;
        int abi_version = 0;
        ELFType type = ELFType::NONE;
        MachineType machine = MachineType::NONE;
        int e_version = 1;
        uint64_t entry_point = 0;
        int flags = 0;
        int e_shstrndx = 0;

        std::vector<Section> sections;

        void write(std::ostream &stream);
    };
}
