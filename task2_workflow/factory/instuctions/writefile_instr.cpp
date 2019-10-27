#include "writefile_instr.h"

WriteFileInstr::WriteFileInstr(pair instr)
{
	if (instr.second.size() != 1) { 
		throw std::exception("WRITEFILE must receive 1 argument\n");
	}
	filename_ = instr.second[0];
}

void WriteFileInstr::execute(std::vector<std::string>& input)
{
	std::ofstream outputFile(filename_);

	for (const auto& item : input) {
		outputFile << item << "\n";
	}
}
