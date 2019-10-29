//John McNerney
#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// FSA states 
enum FSA_STATE {
	HALT, START, CMNT, ERR, ID, INT, DECPT, FRAC, SLIT, SLITQ, GTHAN, LTHAN, DOT
};
const int NUM_FSA_STATES = 13;
const string FSA_STATE_STR[NUM_FSA_STATES] = {
	"HALT", "START", "CMNT", "ERR", "ID", "INT", "DECPT", "FRAC", "SLIT", "SLITQ", 
	"GTHAN", "LTHAN", "DOT"
};

// FSA character categories
enum CHAR_CAT {
	UNKNOWN, ANY, WHITESP, EOFL, EOL, LBRACE, RBRACE, ALPHA, DIGIT, UNDERSC, DOTC, 
	QUOTE, GTHANC, LTHANC, COLON, EQUAL, SYMBOL, OTHER
};
const int NUM_CHAR_CATS = 18;
const string CHAR_CAT_STR[NUM_CHAR_CATS] = {
	"UNKNOWN", "ANY", "WHITESP", "EOFL", "EOL", "LBRACE", "RBRACE", "ALPHA", "DIGIT", 
	"UNDERSC", "DOTC","QUOTE", "GTHANC", "LTHANC", "COLON", "EQUAL", "SYMBOL", "OTHER"
};

// FSA actions
enum ACTION { KEEP, DISC, PUTB };
const int NUM_FSA_ACTIONS = 3;
const string ACTION_STR[NUM_FSA_ACTIONS] = { "KEEP","DISC","PUTB" };

// A Transition between two states based on a character category and look ahead,
// specifying the TO state and the action to take with the character
class transition {
public:
	FSA_STATE from;
	FSA_STATE to;
	CHAR_CAT  ccat;
	CHAR_CAT  la;
	ACTION    act;
};

// FSA Transitions 
const int NUM_FSA_TRANS = 41;
const transition trans[NUM_FSA_TRANS] = {
	// { FROM,   TO,    CHAR_CAT, LA_CHAR_CAT, ACTION }
	   { START,  START, WHITESP,  ANY,         DISC },
	   { START,  ID,    ALPHA,    ANY,         KEEP },
	   { START,  INT,   DIGIT,    ANY,         KEEP },
	   { START,  HALT,  SYMBOL,   ANY,         KEEP },
	   { START,  CMNT,  LBRACE,   ANY,         DISC },
	   { START,  DOT,   DOTC,     ANY,         KEEP },
	   { START,  SLIT,  QUOTE,    ANY,         KEEP },
	   { START,  GTHAN, GTHANC,   ANY,         KEEP },
	   { START,  LTHAN, LTHANC,   ANY,         KEEP },
	   { START,  GTHAN, COLON,    ANY,         KEEP },
	   { START,  HALT,  EQUAL,    ANY,         KEEP },
	   { START,  START, EOL,      ANY,         DISC },
	   { START,  HALT,  EOFL,     ANY,         DISC },
	   { CMNT,   START, RBRACE,   ANY,         DISC },
	   { CMNT,   ERR,   EOFL,     ANY,         KEEP },
	   { ID,     ID,    ALPHA,    ANY,         KEEP },
	   { ID,     ID,    DIGIT,    ANY,         KEEP },
	   { ID,     ID,    UNDERSC,  ANY,         KEEP },
	   { INT,    INT,   DIGIT,    ANY,         KEEP },
	   { INT,    HALT,  DOTC,     DOTC,        PUTB },
	   { INT,    DECPT, DOTC,     ANY,         KEEP },
	   { DECPT,  FRAC,  DIGIT,    ANY,         KEEP },
	   { FRAC,   FRAC,  DIGIT,    ANY,         KEEP },
	   { SLIT,   SLITQ, QUOTE,    QUOTE,       KEEP },
	   { SLIT,   HALT,  QUOTE,    ANY,         KEEP },
	   { SLIT,   ERR,   EOL,      ANY,         KEEP },
	   { SLIT,   ERR,   EOFL,     ANY,         KEEP },
	   { SLITQ,  SLIT,  QUOTE,    ANY,         KEEP },
	   { GTHAN,  HALT,  EQUAL,    ANY,         KEEP },
	   { LTHAN,  HALT,  EQUAL,    ANY,         KEEP },
	   { LTHAN,  HALT,  GTHANC,   ANY,         KEEP },
	   { DOT,    HALT,  DOTC,     ANY,         KEEP },
	   { CMNT,   CMNT,  ANY,      ANY,         DISC },
	   { ID,     HALT,  ANY,      ANY,         PUTB },
	   { INT,    HALT,  ANY,      ANY,         PUTB },
	   { DECPT,  ERR,   ANY,      ANY,         KEEP },
	   { FRAC,   HALT,  ANY,      ANY,         PUTB },
	   { SLIT,   SLIT,  ANY,      ANY,         KEEP },
	   { GTHAN,  HALT,  ANY,      ANY,         PUTB },
	   { LTHAN,  HALT,  ANY,      ANY,         PUTB },
	   { DOT,    HALT,  ANY,      ANY,         PUTB }
    // FINISH THESE, BASED ON THE FSA!!!!
};

class scanner {
public:
	int open(string source);
	string getNextLexeme();
	string getError();
	bool isError();
private: // suggested, but not required
	string error = "";
	bool is_error = false;
	int lineNo = 1;
	ifstream f;
	char c;
	string lex;
	FSA_STATE state;
	FSA_STATE getTrans(FSA_STATE state, char c, char la);
	CHAR_CAT categorizeChar(char c);
	void checkError(FSA_STATE state, char cCat);
};