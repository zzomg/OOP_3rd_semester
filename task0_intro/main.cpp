#include <iostream>
#include "pch.h"
#include "txt_parser.h"

int main(int argc, char** argv)
{
	Parser p1;
	
	try {
		p1.parse_txt_(argv[1]);
		p1.push_csv_(argv[2]);
	}
	catch (std::exception &ex) {
		std::cout << "Error: " << ex.what() << std::endl;
	}

	return 0;
}
