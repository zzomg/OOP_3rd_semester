#include "pch.h"
#include "txt_parser.h"
#include <iostream>
#include <algorithm> //std::sort
#include <fstream>
#include <string>
#include <cctype> //std::isalnum
#include <vector>
#include <list>
#include <map>

Parser::Parser()
{
	totalWordsCount_ = 0;
}


void Parser::parse_txt_(const std::string &inputFilePath)

{
	std::ifstream inputFile_(inputFilePath);
	if (!inputFile_)
	{
		throw std::exception("File in opening failed\n");
	}
	std::string line;
	while ( std::getline(inputFile_, line))
	{
		parse_line_(line);
	}
}

void Parser::parse_line_(std::string& line)
{
	std::string buf;

	for (char& c : line) {
		if (std::isalnum((unsigned char)c)) {
			buf.push_back(c);
		}
		else {
			if (!buf.empty()) { //на случай, если два разделителя идут подряд
				words_[buf]++;
				totalWordsCount_++;
				buf.clear();
			}
			else {
				continue;
			}
		}
	}
}

std::vector<pair> sort_(const std::map<std::string, long int>& map)
{
	std::vector<pair> vec(map.begin(), map.end());

	std::sort(vec.begin(), vec.end(), [](const pair& l, const pair& r) {
		if (l.second != r.second)
			return l.second > r.second;
		return l.first > r.first;
	});

	return vec;
}

void Parser::push_csv_(const std::string &outputFilePath)
{
	std::ofstream outputFile_(outputFilePath);
	if (!outputFile_)
	{
		throw std::exception("File out opening failed\n");
	}

	for (auto& pair : sort_(words_))
	{
		outputFile_ << pair.first << "," << pair.second << ","
			<< ((float)pair.second / (float)totalWordsCount_) * 100 << std::endl;
	}
}


