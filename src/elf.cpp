/*
    This file is part of rp++.

    Copyright (C) 2014, Axel "0vercl0k" Souchet <0vercl0k at tuxfamily.org>
    All rights reserved.

    rp++ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    rp++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with rp++.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "elf.hpp"

#include "x86.hpp"
#include "x64.hpp"
#include "arm.hpp"

#include <iostream>

std::string Elf::get_class_name(void) const
{
    return "Elf";
}

void Elf::display_information(const VerbosityLevel lvl) const
{
    ExecutableFormat::display_information(lvl);
    std::cout << "Elf Information:" << std::endl;
    m_ELFLayout->display(lvl);
}

CPU::E_CPU Elf::extract_information_from_binary(std::ifstream &file)
{
	uint32_t size_init = 0;
    std::array<uint8_t, EI_NIDENT> buf;
    CPU::E_CPU cpu = CPU::CPU_UNKNOWN;
    std::cout << "Loading ELF information.." << std::endl;

    /* Remember where the caller was in the file */
    std::streampos off = file.tellg();

    file.seekg(0, std::ios::beg);
    file.read((char*)buf.data(), EI_NIDENT);

    unsigned short type = 0;

	{
        auto const os = buf.at(EI_OSABI);
        file.read((char*)&type, 2);


	    int aaa = 5;
	}

    switch(buf.at(EI_CLASS))
    {
        case ELFCLASS32:
        {
			size_init = 4;
			break;
        }

        case ELFCLASS64:
        {
            size_init = 8;
            break;
        }

        default:
            RAISE_EXCEPTION("Cannot determine the architecture size");
    }

	if(size_init == 8)
		init_properly_ELFLayout<x64Version>();
	else
		init_properly_ELFLayout<x86Version>();

    /* Filling the structure now !*/
    m_ELFLayout->fill_structures(file);

    /* Set correctly the pointer */
    file.seekg(off);

	switch(m_ELFLayout->get_cpu())
	{
		case RP_ELFEM_386:
		{
			cpu = CPU::CPU_x86;
			break;
		}

		case RP_ELFEM_X86_64:
		{
			cpu = CPU::CPU_x64;
			break;
		}

		case RP_ELFEM_ARM:
		{
			cpu = CPU::CPU_ARM;
			break;
		}

		default:
			RAISE_EXCEPTION("Cannot determine the CPU type");
	}

    return cpu;
}

std::shared_ptr<CPU> Elf::get_cpu(std::ifstream &file)
{
    std::shared_ptr<CPU> cpu { nullptr };
    CPU::E_CPU cpu_type { CPU::CPU_UNKNOWN };

    cpu_type = extract_information_from_binary(file);

    switch(cpu_type)
    {
        case CPU::CPU_x86:
        {
            cpu = std::make_shared<x86>();
            break;
        }

        case CPU::CPU_x64:
        {
            cpu = std::make_shared<x64>();
            break;
        }

		case CPU::CPU_ARM:
		{
			cpu = std::make_shared<ARM>();
			break;
		}

        default:
            RAISE_EXCEPTION("Cannot determine the CPU type");
    }
    
    if(cpu == nullptr)
        RAISE_EXCEPTION("Cannot allocate a cpu");

    return cpu;
}

std::vector<std::shared_ptr<Section>> Elf::get_executables_section(std::ifstream & file) const
{
    return m_ELFLayout->get_executable_section(file);
}

uint64_t Elf::get_image_base_address(void) const
{
    return m_ELFLayout->get_image_base_address();
}