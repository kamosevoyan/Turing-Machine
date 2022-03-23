#ifndef TURINGMACHINEINCLUDE
#define TURINGMACHINEINCLUDE

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>
#include <tuple>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

namespace TM
{
	std::string strip(const std::string& input);
	void setExtedSize(const size_t);

	class TuringMachine
	{	
		friend void setExtedSize(const size_t);
			
		static size_t EXTEND_SIZE;
		struct Tape
		{
			std::string leftSide,rightSide;
			char emptySymbol;
			int pos;	
			
			void set(const std::string&, char);
			int move(char);
			char& get(int);
			void write(char);
			void print();			
			size_t h,w;			
			Tape();
		};
		
		const std::string  dir{'<', '@', '>'};
		
		public:
			TuringMachine();
			TuringMachine(const TuringMachine&&) = delete;
			TuringMachine(TuringMachine&&) = delete;
			TuringMachine& operator=(const TuringMachine&) = delete;
			
			bool setConfiguration(const std::stringstream&) &;
			bool setInput(const std::string&) &;
					
		private:
			std::string alphabetSymbols;
			std::vector<std::string> stateSymbols;
			std::map<std::pair<std::string, char>, std::string> lambda;
			std::map<std::pair<std::string, char>, char> delta;
			std::map<std::pair<std::string, char>, char> nyu;
			std::string initalState;
			std::string haltState;
			char emptySymbol;
			Tape tape;
			bool constructed = false;
			
			void print();

	};
}
#endif
