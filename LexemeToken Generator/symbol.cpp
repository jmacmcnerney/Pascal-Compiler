//John McNerney
#include "symbol.h"

//-----------------------------------------------------
//						constructor
//-----------------------------------------------------
symbol::symbol() {
	lexeme = "";
	category = dataType = 0;
	next = NULL;
}

//-----------------------------------------------------
//					simple gets/sets
//-----------------------------------------------------
void   symbol::setDataType(int dt) { dataType = dt; }
string symbol::getLex() { return lexeme; }
int    symbol::getCategory() { return category; }
int    symbol::getDataType() { return dataType; }

//-----------------------------------------------------
//					    setLex
//-----------------------------------------------------
void   symbol::setLex(string lex) {
	lexeme = lex;

	//TO DO: set category and data type.
	// myvar,		'Hello',	 12.5,      12
	// identifier,	string lit,	 real lit,  int lit
	//if the first character in a lexeme is a digit then it has to be a real lit or int lit
	if (isdigit(lexeme[0])) {
		category = SYMCAT_INT_LIT;
		dataType = DTYPE_INT;
		//checks each char in lexeme for period
		for (int i = 0; i < lexeme.length(); i++) {
			//if period found must be a float (real lit)
			if (lexeme[i] == '.') {
				category = SYMCAT_REAL_LIT;
				dataType = DTYPE_REAL;
			}
		}
	}
	//if first character is a tick must be a string lit
	else if (lexeme[0] == '\'') {
		category = SYMCAT_STRING_LIT;
		dataType = DTYPE_STRING;
	}
	//if none of the above, must be identifier
	else {
		category = SYMCAT_IDENT;
		dataType = DTYPE_UNKNOWN;
	}
} // setLex()
