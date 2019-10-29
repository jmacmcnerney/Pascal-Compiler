//------------------------------------------------------------------------
//                              jlex.cpp
//------------------------------------------------------------------------
// This is the main(), the test program, for project 1.2 - the Lexemizer.
// You are required to use this test program for your project
//------------------------------------------------------------------------
#include <iostream>
#include "scanner.h"
using namespace std;

int main(int argc, char** argv) {
	scanner s;
	string  filename;

	// get file name from arguments or user
	if (argc > 1)
		filename = argv[1];
	else {
		cout << "Enter source file name: ";
		cin >> filename;
	}

	// open source file
	if (!s.open(filename)) {
		cout << s.getError() << endl;
		return 0;
	}
	// repeatedly get lexemes until an error occurs or there are no more
	string lex="?";
	while (lex != "" && !s.isError()) {
		lex = s.getNextLexeme();
		cout << lex << endl;
	}

	// chech for error message
	if (s.isError())
		cout << s.getError() << endl;
	else
		cout << "TEST PROGRAM: No error found\n";

	return 0;
}