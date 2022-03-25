#include "TuringMachine.h"

int main(int argc, char* argv[])
{
	
	if (argc == 1)
	{
		std::cerr << "Error: missing inputs.\n";
		return -1;
	}
	else if (argc <= 2)
	{
		std::cerr << "Error: missing tape input\n";
		return -2;
	}
	else
	{
		std::ifstream File;
	
		File.open(argv[1]);
	
		if(File.fail())
		{
			std::cerr << "Error: invalid file name.\n";
			return -3;
		}
		else
		{
			std::stringstream stream;
			stream << File.rdbuf();
			File.close();
			
			TM::TuringMachine machine;
	
			if (!machine.setConfiguration(stream))
			{
				return -4;
			}
	
			if(machine.setInput(argv[2]))
			{
				return -5;
			}
	
		}
	}

	return 0;
}
