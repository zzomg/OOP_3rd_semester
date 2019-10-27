#pragma once
#include "instruction_base.h"

class WriteFileInstr : public Instruction
{
	std::string filename_;

public:

	WriteFileInstr() {}
	WriteFileInstr(pair instr);
	virtual ~WriteFileInstr() {}

	virtual void execute(std::vector<std::string>& input);
};
