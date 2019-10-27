#pragma once
#include "instruction_base.h"

class ReplaceInstr : public Instruction
{
	std::string word1_;
	std::string word2_;

public:

	ReplaceInstr() {}
	ReplaceInstr(pair instr);
	virtual ~ReplaceInstr() {}

	virtual void execute(std::vector<std::string>& input);
};
