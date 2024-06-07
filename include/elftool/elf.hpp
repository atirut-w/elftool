#pragma once
#include <istream>
#include <bit>
#include <cstdint>

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
        T read(std::istream& stream)
        {
            T value;
            
            // If the ELF file is not in native endianness, swap the bytes
            if ((endianness == Endianness::LSB && std::endian::native == std::endian::big) || (endianness == Endianness::MSB && std::endian::native == std::endian::little))
            {
                for (size_t i = 0; i < sizeof(T); i++)
                {
                    reinterpret_cast<char*>(&value)[i] = stream.get();
                }
            }
            else
            {
                stream.read(reinterpret_cast<char*>(&value), sizeof(T));
            }

            return value;
        }

        uint64_t read_address(std::istream& stream);

    public:
        ELF(std::istream& stream);

        Bitness bitness;
        Endianness endianness;
        ELFVersion ei_version;
        ABI abi;
        int abi_version;

        ELFType type;
        MachineType machine;
        int e_version;
        uint64_t entry_point;
        int flags;
    };
}
