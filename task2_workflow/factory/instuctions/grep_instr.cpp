#include "grep_instr.h"

// "Grep" instruction constructor
GrepInstr::GrepInstr(pair instr)
{
	if (instr.second.size() != 1) { //takes solely 1 argument
		throw std::exception("GREP must receive 1 argument\n");
	}
	word_ = instr.second[0];
}

// "Grep" instruction implementation
void GrepInstr::execute(std::vector<std::string>& input)
{
	if (input.empty()) {
		throw std::exception("Input file is empty\n");
	}

	for (int i = input.size() - 1; i >= 0; --i) {
		if (input[i].find(word_) == std::string::npos) {
			input.erase(input.begin() + i);
		}
	}
}
