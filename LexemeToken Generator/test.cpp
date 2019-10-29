//------------------------------------------------
//           test program for project 1.1
//------------------------------------------------
#include <iostream>
#include <fstream>
#include "symTbl.h"

int  main() {
	symTbl t;
	string lex;

	// test print empty symbol table
	symTblRef p;
	cout << "Empty table:\n";
	t.print();

	// open input file: one lexeme per line (may contain spaces)
	ifstream f("lextest.txt");
	if (f.fail()) {
		cout << "Error opening lextest.txt\n";
		getchar();
		return 1;
	}

	while (!f.eof()) {   // there should be NO BLANK LINE at end of file
		getline(f, lex);
		t.insert(lex);
		p = t.search(lex);
		if (p != NULL)
			cout << "inserted " << lex << ": found lex=" <<
			p->getLex() << " cat=" << p->getCategory() <<
			" dtype=" << p->getDataType() << endl;
		else
			cout << "insert failed on search: " << lex << endl;
	}
	f.close();

	t.print();
	return 0;
}
