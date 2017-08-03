#include <iostream>
#include <fstream>
#include <iomanip>
#include <stack>
#include <stdexcept>

const std::string ADD_COMMAND = "add";
const std::string SUB_COMMAND = "sub";

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

int main(int argc, char* argv[]) {

	// setup the print out format for the precision required.
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
	std::cout.precision(3);

	// open the file for reading
	std::ifstream in;
	in.open(argv[1]);

	// string to be read into
	std::string word;
	std::stack<Number> s;

	// read the file while we have input.
	while (in >> word) {
		if(word == ADD_COMMAND) {
			add(s);
		}
		else if(word == SUB_COMMAND) {
			sub(s);
		}
		else{
			try {
				s.push(Number(std::stoi(word)));
			}
			catch (const std::invalid_argument& e){
				std::cerr << "invalid argument: " << e.what() << '\n';
			}
		}
	}
	in.close();
}
