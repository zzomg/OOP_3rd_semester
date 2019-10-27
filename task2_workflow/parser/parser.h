#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

typedef std::pair< std::string, std::vector<std::string> > pair;

class Parser
{
	std::map<int, pair> instructions_;
	std::vector<int> instructions_order_;

	void getInstrOrder(std::ifstream& workflowFile);
	void syntaxPrecheck(std::ifstream& workflowFile) const;
	int getInstrID(std::string& line);
	pair getInstrSignature(std::string& line);

public:

	Parser() {};
	~Parser() {};

	friend class Workflow;

	void parseWFFile(std::ifstream& workflowFile);
};
