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

void Parser::parse_txt_()
{
	for (std::string line; std::getline(inputFile_, line, '.'); )
	{
		parse_line_(line);
	}
}

void Parser::parse_line_(std::string& line)
{
	std::string buf;

	for (char& c : line) {
		if (std::isalnum(c)) {
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

std::vector<pair> Parser::sort_(std::map<std::string, long int>& map)
{
	std::vector<pair> vec;

	std::copy(map.begin(), map.end(), std::back_inserter<std::vector<pair>>(vec));

	std::sort(vec.begin(), vec.end(), [](const pair& l, const pair& r) {
		if (l.second != r.second)
			return l.second > r.second;
		return l.first > r.first;
		});

	return vec;
}

void Parser::push_csv_()
{
	for (auto& pair : Parser::sort_(words_))
	{
		outputFile_ << pair.first << "," << pair.second << ","
			<< ((float)pair.second / (float)totalWordsCount_) * 100 << std::endl;
	}
}

int main(int argc, char** argv)
{
	Parser p1(argv[1], argv[2]);

	if (!p1.inputFile_)
	{
		std::cout << "File 1 opening failed\n";
		return EXIT_FAILURE;
	}
	if (!p1.outputFile_)
	{
		std::cout << "File 2 opening failed\n";
		return EXIT_FAILURE;
	}

	p1.parse_txt_();
	p1.push_csv_();

	return 0;
}
