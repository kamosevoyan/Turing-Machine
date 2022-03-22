#include "TuringMachine.h"

int main(int argc, char* argv[])
{
	TM::setExtedSize(23);
	TM::TuringMachine machine;
	
	machine.setConfiguration("src/configs/unary_product.txt");
	
	machine.setInput("11*11");
	
	return 0;
}
