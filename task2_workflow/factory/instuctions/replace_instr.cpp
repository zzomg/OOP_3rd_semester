#include "replace_instr.h"

ReplaceInstr::ReplaceInstr(pair instr)
{
	if (instr.second.size() != 2) { 
		throw std::exception("REPLACE must receive 2 arguments\n");
	}
	word1_ = instr.second[0];
	word2_ = instr.second[1];
}

void ReplaceInstr::execute(std::vector<std::string>& input)
{
	std::string::size_type word1_pos;
	for (std::string& line : input) {
		while ((word1_pos = line.find(word1_)) != std::string::npos) {
			line.replace(word1_pos, word1_.length(), word2_);
		}
	}
}
