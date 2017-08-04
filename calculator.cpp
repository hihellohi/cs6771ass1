#include <iostream>
#include <fstream>
#include <iomanip>
#include <stack>
#include <vector>
#include <stdexcept>

const std::string ADD_COMMAND = "add";
const std::string SUB_COMMAND = "sub";
const std::string MULT_COMMAND = "mult";
const std::string DIV_COMMAND = "div";

class Number {
	private:
		int intval;
		double doubleval;
		bool doublemode;

	public:
		Number(int initial){
			doublemode = false;
			intval = initial;
		}

		Number(double initial){
			doublemode = true;
			doubleval = initial;
		}

		Number operator+(const Number& other) {
			if(!doublemode && !other.doublemode){
				return Number(intval + other.intval);
			}
			else {
				return Number((doublemode ? doubleval : intval) + 
					(other.doublemode ? other.doubleval : other.intval));
			}
		}

		Number operator-(const Number& other) {
			if(!doublemode && !other.doublemode){
				return Number(intval - other.intval);
			}
			else {
				return Number((doublemode ? doubleval : intval) - 
					(other.doublemode ? other.doubleval : other.intval));
			}
		}

		Number operator*(const Number& other) {
			if(!doublemode && !other.doublemode){
				return Number(intval * other.intval);
			}
			else {
				return Number((doublemode ? doubleval : intval) * 
					(other.doublemode ? other.doubleval : other.intval));
			}
		}

		Number operator/(const Number& other) {
			if(!doublemode && !other.doublemode && !(intval % other.intval)){
				return Number(intval / other.intval);
			}
			else {
				return Number((doublemode ? doubleval : intval) / 
					(double)(other.doublemode ? other.doubleval : other.intval));
			}
		}

		friend std::ostream& operator<<(std::ostream& os, const Number& num);
};

std::ostream& operator<<(std::ostream& os, const Number& num) {
	if(num.doublemode){
		return os << num.doubleval;
	}
	return os << num.intval;
}

void add(std::stack<Number> &s){
	Number a = s.top();
	s.pop();
	Number b = s.top();
	s.pop();
	Number c = a + b;
	s.push(c);

	std::cout << a << " + " << b << " = " << c << '\n';
}

void sub(std::stack<Number> &s){
	Number a = s.top();
	s.pop();
	Number b = s.top();
	s.pop();
	Number c = a - b;
	s.push(c);

	std::cout << a << " - " << b << " = " << c << '\n';
}

void mult(std::stack<Number> &s){
	Number a = s.top();
	s.pop();
	Number b = s.top();
	s.pop();
	Number c = a * b;
	s.push(c);

	std::cout << a << " * " << b << " = " << c << '\n';
}

void div(std::stack<Number> &s){
	Number a = s.top();
	s.pop();
	Number b = s.top();
	s.pop();
	Number c = a / b;
	s.push(c);

	std::cout << a << " / " << b << " = " << c << '\n';
}

void execute(
		std::vector<std::string>::iterator start, 
		std::vector<std::string>::iterator end,
		unsigned int repeat,
		std::stack<Number> &s) {

	std::string word;
	for(auto j = 0u; j < repeat; j++)
	for(std::vector<std::string>::iterator it = start; it != end; it++) {

		word = *it;
		if(word == ADD_COMMAND) {
			add(s);
		}
		else if(word == SUB_COMMAND) {
			sub(s);
		}
		else if(word == MULT_COMMAND) {
			mult(s);
		}
		else if(word == DIV_COMMAND) {
			div(s);
		}
		else{
			try {
				if(word.find('.') == std::string::npos) {
					s.push(Number(std::stoi(word)));
				}
				else {
					s.push(Number(std::stod(word)));
				}
			}
			catch (const std::invalid_argument& e){
				std::cerr << "invalid argument: " << e.what() << '\n';
			}
		}
	}
}

int main(int argc, char* argv[]) {

	// setup the print out format for the precision required.
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
	std::cout.precision(3);

	// open the file for reading
	std::ifstream in;
	in.open(argv[1]);

	// string to be read into
	std::string word;
	std::vector<std::string> commands;

	// read the file while we have input.
	while (in >> word) {
		commands.push_back(word);
	}
	in.close();

	std::stack<Number> s;
	execute(commands.begin(), commands.end(), 1, s);

	return 0;
}
