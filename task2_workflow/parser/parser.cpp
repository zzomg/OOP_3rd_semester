#include "parser.h"

namespace 
{
	bool containDigits(const std::string& s)
	{
		return std::any_of(s.begin(), s.end(), ::isdigit);
	}
}

// Parses the workflow file and checks if it's correct
void Parser::parseWFFile(std::ifstream& workflowFile)
{
	std::string line;
	int instrID;

	syntaxPrecheck(workflowFile);

	std::getline(workflowFile, line, '\n'); //ignore 'desc'

	//reading until 'csed', ignore 'csed'
	while (std::getline(workflowFile, line, '\n') && line != "csed") { 
		instrID = getInstrID(line);
		if (instructions_.find(instrID) != instructions_.end()) {
			throw std::exception("Instruction ID must be a unique number\n");
		}
		instructions_[instrID] = getInstrSignature(line);
	}

	getInstrOrder(workflowFile);
}

// Gets the order in which instructions are executed
void Parser::getInstrOrder(std::ifstream& workflowFile)
{
	size_t pos_start = 0;
	size_t pos_end;

	std::string delim = "->";
	size_t delim_len = delim.length();

	std::string line;
	std::string token;

	while (std::getline(workflowFile, line, '\n')) {
		while ((pos_end = line.find(delim, pos_start)) != std::string::npos) {
			token = line.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			if (!containDigits(token)) {
				throw std::exception("Wrong syntax : Incorrect instruction order\n");
			}
			instructions_order_.push_back(std::stoi(token));
		}
	}
	instructions_order_.push_back(std::stoi(line.substr(pos_start)));
}

// Checks overall syntax correctness :
// whether workflow starts with 'desc' & ends with 'csed'
void Parser::syntaxPrecheck(std::ifstream& workflowFile) const
{
	std::string::size_type desc = std::string::npos;
	std::string::size_type csed = std::string::npos;
	std::string line;

	while (std::getline(workflowFile, line, '\n')) {
		if (line.find("desc") != std::string::npos) {
			desc = line.find("desc");
		}
		if (line.find("csed") != std::string::npos) {
			csed = line.find("csed");
		}
	}

	if (desc == std::string::npos) {
		throw std::exception("Wrong syntax : Workflow must start with \"desc\"\n");
	}
	if (csed == std::string::npos) {
		throw std::exception("Wrong syntax : Workflow must end with \"csed\"\n");
	}
	workflowFile.clear();
	workflowFile.seekg(0, std::ios::beg);
}

// Gets instruction ID if it's initialized correctly
int Parser::getInstrID(std::string& line)
{
	std::string::size_type id_end_pos = line.find("=");
	std::string id;

	if (id_end_pos == std::string::npos) {
		throw std::exception("Wrong syntax : Instruction initialization must start with \"=\"\n");
	}
	id = line.substr(0, id_end_pos);
	if (!containDigits(id)) {
		throw std::exception("Wrong syntax : Instruction ID not found\n");
	}

	return std::stoi(id);
}

// Gets instruction name and list of arguments
pair Parser::getInstrSignature(std::string& line)
{
	std::stringstream ss(line);
	std::string::size_type init_pos;
	std::string item;
	pair instruction;

	//spacing variations support: '='/' = '/'= '/' ='
	init_pos = line.find("=") + 1;
	ss.seekg(init_pos);
	if (ss.get() == ' ') {  
		ss.seekg(init_pos + 1);
	}
	else { 
		ss.seekg(init_pos);
	}

	std::getline(ss, item, ' '); //reads instruction name
	if (item.empty()) {
		throw std::exception("Wrong syntax : Incorrect instruction name\n");
	}
	std::transform(item.begin(), item.end(), item.begin(), ::tolower);
	instruction.first = item;

	while (std::getline(ss, item, ' ')) { //reads instruction arguments
		std::transform(item.begin(), item.end(), item.begin(), ::tolower);
		instruction.second.push_back(item);
	}

	return instruction;
}
