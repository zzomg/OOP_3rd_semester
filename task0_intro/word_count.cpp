#include "pch.h"
#include <iostream>
#include <algorithm> //std::sort
#include <fstream>
#include <string>
#include <cctype> //std::isalnum
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

public:
	std::ifstream inputFile;
	std::ofstream outputFile;

private:
	std::map<std::string, long int> words_;
	std::list<std::string> lines_;
	long int totalWordsCount_;
};

std::list<std::string> Parser::load_txt_()
{
	for (std::string line; std::getline(inputFile, line); )
	{
		lines_.push_back(line);
	}
	return lines_;
}

void Parser::parse_txt_(std::list<std::string> lines_)
{
	for (std::string& line : lines_)
	{
		parse_line_(line);
	}
}

void Parser::parse_line_(std::string& line)
{
	std::string buf;

	for (char& c : line) {
		if (std::isalnum(static_cast<unsigned char>(c))) {
			buf.push_back(c);
		} 
		else {
			std::map<std::string, long int>::iterator it = words_.find(buf);
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

std::vector<pair> Parser::sort_(std::map<std::string, long int> map)
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
	Parser p1;
	p1.inputFile.open(argv[1]);
	p1.outputFile.open(argv[2]);

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

	return 0;
}
