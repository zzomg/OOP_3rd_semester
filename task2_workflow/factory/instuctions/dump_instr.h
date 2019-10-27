#pragma once
#include "instruction_base.h"

class DumpInstr : public Instruction
{
	std::string filename_;

public:

	DumpInstr() {}
	DumpInstr(pair instr);
	virtual ~DumpInstr() {}

	virtual void execute(std::vector<std::string>& input);
};
