#include "readfile_instr.h"

ReadFileInstr::ReadFileInstr(pair instr)
{
	if (instr.second.size() != 1) { 
		throw std::exception("READFILE must receive 1 argument\n");
	}
	filename_ = instr.second[0];
}

void ReadFileInstr::execute(std::vector<std::string>& input)
{
	std::ifstream inputFile(filename_);
	if (!inputFile) {
		throw std::exception("Input file opening failed\n");
	}

	std::string line;
	while (std::getline(inputFile, line, '\n')) {
		input.push_back(line);
	}
}
