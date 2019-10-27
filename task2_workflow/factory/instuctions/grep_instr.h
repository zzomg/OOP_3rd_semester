#pragma once
#include "instruction_base.h"

class GrepInstr : public Instruction
{
	std::string word_;

public:

	GrepInstr() {}
	GrepInstr(pair instr);
	virtual ~GrepInstr() {}

	virtual void execute(std::vector<std::string>& input);
};
