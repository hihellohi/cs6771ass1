#include <iostream>
#include <fstream>
#include <iomanip>
#include <stack>
#include <queue>
#include <vector>
#include <stdexcept>
#include <cmath>

const std::string ADD_COMMAND = "add";
const std::string SUB_COMMAND = "sub";
const std::string MULT_COMMAND = "mult";
const std::string DIV_COMMAND = "div";
const std::string ROOT_COMMAND = "sqrt";
const std::string POP_COMMAND = "pop";
const std::string REV_COMMAND = "reverse";
const std::string REPEAT_COMMAND = "repeat";
const std::string END_REPEAT_COMMAND = "endrepeat";

class Number;
void execute(
		std::vector<std::string>::iterator start, 
		std::vector<std::string>::iterator end,
		std::stack<Number> &s);

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

		int getIntValue() {
			return doublemode ? 0 : intval;
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

		Number root() {
			return Number(sqrt(doublemode ? doubleval : intval));
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

void root(std::stack<Number> &s){
	Number a = s.top();
	s.pop();
	Number c = a.root();
	s.push(c);

	std::cout << "root " << a << " = " << c << '\n';
}

void rev(std::stack<Number> &s){
	Number a = s.top();
	s.pop();

	std::queue<Number> q;
	for(int i = 0; i < a.getIntValue(); i++){
		q.push(s.top());
		s.pop();
	}

	while(!q.empty()){
		s.push(q.front());
		q.pop();
	}
}

void repeat(
		std::stack<Number> &s,
		std::vector<std::string>::iterator &it,
		std::vector<std::string>::iterator end) {

	Number a = s.top();
	s.pop();
	
	auto start = it + 1;
	int level = 1;
	while(level){
		if(++it == end) break;
		if(*it == REPEAT_COMMAND) level++;
		else if(*it == END_REPEAT_COMMAND) level--;
	}

	for(int i = 0; i < a.getIntValue(); i++){
		execute(start, it, s);
	}

	if(it == end) it--;
}

void push(std::stack<Number> &s, const std::string &word){
	try {
		if(word.find('.') == std::string::npos) {
			s.push(Number(std::stoi(word)));
		}
		else {
			s.push(Number(std::stod(word)));
		}
	}
	catch (const std::invalid_argument& e){
		std::cerr << "invalid argument: " << word << '\n';
	}
}

void execute(
		std::vector<std::string>::iterator start, 
		std::vector<std::string>::iterator end,
		std::stack<Number> &s) {

	std::string word;
	for(auto it = start; it != end; it++) {

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
		else if(word == ROOT_COMMAND) {
			root(s);
		}
		else if(word == POP_COMMAND) {
			s.pop();
		}
		else if(word == REV_COMMAND) {
			rev(s);
		}
		else if(word == REPEAT_COMMAND){
			repeat(s, it, end);
		}
		else{
			push(s, word);
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
	execute(commands.begin(), commands.end(), s);

	return 0;
}
