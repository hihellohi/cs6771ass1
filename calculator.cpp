#include <iostream>
#include <fstream>
#include <iomanip>
#include <stack>
#include <stdexcept>

const std::string ADD_COMMAND = "add";

void add(std::stack<int> &s){
	int a = s.top();
	s.pop();
	int b = s.top();
	s.pop();
	int c = a + b;
	s.push(c);

	std::cout << a << " + " << b << " = " << c << '\n';
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
	std::stack<int> s;

	// read the file while we have input.
	while (in >> word) {
		try {
			if(word == ADD_COMMAND) {
				add(s);
			}
			else{
				s.push(std::stoi(word));
			}
		}
		catch (const std::invalid_argument& e){
			std::cerr << "invalid argument: " << e.what() << '\n';
		}
	}
	in.close();
}
