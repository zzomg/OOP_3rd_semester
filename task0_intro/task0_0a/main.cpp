#include "pch.h"
#include <iostream>
#include "module1.h"
#include "module2.h"
#include "module3.h"

//using namespace std; (вопрос 6)

int main(int argc, char** argv)
{
	std::cout << "Hello world!" << "\n";

	std::cout << Module1::getMyName() << "\n";
	std::cout << Module2::getMyName() << "\n";

	using namespace Module1;
	std::cout << getMyName() << "\n"; // (A)
	std::cout << Module2::getMyName() << "\n";

	//using namespace Module2; // (B)
	//std::cout << getMyName() << "\n"; // COMPILATION ERROR (C) 

	// Вопрос 4
	// * можно дать другое имя
	// * можно включить только определенный метод using Module2::getMyName; 
	// * можно обратиться напрямую Module2::getMyName();

	using Module2::getMyName;
	std::cout << getMyName() << "\n"; // (D)

	//Вопрос 5
	using Module3::getMyName; 
	std::cout << Module3::getMyName() << "\n";

	return 0;
}
