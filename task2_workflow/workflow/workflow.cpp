#include "workflow.h"
#include "parser.h"
#include "instr_factory.h"

void Workflow::run(const std::string& workflowFilePath)
{
	std::ifstream workflowFile(workflowFilePath);
	if (!workflowFile) {
		throw std::exception("Workflow file opening failed\n");
	}

	Parser parser;
	parser.parseWFFile(workflowFile);

	InstrFactory& factory = InstrFactory::instance();

	std::vector<int>::iterator it;
	for (it = parser.instructions_order_.begin(); it != parser.instructions_order_.end(); ++it) {
		if (it == parser.instructions_order_.begin() &&
			parser.instructions_[*it].first != "readfile") {
			throw std::exception("Workflow must start with \"readfile\"\n");
		}
		if (it == parser.instructions_order_.end() - 1 &&
			parser.instructions_[*it].first != "writefile") {
			throw std::exception("Workflow must end with \"writefile\"\n");
		}
		if (it != parser.instructions_order_.begin() &&
			it != parser.instructions_order_.end() - 1 &&
			(parser.instructions_[*it].first == "writefile" ||
				parser.instructions_[*it].first != "readfile")) {
			throw std::exception("\"Readfile\" and \"writefile\" can not be executed more than once\n");
		}
		Instruction* instr = factory.Create(parser.instructions_[*it]);
		instr->execute(input);
		delete instr;
	}
}
