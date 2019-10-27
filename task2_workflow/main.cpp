#include "workflow.h"

int main(int argc, char** argv)
{
	Workflow wf;
	try
	{
		wf.run(argv[1]);
	}
	catch (const std::exception & ex)
	{
		std::cout << "Error : " << ex.what() << std::endl;
	}
}
