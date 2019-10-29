#include "dump_instr.h"

DumpInstr::DumpInstr(pair instr)
{
	if (instr.second.size() != 1) { 
		throw std::exception("DUMP must receive 1 argument\n");
	}
	filename_ = instr.second[0];
}

void DumpInstr::execute(std::vector<std::string>& input)
{
	std::ofstream dumpFile(filename_);

	for (const auto& item : input) {
		dumpFile << item << "\n";
	}
}
