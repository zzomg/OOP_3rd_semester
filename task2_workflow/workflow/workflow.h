#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

class Workflow
{
public:
	Workflow() {};
	~Workflow() {};

	std::vector<std::string> input;
	friend class Parser;

	void run(const std::string& workflowFilePath);
};
