#pragma once
#include "instruction_base.h"

class ReadFileInstr : public Instruction
{
	std::string filename_;

public:

	ReadFileInstr() {}
	ReadFileInstr(pair instr);
	virtual ~ReadFileInstr() {}

	virtual void execute(std::vector<std::string>& input);
};
