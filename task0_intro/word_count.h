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
public:
	Parser() { totalWordsCount_ = 0; };
	~Parser() {};

	std::list<std::string> load_txt_();
	void parse_txt_(std::list<std::string> lines_);
	void parse_line_(std::string& line);
	std::vector<pair> sort_(std::map<std::string, long int> map);
	void push_csv_();

	std::ifstream inputFile;
	std::ofstream outputFile;

private:
	std::map<std::string, long int> words_;
	std::list<std::string> lines_;
	long int totalWordsCount_;
};
