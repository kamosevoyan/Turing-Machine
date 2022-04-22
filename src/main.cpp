#include "TuringMachine.h"

void ui_handler(int argc, char * argv[])
{
	if (argc == 1)
	{
		throw std::string("Error: missing inputs: Use this pattern: file input\n\n");
	}
	if (argc <= 2)
	{
		throw std::string("Error: missing tape input\n");
	}

	std::ifstream File;
	File.open(argv[1]);

	if(File.fail())
	{
		throw std::string("Error: invalid file name.\n");
	}
	
	if (File.peek() == std::ifstream::traits_type::eof())
	{
		throw std::string("Error: empty file\n");
	}

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
	
	return 0;
}