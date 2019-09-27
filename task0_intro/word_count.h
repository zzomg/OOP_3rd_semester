#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

typedef std::pair<std::string, long int> pair;

class Parser
{
	void parse_line_(std::string& line);
	std::map<std::string, long int> words_;
	long int totalWordsCount_;
public:
	Parser();
	
	void parse_txt_(const std::string &inputFilePath);
	void push_csv_(const std::string &outputFilePath);
};



