#include "TuringMachine.h"

int main(int argc, char* argv[])
{
	
	if (argc == 1)
	{
		std::cerr << "Error: missing inputs: Use this pattern: file input\n\n";
		exit(1);
	}
	else if (argc <= 2)
	{
		std::cerr << "Error: missing tape input\n";
		exit(1);
	}
	else
	{
		std::ifstream File;
	
		File.open(argv[1]);
	
		if(File.fail())
		{
			std::cerr << "Error: invalid file name.\n";
			exit(1);
		}
		else
		{
			std::stringstream stream;
			stream << File.rdbuf();
			
			TM::TuringMachine machine;
	
			try
			{
				machine.setConfiguration(stream);
			}
			catch(const std::string& report)
			{
				std::cerr << "\033[31m" << report << "\033[0m";
			}
	
			try 
			{
				machine.setInput(argv[2]);
			}
			catch(const std::string& report)
			{
				std::cerr << "\033[31m" << report << "\033[0m";
			}
	
		}
	}

	return 0;
}