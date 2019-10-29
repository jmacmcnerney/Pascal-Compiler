//John McNerney
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "symTbl.h"

//------------------- constructor ------------------
symTbl::symTbl() {
	for (int i = 0; i < SYMTBL_NUM_BUCKETS; i++)
		bucket[i] = NULL;
} // constructor

//--------------------  insert  -------------------
symTblRef symTbl::insert(string newlex) {
	symTblRef p = search(newlex);
	if (p != NULL) {
		return p;
	}
	//creating new symbol from symbol.cpp and setting it to the value of the lexeme
	p = new symbol;
	p->setLex(newlex);
	//getting the hash of the lexeme
	int number = hash(newlex);
	//Code from class
	p->next = bucket[number];
	bucket[number] = p;
	return p;
} // insert()

//--------------------- hash ---------------------
int symTbl::hash(string lex) {
	//variable creation for loops
	int hash;
	int ascii;
	//scans the lexeme
	for (int i = 0; i < lex.length(); i++) {
		//converts each char to ascii and adds it all to hash (code from class)
		ascii = ((int)lex[i]);
		hash += ascii;
	}
	//Code from project specs
	return hash % SYMTBL_NUM_BUCKETS;
} // hash()

//---------------------- search ---------------------
symTblRef symTbl::search(string search4) {
	//grab the hash
	int number = hash(search4);
	//create a new pointer and point it at the bucket of the hash
	symTblRef p = bucket[number];
	//scan through the bucket to see if the search4 entry already exists
	while (p != NULL) {
		if (p->getLex() == search4) {
			return p;
		}
		p = p->next;
	}
	//returns null if the search4 isnt already in a bucket
	return NULL;
} // search()

//---------------------- print ---------------------
void symTbl::print() {
	int numEntries = 0;  
	cout << "---------- SYMBOL TABLE ------------\n";
	for (int i = 0; i < SYMTBL_NUM_BUCKETS; i++) {
		// only print bucket if it has an entry
		if (bucket[i] != NULL) {
			// print first entry in this bucket with bucket #
			symTblRef p = bucket[i];
			cout << "[" << setw(3) << i << "]: ";
			cout << "cat=" << p->getCategory()
				<< "  dtype=" << p->getDataType()
				<< "  lex=" << p->getLex() << endl;
			numEntries++;
			p = bucket[i]->next;

			// print all other entries in this bucket
			while (p) {
				cout << "       cat=" << p->getCategory()
					<< "  dtype=" << p->getDataType()
					<< "  lex=" << p->getLex() << endl;
				numEntries++;
				p = p->next;
			} // while more entries in bucket
		} // if bucket not empty
	} // for all buckets
	cout << "------------------------------------\n";
	cout << "Number of entries: " << numEntries << endl << endl;
	cout << "Press any key to continue..";
	getchar();
} // print()
