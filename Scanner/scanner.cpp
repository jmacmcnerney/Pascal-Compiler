//John McNerney
//------------------------------------------------------------------------
//                              scanner.cpp
//------------------------------------------------------------------------
// This file opens the source file, starts the next lexeme, checks for and 
// returns errors, handles the categorization of characters and state
// transitions for building lexemes.
//------------------------------------------------------------------------
#include "scanner.h"

//------------------------------------------------------------------------
//								   open
//------------------------------------------------------------------------
int scanner::open(string source) {
	//opens file
	f.open(source);
	//if file fails to open, sets error and ends program
	if (f.fail()) {
		error = "SCANERROR::open() Line=" + to_string(lineNo) + " Character=UNPRINTABLE (32) \nUnable to open input file.";
		return 0;
	}
	return 1;
} // open()

//------------------------------------------------------------------------
//							   getNextLexeme
//------------------------------------------------------------------------
string scanner::getNextLexeme() {
	//initialize state as START and resets lex for each new lexeme
	state = START;
	lex = "";

	//reads characters until STATE is ERR or HALT
	while (state != ERR && state != HALT) {
		//reads 1 char from file and assigns it to c
		c = f.get();
		//sends state, c, and 1 char in advance aka the lookahead (f.peek()) to getTrans()
		state = getTrans(state, c, f.peek());
	}
	//resets lexeme if is_error is true
	if (is_error)
		return "";
	//otherwise returns lexeme to be displayed
	else 
		return lex;
} // getNextLexeme()

//------------------------------------------------------------------------
//								getError
//------------------------------------------------------------------------
string scanner::getError() {
	return error;
} // getError()

//------------------------------------------------------------------------
//								isError
//------------------------------------------------------------------------
bool scanner::isError() {
	return is_error;
} // isError()


//------------------------------------------------------------------------
//								getTrans
//------------------------------------------------------------------------
FSA_STATE scanner::getTrans(FSA_STATE state, char c, char la) {
	//finds CHAR_CAT of current char (c) and lookahead char found with f.peek() (la)
	CHAR_CAT cCat = categorizeChar(c);
	CHAR_CAT laCat = categorizeChar(la);

	//if CHAR_CAT of current char (c) == EOL, increments lineNo
	if (cCat == EOL) { lineNo += 1; }

	//checks for error using current state and current char (c) CHAR_CAT
	checkError(state, cCat);

	//if is_error == false
	if (!is_error) {
		//loops through all FSA transitions
		for (int i = 0; i < NUM_FSA_TRANS; i++) {
			//if (current loop state == state) AND (current char CHAR_CAR == cCat CHAR_CAT OR ANY CHAR_CAT) 
			//AND (current lookahead CHAR_CAT == laCat CHAR_CAT OR ANY CHAR_CAT) then perform ACT
			if ((trans[i].from == state) && (trans[i].ccat == cCat || trans[i].ccat == ANY) && (trans[i].la == laCat || trans[i].la == ANY)) {
				//if ACT == KEEP add to lexeme
				if (trans[i].act == KEEP) { lex += c; }
				//if ACT == PUTB put char back
				if (trans[i].act == PUTB) { f.putback(c); }
				//no need to do anything for ACT == DISC
				//return TO FSA_STATE of current FSA transition
				return trans[i].to;
			}
		}
	}
	return ERR;
} // getTrans()

//------------------------------------------------------------------------
//							  categorizeChar
//------------------------------------------------------------------------
CHAR_CAT scanner::categorizeChar(char c) {
	//assigns CHAR_CAT based on the value of c
	if (c == '\t' || c == ' ') { return WHITESP; }
	if (int(c) == -1) { return EOFL; }
	if (c == '\n' || c == '\r') { return EOL; }
	if (c == '{') { return LBRACE; }
	if (c == '}') { return RBRACE; }
	if (isalpha(c)) { return ALPHA; }
	if (isdigit(c)) { return DIGIT; }
	if (c == '_') { return UNDERSC; }
	if (c == '.') { return DOTC; }
	if (c == '\'') { return QUOTE; }
	if (c == '>') { return GTHANC; }
	if (c == '<') { return LTHANC; }
	if (c == ':') { return COLON; }
	if (c == '=') { return EQUAL; }
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '[' || c == ']' || c == ',' 
		|| c == ':' || c == ';' || c == '^' || c == '(' || c == ')') { return SYMBOL; }
	if (int(c) >= int(' ') && int(c) < int('~')) { return OTHER; }
	//assigns CHAR_CAT as UNKNOWN if c is none of the above
	return UNKNOWN;	
} // categorizeChar()

//------------------------------------------------------------------------
//								checkError
//------------------------------------------------------------------------
void scanner::checkError(FSA_STATE state, char cCat) {
	//sets error lineNo text
	string lineNoText = "Line=" + to_string(lineNo);
	//sets error char text
	string charText = "Character='" + string(1, c) + "' (" + to_string(c) + ")";
	//if cCat == UNKNOWN (from categorizeChar()) set character to be UNPRINTABLE
	if (cCat == UNKNOWN || cCat == EOFL || cCat == EOL) { charText = "Character=UNPRINTABLE (" + to_string(c) + ")"; }
	//Comment Errors (RBRACE before LBRACE OR Invalid Beginning of Lexeme (OTHER CHAR_CAT))
	if (state == START && (cCat == RBRACE || cCat == OTHER)) {
		if (cCat == RBRACE) {
			error = "SCANERROR::getTrans() " + lineNoText + " " + charText + "\n" + "Beginning of comment expected.";
			is_error = true;
		}
		else {
			error = "SCANERROR::getTrans() " + lineNoText + " " + charText + "\n" + "Invalid beginning of lexeme.";
			is_error = true;
		}	
	}
	//Comment Error (No RBRACE)
	if (state == CMNT && cCat == EOFL) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "End of comment expected.";
		is_error = true;
	}
	//String Literal Error (No close to SLIT before EOL)
	if (state == SLIT && cCat == EOL) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "End of line found in string literal.";
		is_error = true;
	}
	//String Literal Error (No close to SLIT before EOFL)
	if (state == SLIT && cCat == EOFL) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "End of file found in string literal.";
		is_error = true;
	}
	//Should Never Occur (No transition out of SLIT)
	if (state == SLIT && cCat == OTHER) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "STATE=SLIT No transition from state found.";
		is_error = true;
	}
	//Should Never Occur (Single quote to finish SLIT)
	if (state == SLITQ && cCat == ANY) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "Single quote expected.";
		is_error = true;
	}
	//Should Never Occur (Anything but # after DECPT)
	if (state == DECPT && cCat != DIGIT) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "Digit expected.";
		is_error = true;
	}
	//Invalid Char in source. (Ascii < 32 or > 126)
	if (cCat == UNKNOWN) {
		//sets char in charText to ~ if c == ~ (since that is not UNPRINTABLE)
		if (c == '~') { charText = "Character='" + string(1, c) + "' (" + to_string(c) + ")"; }
		error = "SCANERROR::categorizeChar() " + lineNoText + " " + charText + "\n" + "Invalid character found in source.";
		is_error = true;
	}
	//Should Never Occur (Unknown Error for current STATE)
	if (state == ERR) {
		error = "SCANERROR::getNextLexeme() " + lineNoText + " " + charText + "\n" + "Unknown error for STATE=" + to_string(state) + ".";
		is_error = true;
	}
} // checkError()