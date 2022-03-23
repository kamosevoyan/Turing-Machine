#include "TuringMachine.h"
#define DEBUG

size_t TM::TuringMachine::EXTEND_SIZE = 10;

TM::TuringMachine::Tape::Tape()
{
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	this->w = w.ws_col;
	this->h = w.ws_row;
	
	TM::setExtedSize(this->w * 2);
}

void TM::TuringMachine::Tape::set(const std::string& input, char empty)
{
	this->rightSide = input;
	this->rightSide.insert(this->rightSide.end(), TM::TuringMachine::EXTEND_SIZE , empty);
	this->leftSide = std::string(TM::TuringMachine::EXTEND_SIZE, empty);
	this->emptySymbol = empty;
	this->pos = 0;
}

void TM::TuringMachine::Tape::print()
{
	std::string longSpace = std::string(this->w / 2, ' ');
	
	if (this->pos > (int)this->w / 2)
	{
		std::copy(this->rightSide.begin() - this->w/2 + this->pos, this->rightSide.begin() + this->w/2 + this->pos, std::ostream_iterator<char>(std::cout, ""));
	}
	else if (abs(this->pos) > this->w / 2)
	{
		std::cout << "from " << (this->pos - (int)this->w / 2) << " to " << ( this->pos + (int)this->w / 2) << "\n";
		std::copy(this->leftSide.rend() + this->pos - (int)this->w / 2, this->leftSide.rend() + this->pos + (int)this->w / 2, std::ostream_iterator<char>(std::cout, ""));
	}
	else
	{
		std::copy(this->leftSide.rend() - this->w/2 + this->pos, this->leftSide.rend(), std::ostream_iterator<char>(std::cout, ""));
		std::copy(this->rightSide.begin() , this->rightSide.begin() + this->w/2 + this->pos, std::ostream_iterator<char>(std::cout, ""));
	}
	
	std::cout << std::endl;
		
	std::cout << longSpace << "^" << std::endl;
	
	
	std::cout << longSpace <<"▀" << std::endl;
}

char& TM::TuringMachine::Tape::get(int pos)
{
	if (pos >= 0)
	{
		return this->rightSide[pos];
	}
	else
	{
		return this->leftSide[abs(pos) - 1];
	}
}

void TM::TuringMachine::Tape::write(char value)
{
	this->get(this->pos) = value;
}


int TM::TuringMachine::Tape::move(char dir)
{
	int where;
		
	switch(dir) 
	{
		case '<':
			where = -1;
			break;
		case '@':
			where = 0;
			break;
		case '>':
			where = 1;
			break;
	}
	
	this->pos += where;
	
	if (pos >= 0)
	{
		if ((size_t)pos >= this->rightSide.length())
		{
			this->rightSide.insert(this->rightSide.end(), TM::TuringMachine::EXTEND_SIZE, this->emptySymbol);
		}
	}
	else
	{
		if ((size_t)(abs(pos) - 1) >= this->leftSide.length())
		{
			this->leftSide.insert(this->leftSide.end(), TM::TuringMachine::EXTEND_SIZE, this->emptySymbol);
		}
	}
	
	return pos;
	
}


bool TM::TuringMachine::setInput(const std::string& input) & 
{
	if (!constructed)
	{
		std::cerr<<"Error: The machine has not been constructed yet.\n";
		return false;
	}

	if (input.length() == 0)
	{
		std::cerr << "Error: Null input\n";
		return false;
	}
	
	this->tape.set(input, this->emptySymbol);
	
	std::string state(this->initalState);
	char write, read, move;
	int pos = 0;
	
#ifdef DEBUG
	std::string  command;
	long long step = 0;
#endif


	while (true)
	{
		
#ifdef DEBUG
		system("clear");
		
		std::cout << "Commands for debug: \n";//qqqq
		std::cout << "0: non stop mode: \n";
		std::cout << "f: 10  steps: \n";
		std::cout << "otherwise: 1 step\n\n\n\n";
		
		
		std::cout << "Debug info: \n\n";
		
		
		std::cout << "Total steps " << (++step) << "\n";
		
#endif

		read = this->tape.get(pos)   ;
		auto key = std::make_pair(state, read);

#ifdef DEBUG	
		std::cout << "read = "<< read << "\n";
		std::cout << "old state " << state << "\n";
		this->tape.print();
#endif
		
		if ((this->lambda.find(key) != this->lambda.end()) && (this->delta.find(key) != this->delta.end() ) && (this->nyu.find(key) != this->nyu.end()) )
		{
			state = this->lambda[key];	
			write = this->delta[key];
			move = this->nyu[key];
			
#ifdef DEBUG				
			std::cout << "new state " << state << "\n";
			std::cout << "write " << write << "\n";
			std::cout << "pos = "<< pos << "\n";
			std::cout << "move " << move << "\n";
#endif
		}
		else
		{
			if ( (this->alphabetSymbols.find(read) != std::string::npos) && (std::find(this->stateSymbols.begin(), this->stateSymbols.end(), state) != this->stateSymbols.end()) )
			{
				std::cerr << "Error: Unexpected input: read " << read << " in state " << state << "\n";
			}
			else
			{
				std::cerr<<"Error: Unknown input value " << read << std::endl;
			}
			return false;
		}
		
		
		this->tape.write(write);
		pos = this->tape.move(move);
		
		if (state == this->haltState)
		{
			this->tape.print();
			break;
		}

#ifdef DEBUG
	if (command == "f")
	{
		if (step % 10 != 0)
		{
			continue;
		}
		else
		{
			goto THERE;
		}
	}
	else if (command == "0")
	{
		continue;
	}
	else
	{
		THERE:	
		std::cout << "command : ";
		std::cin >> command;
	}		
#endif
		
	}
	return true;
}

TM::TuringMachine::TuringMachine()
{
	
}

bool TM::TuringMachine::setConfiguration(const std::stringstream& stream) &
{	
	std::string alphabetSymbols;
	std::vector<std::string> stateSymbols;
	std::stringstream data;
	
	data << stream.rdbuf();
	
	std::string inOutAlphabet, states, lambdaDeltaNyu, token;

	while(true)
	{
        std::getline(data, inOutAlphabet);
		if (inOutAlphabet.length() > 0)
		{
			break;
		}
	}

	while(true)
	{
        std::getline(data, states);
		if (states.length() > 0)
		{
			break;
		}
	}
		
	std::stringstream inOut(inOutAlphabet);
	std::stringstream sts(states);	
	char ctoken;
	
	while(std::getline(inOut, token, ','))
	{
		ctoken = strip(token)[0];		
		if (alphabetSymbols.find(ctoken) == std::string::npos)
		{
				alphabetSymbols.push_back(ctoken);
		}
		else
		{
			std::cerr<<"Error: Input/output alphabet value "<< token <<" is already given"<<std::endl;
			return false;
		}
	}
	
	while(std::getline(sts, token, ','))
	{
		token = strip(token);
		if(std::find(stateSymbols.begin(), stateSymbols.end(), token) == stateSymbols.end())
		{
			stateSymbols.push_back(token);
		}
		
		else
		{
			std::cerr<<"Error: State "<<token<<" is already given"<<std::endl;
			return false;
		}
	}
	
	this->alphabetSymbols = alphabetSymbols;
	this->stateSymbols = stateSymbols;
	
	std::stringstream lmDtNy;
	std::string rtoken;
	char dirtoken, ltoken;
	
	int rows = stateSymbols.size();
	int columns = alphabetSymbols.size();

#ifdef DEBUG
	std::cout<<"Input/output alphabet values \n";
	for (auto a: alphabetSymbols)
		std::cout<<a<<"\n";
	std::cout<<"***********************************\n\n\n\n";

	std::cout<<"States alphabet values \n";
	for (auto a: stateSymbols)
		std::cout<<a<<"\n";
	std::cout<<"************************************\n\n\n\n";
#endif

	for (int row = 0; row < rows - 1; ++row)
	{
		while(true)
		{
			std::getline(data, lambdaDeltaNyu);
			if (lambdaDeltaNyu.length() > 0)
			{
				break;
			}
			
			if (data.tellg() == std::string::npos)
			{
				std::cerr<<"Error: Configuration values for state " << stateSymbols[row]<<" and below are not given\n";
				return false;
			}
		}
			
			
		lmDtNy = std::stringstream(lambdaDeltaNyu);


		for (int column = 0; column < columns; ++column)
		{
			if (lmDtNy.tellg() == std::string::npos)
			{
				std::cerr<<"Error: Less values than expected in "<<row<< " row "<<column<< " column "<<std::endl;
				return false;
			}

			std::getline(lmDtNy, token, '|');
			
			token = strip(token);
			
			if (token == "X")
			{
				continue;
			}
		
			int first = token.find(',');
			int second = token.find(',', first + 1);
			
			ltoken = token[0];
			rtoken = token.substr(first + 1, second - first - 1);
			dirtoken = token[token.length() - 1];
						
						
			if(std::find(this->alphabetSymbols.begin(), this->alphabetSymbols.end(), ltoken) == this->alphabetSymbols.end())
			{
				std::cerr<<"Error: Unknows output value "<< ltoken<<" in "<<row<<" row "<<column<<" column "<<std::endl;
				return false;
			}

			if(std::find(this->stateSymbols.begin(), this->stateSymbols.end(), rtoken) == this->stateSymbols.end())
			{
				std::cerr<<"Error: Unknown state value "<< rtoken<<" in "<<row<<" row "<<column<<" column "<<std::endl;
				return false;
			}
			
			if(std::find(TuringMachine::dir.begin(), TuringMachine::dir.end(), dirtoken) == TuringMachine::dir.end())
			{
				std::cerr << "Error: Unknown direction value " << dirtoken << " in " << row << " row " << column << " column " << std::endl;
				return false;
			}
			
			this->lambda[std::make_pair(this->stateSymbols[row], this->alphabetSymbols[column])] = rtoken;
			this->delta[std::make_pair(this->stateSymbols[row], this->alphabetSymbols[column])] = ltoken;
			this->nyu[std::make_pair(this->stateSymbols[row], this->alphabetSymbols[column])] = dirtoken;
		}		
		
		if(lmDtNy.tellg() != std::string::npos)
		{
			std::cerr<<"Error: More values in " << row << " row than expected" << std::endl;
			return false;
		}
	}
	
	
	while(true)
	{
		std::getline(data, token);
		if (token.length() > 0)
		{
			break;
		}
		
		if(data.tellg() == std::string::npos)
		{
			std::cerr<<"Error: Initial state value is not given\n";
			return false;
		}
	}
	
	token = strip(token);
	
	if (std::find(this->stateSymbols.begin(), this->stateSymbols.end(), token) == this->stateSymbols.end())
	{
		std::cerr<<"Error: Unknown initial state value "<<token<<std::endl;
		return false;
	}
	
	this->initalState = token;
	token.erase();
	
	while(true)
	{
		std::getline(data, token);
		if (token.length() > 0)
		{
			break;
		}
		
		if(data.tellg() == std::string::npos)
		{
			std::cerr<<"Error: Halt state value is not given\n";
			return false;
		}
	}
	
	
	token = strip(token);
	
	if (std::find(this->stateSymbols.begin(), this->stateSymbols.end(), token) == this->stateSymbols.end())
	{
		std::cerr<<"Error: Unknown halt state value "<<token<<std::endl;
		return false;
	}
	
	this->haltState = token;
	token.erase();
	
	
	while(true)
	{
		std::getline(data, token);
		if (token.length() > 0)
		{
			break;
		}
		
		if(data.tellg() == std::string::npos)
		{
			std::cerr<<"Error: Empty symbol value is not given\n";
			return false;
		}
	}
	
	
	ctoken = strip(token)[0];
	
	if (this->alphabetSymbols.find(ctoken) == std::string::npos)			
	{
		std::cerr<<"Error: Enknown empty symbol state value "<<token<<std::endl;
		return false;
	}
	
	this->emptySymbol = ctoken;
	
	this->constructed = true;
	
#ifdef DEBUG
	std::cout << "inital state " << this->initalState << "\n";
	std::cout << "halt state " << this->haltState << "\n";
	std::cout << "empty symbol " << this->emptySymbol << "\n\n\n";
	this->print();
#endif
	
	return true;
}

void TM::TuringMachine::print()
{
	std::cout << "lambdas\n";
	for (auto it = this->lambda.begin(); it != this->lambda.end(); ++it)
	{
		std::cout << "( "<< it->first.first << " , " << it->first.second << " ) ->  " << it->second << "\n";;
	}
	
	std::cout << "\n\n";
	
	std::cout << "deltas\n";
	for (auto it = this->delta.begin(); it != this->delta.end(); ++it)
	{
		std::cout << "( "<< it->first.first << " , " << it->first.second << " ) ->  " << it->second << "\n";;
	}
	
	std::cout << "\n\n";
	
	std::cout << "nyus\n";
	for (auto it = this->nyu.begin(); it != this->nyu.end(); ++it)
	{
		std::cout << "( "<< it->first.first << " , " << it->first.second << " ) ->  " << it->second << "\n";;
	}
	
	std::cout << "\n\n";
}

std::string TM::strip(const std::string& input)
{
	std::string output(input);
	std::size_t pos = 0;
	
	while( ((pos = output.find(" ")) != std::string::npos)   ||  ((pos = output.find("\t")) != std::string::npos) )
	{
		output.erase(pos, 1);
	}
	return output;
}

void TM::setExtedSize(const size_t size)
{
	if ( (0 < size) && (size < 1000) )
	{
		TM::TuringMachine::EXTEND_SIZE = size;
	}
	else
	{
		std::cerr << "Error: Invalid exted size value.\n";
	}
}