#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>
#include <fstream>

#include "cpu.hpp"
#include "executable_format.hpp"

class Program
{
	public:
        explicit Program(const std::string & program_path);
		~Program(void);

        void display_information(VerbosityLevel lvl = VERBOSE_LEVEL_1);

    private:
        CPU* m_cpu;
        ExecutableFormat* m_exformat;
        std::ifstream m_file;
};

#endif