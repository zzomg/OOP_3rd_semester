#include "sort_instr.h"
#include "instruction_maker.h"

static InstructionMaker<SortInstr> sortInstrMaker("sort");

SortInstr::SortInstr(pair instr)
{
	if (instr.second.size() != 0) {
		throw std::exception("SORT must not receive any arguments\n");
	}
}

void SortInstr::execute(std::vector<std::string>& input)
{
	std::sort(input.begin(), input.end(),
		[](const std::string& l, const std::string& r) {
			return l < r;
		});
}
