#pragma once
#include "instruction_base.h"
#include "grep_instr.h"
#include "readfile_instr.h"
#include "writefile_instr.h"
#include "sort_instr.h"
#include "replace_instr.h"
#include "dump_instr.h"

class InstrFactory
{
	InstrFactory() {};
	~InstrFactory() {};

	InstrFactory(InstrFactory const&) = delete;
	InstrFactory& operator= (InstrFactory const&) = delete;

public:

	static InstrFactory& instance()
	{
		static InstrFactory factory;
		return factory;
	}

	Instruction* Create(pair instr) const;
};
