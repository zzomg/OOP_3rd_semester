/*
*
*
*
*
*
*
THIS IS AN OLD VERSION, DON'T USE IT 
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/


#include "pch.h"
#include <iostream>
#include <algorithm> //std::sort
#include <fstream>
#include <string>
#include <clocale> //std::setlocale
#include <cwctype> //std::iswalpha
#include <vector>
#include <list>
#include <map>

typedef std::pair<std::wstring, long int> pair;

class Parser
{
public:
	Parser() { totalWordsCount_ = 0; };
	~Parser() {};

	std::list<std::wstring> load_txt_();
	void parse_txt_(std::list<std::wstring> lines_);
	void parse_line_(std::wstring& line);
	std::vector<pair> sort_(std::map<std::wstring, long int> map);
	void push_csv_();

public:
	std::wifstream inputFile;
	std::wofstream outputFile;

private:
	std::map<std::wstring, long int> words_;
	std::list<std::wstring> lines_;
	long int totalWordsCount_;
};

std::list<std::wstring> Parser::load_txt_()
{
	for (std::wstring line; std::getline(inputFile, line); )
	{
		lines_.push_back(line);
	}
	return lines_;
}

void Parser::parse_txt_(std::list<std::wstring> lines_)
{
	for (std::wstring& line : lines_)
	{
		parse_line_(line);
	}
}

void Parser::parse_line_(std::wstring& line)
{
	std::wstring buf;

	for (wchar_t& c : line) {
		if (std::iswalnum(c)) {
			buf.push_back(c);
		} 
		else {
			std::map<std::wstring, long int>::iterator it = words_.find(buf);
			if (!buf.empty()) {
				if (it != words_.end()) {
					it->second++;
					totalWordsCount_++;
					buf.clear();
				}
				else {
					words_[buf] = 1;
					totalWordsCount_++;
					buf.clear();
				}
			}
			else {
				continue;
			}
		}
	}
}

std::vector<pair> Parser::sort_(std::map<std::wstring, long int> map)
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
	for (auto const &pair : Parser::sort_(words_))
	{
		outputFile << pair.first << "," << pair.second << ","
			<< ((float)pair.second / (float)totalWordsCount_) * 100 << std::endl;
	}
}

int main(int argc, char **argv)
{	
	std::setlocale(LC_ALL, "");

	Parser p1;

	p1.inputFile.open("input.txt");
	p1.outputFile.open("output_rus.txt");

	if (!p1.inputFile)  
	{
		std::cout << "File 1 opening failed\n";
		return EXIT_FAILURE;
	}
	if (!p1.outputFile)
	{
		std::cout << "File 2 opening failed\n";
		return EXIT_FAILURE;
	}

	p1.parse_txt_(p1.load_txt_());
	p1.push_csv_();

	p1.inputFile.close();
	p1.outputFile.close();

	return 0;
}
