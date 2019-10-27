#include "instr_factory.h"

Instruction* InstrFactory::Create(pair instr) const
{
	if (instr.first == "grep") return new GrepInstr(instr);
	else if (instr.first == "readfile") return new ReadFileInstr(instr);
	else if (instr.first == "writefile") return new WriteFileInstr(instr);
	else if (instr.first == "sort") return new SortInstr(instr);
	else if (instr.first == "replace") return new ReplaceInstr(instr);
	else if (instr.first == "dump") return new DumpInstr(instr);
	else throw std::exception("Unrecognized instruction type\n");
}
