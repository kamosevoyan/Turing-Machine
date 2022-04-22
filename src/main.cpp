#include "TuringMachine.h"

void ui_handler(int argc, char * argv[])
{
	if (argc == 1)
	{
<<<<<<< HEAD
		throw std::string("Error: missing inputs: Use this pattern: file input\n\n");
=======
		std::cerr << "Error: missing inputs: Use this pattern: file input\n\n";
		exit(1);
>>>>>>> 42691bc6691f92c1b61abd99af133a1279b885a2
	}
	if (argc <= 2)
	{
<<<<<<< HEAD
		throw std::string("Error: missing tape input\n");
=======
		std::cerr << "Error: missing tape input\n";
		exit(1);
>>>>>>> 42691bc6691f92c1b61abd99af133a1279b885a2
	}

	std::ifstream File;
	File.open(argv[1]);

	if(File.fail())
	{
<<<<<<< HEAD
		throw std::string("Error: invalid file name.\n");
	}
=======
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
>>>>>>> 42691bc6691f92c1b61abd99af133a1279b885a2
	
	if (File.peek() == std::ifstream::traits_type::eof())
	{
		throw std::string("Error: empty file\n");
	}

<<<<<<< HEAD
	std::stringstream stream;
	stream << File.rdbuf();
	TM::TuringMachine machine;
	machine.setConfiguration(stream);
	machine.setInput(argv[2]);


}

int main(int argc, char* argv[])
{
	try
	{
		ui_handler(argc, argv);
	}
	catch(const std::string& report)
	{
		std::cerr << "\033[31m" << report << "\033[0m";
		return 1;
	}
	
=======
>>>>>>> 42691bc6691f92c1b61abd99af133a1279b885a2
	return 0;
}