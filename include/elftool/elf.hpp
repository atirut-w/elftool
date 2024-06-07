#pragma once
#include <istream>
#include <bit>
#include <cstdint>

namespace ELFTool
{
    enum class ELFClass
    {
        NONE,
        CLASS32,
        CLASS64
    };

    enum class ELFFormat
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

    enum class ELFOSABI
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

    enum class ELFMachine
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
            if ((ei_data == ELFFormat::LSB && std::endian::native == std::endian::big) || (ei_data == ELFFormat::MSB && std::endian::native == std::endian::little))
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

        ELFClass ei_class;
        ELFFormat ei_data;
        ELFVersion ei_version;
        ELFOSABI ei_osabi;
        int ei_abiversion;

        ELFType e_type;
        ELFMachine e_machine;
        int e_version;
        uint64_t e_entry;
        int flags;
    };
}
