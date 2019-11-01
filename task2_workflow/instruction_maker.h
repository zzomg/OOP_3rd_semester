#include "instruction_base.h"

class InstructionMakerBase
{
public:
	virtual Instruction * Create(const std::vector<std::string> &args) const = 0;
	virtual ~InstructionMakerBase() {}
};

template<typename T>
class InstructionMaker : public InstructionMakerBase
{
public:
	InstructionMaker(const std::string& key)
	{
		InstrFactory::instance().RegisterMaker(key, this);
	}

	virtual Instruction * Create(const std::vector<std::string> &args) const
	{
		return new T(node);
	}
};
