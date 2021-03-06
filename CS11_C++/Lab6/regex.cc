#include "regex.hh"
#include <iostream>
#include <string>


/* Initialize the regex operator to apply exactly once. */
RegexOperator::RegexOperator() {
    minRepeat = 1;
    maxRepeat = 1;
}


/* Returns the "minimum repeat count" value. */
int RegexOperator::getMinRepeat() const {
    return minRepeat;
}


/* Returns the "maximum repeat count" value. */
int RegexOperator::getMaxRepeat() const {
    return maxRepeat;
}


/* Sets the "minimum repeat count" value. */
void RegexOperator::setMinRepeat(int n) {
    assert(n >= 0);
    minRepeat = n;
}


/* Sets the "maximum repeat count" value. */
void RegexOperator::setMaxRepeat(int n) {
    assert(n >= -1);
    maxRepeat = n;
}


/* Clears the list of matches stored in the regex operator.  Typically done
 * in preparation to try to match the regex to a new string.
 */
void RegexOperator::clearMatches() {
    matches.clear();
}


/* Records a new match of the operator in the list of matches. */
void RegexOperator::pushMatch(const Range &r) {
    matches.push_back(r);
}


/* Reports how many times the regex operator has successfully matched in the
 * string.
 */
int RegexOperator::numMatches() const {
    return (int) matches.size();
}


/* Removes the last match the operator successfully matched against.  Used for
 * backtracking by the regex engine.
 */
Range RegexOperator::popMatch() {
    Range r = matches.back();
    matches.pop_back();
    return r;
}

/* Handles the matching character operation for regex
 */
class MatchChar : public RegexOperator
{
private:
	char c;
public:
	// Char c is matching character
    MatchChar(char c) : RegexOperator() {
        this->c = c;
    }
    
	//Match function
    bool match(const string &s, Range &r) const{
		//Check if at end of string
		if (r.start >= (int)s.size()){
			return false;
		}
		//Check if character matches
        if (s[r.start] == c){
			r.end += 1;
			return true;
		}
		return false;
            
    }
    
    
};

/* Handles the match-any operation for regex
 */
class MatchAny : public RegexOperator
{
public:
    MatchAny() : RegexOperator() { }
    
    bool match(const string &s, Range &r) const{
		//Check if at end of string
		if (r.start >= (int)s.size()){
			return false;
		}
		//Else, matches
        else{
			r.end += 1;
			return true;
		}
    }
    
};

/* Handles the match-subset operation for regex
 */
class  MatchFromSubset : public RegexOperator
{
private:
	string chars;
public:
	// char is the list of characters that can match
    MatchFromSubset(string chars) : RegexOperator(){
        this->chars = chars;
    }
    
	//Check for a match
    bool match(const string &s, Range &r) const{
		//Check if at end of string
		if (r.start >= (int)s.size()){
			return false;
		}
		//Check if the character at r.start in s is in subset
        if (chars.find(s[r.start]) != string::npos){
			r.end += 1;
			return true;
		}
		return false;
    
    }
    
};

/* Handles the exclude-subset operation for regex
 */
class ExcludeFromSubset : public RegexOperator
{
private:
	string chars;
public:
    ExcludeFromSubset(string chars) : RegexOperator(){
        this->chars = chars;
    }
    
	//Check for a match
    bool match(const string &s, Range &r) const{
		//Check if at end of string
		if (r.start >= (int)s.size()){
			return false;
		}
		//Check if the character at r.start in s is not in subset
        if (chars.find(s[r.start]) == string::npos){
			r.end += 1;
			return true;
		}
		return false;
    
    }
    
};

vector<RegexOperator *> parseRegex(const string &expr) {
	vector<RegexOperator *> regexops = vector<RegexOperator *>();
	for (int i = 0; i < (int)expr.size(); i++){
		char c = expr[i];
		if (c == '\\'){
			// Check that the backslash not at end of string, a syntax error
			if (i+1 < (int)expr.size()){
				regexops.push_back(new MatchChar(expr[i+1]));
			}
			i++;
		} else if (c == '.') {
			regexops.push_back(new MatchAny());
		} else if (c == '?') {
			regexops.back()->setMinRepeat(0);
		} else if (c == '*') {
			regexops.back()->setMinRepeat(0);
			regexops.back()->setMaxRepeat(-1);
		} else if (c == '+') {
			regexops.back()->setMinRepeat(1);
			regexops.back()->setMaxRepeat(-1);
		} else if (c == '[') {
			// Go to next character
			i++;
			assert(i < (int)expr.size());
			string s = "";
			// Check if exclude character
			bool negate = (expr[i] == '^');
			if (negate){
				i++;
			}
			assert(i < (int)expr.size());
			//Collect all characters in the subset expression
			while (i < (int)expr.size() && expr[i] != ']'){
				c = expr[i];
				if (c == '\\'){
					if (i+1 < (int)expr.size()){
						s += expr[i+1];
					}
					i++;
				}
				else {
					s += expr[i];
				}
				i++;
			}
			if (negate){
				regexops.push_back(new ExcludeFromSubset(s));
			} else {
				regexops.push_back(new MatchFromSubset(s));
			}
		} else {
			regexops.push_back(new MatchChar(c));
		}
	}
	return regexops;
}
void clearRegex(vector<RegexOperator *> regex) {
	regex.clear();
}
