#pragma once
#include "instruction_base.h"
#include <algorithm>

class SortInstr : public Instruction
{
public:

	SortInstr() {}
	SortInstr(pair instr);
	virtual ~SortInstr() {}

	virtual void execute(std::vector<std::string>& input);
};
