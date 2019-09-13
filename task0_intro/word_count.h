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
	Parser(std::string inputFilePath, std::string outputFilePath);
	~Parser();

	void parse_txt_();
	void parse_line_(std::string& line);
	std::vector<pair> sort_(std::map<std::string, long int>& map);
	void push_csv_();

	std::ifstream inputFile_;
	std::ofstream outputFile_;

private:
	std::map<std::string, long int> words_;
	long int totalWordsCount_;
};

Parser::Parser(std::string inputFilePath, std::string outputFilePath)
{
	totalWordsCount_ = 0;
	inputFile_.open(inputFilePath);
	outputFile_.open(outputFilePath);
};

Parser::~Parser()
{
	inputFile_.close();
	outputFile_.close();
};

