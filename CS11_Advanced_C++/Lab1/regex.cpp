#include "regex.h"
#include <iostream>
#include <memory>
#include <vector>


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

/* Construct a MatchChar regex operator that matches c.
 */
MatchChar::MatchChar(char c) {
    match_char = c;
}

/* Match the character at s[r.start] to the operator's match
 * char. If there is a match, return true, else false.
 */
bool MatchChar::match(const string &s, Range &r) const {
    if ((int)s.length() > r.start) {
        if (s[r.start] == match_char) {
            r.end = r.start + 1;
            return true;
        }
    }
    return false;
}

// MatchAny definition
MatchAny::MatchAny() { }

/* Match any character as long as r points to a valid
 * index in the string s.
 */
bool MatchAny::match(const string &s, Range &r) const {
    if ((int)s.length() > r.start) {
        r.end = r.start + 1;
        return true;
    }
    return false;
}

/* Construct MatchFromSubset to match the characters in s
 */
MatchFromSubset::MatchFromSubset(string s) {
    chars = s;
}

/* Check if character at s[r.start] is in the match subset
 * of characters, and if so return true, else false.
 */
bool MatchFromSubset::match(const string &s, Range &r) const {
    if ((int)s.length() > r.start) {
        if (chars.find(s[r.start]) != string::npos) {
            r.end = r.start + 1;
            return true;
        }
    }
    return false;
}

/* Construct ExcludeFromSubset regex operator with given
 * characters in string as the set to exclude in match.
 */
ExcludeFromSubset::ExcludeFromSubset(string s) {
    chars = s;
}

/* Check if character at s[r.start] is not in
 * the excluded subset associated with the
 * ExcludeFromSubset regex operator. 
 */
bool ExcludeFromSubset::match(const string &s, Range &r) const {
    if ((int)s.length() > r.start) {
        if (chars.find(s[r.start]) == string::npos) {
            r.end = r.start + 1;
            return true;
        }
    }
    return false;
}


/* Parse an input string into regex tokens.
 *
 * This iterates through the passed string and returns
 * a vector of regex operators that correspond to the string.
 */
vector<RegexOperator *> parseRegex(const string &expr) {
    vector<RegexOperator *> regex_ops {};
    bool escaped = false;
    for (size_t i = 0; i < expr.length(); i++) {
        if (escaped) {
            regex_ops.push_back(new MatchChar(expr[i]));
            escaped = false;
        }
        else {
            char c = expr[i];
            switch (c)
            {
                case '\\':
                    // Set parser into escaped mode
                    // and continue onto next character
                    // in escaped mode.
                    escaped = true;
                    break;
                case '.':
                    // Any-match special character read
                    regex_ops.push_back(new MatchAny());
                    break;
                case '+':
                    // Match previous 1 or more times
                    regex_ops.back()->setMinRepeat(1);
                    regex_ops.back()->setMaxRepeat(-1);
                    break;
                case '*':
                    // Match previous 0 or more times
                    regex_ops.back()->setMinRepeat(0);
                    regex_ops.back()->setMaxRepeat(-1);
                    break;
                case '?':
                    // Match previous 0 or 1 times
                    regex_ops.back()->setMinRepeat(0);
                    break;
                case '[':
                    // Begin parsing a set of characters.
                    {
                        bool exclude = false;
                        string char_set = "";
                        // Parse through until reaching a ] character
                        // terminating the character set definition.
                        for (; i < expr.length(); i++) {
                            if (expr[i] == ']') {
                                // Reached end of set definition.
                                break;
                            }
                            else if (expr[i] == '^') {
                                // This set is an exclude set.
                                exclude = true;
                            } else {
                                // Normal character, add to set.
                                char_set += expr[i];
                            }
                        }
                        // Add the subset regex op.
                        if (exclude) {
                            regex_ops.push_back(new ExcludeFromSubset(char_set));   
                        } else {
                            regex_ops.push_back(new MatchFromSubset(char_set));  
                        }
                    }
                    break;
                default:
                    // Character wasn't special, match it as a normal
                    // character.
                    regex_ops.push_back(new MatchChar(c));
                    break;
            }
        }
    }
    return regex_ops;
}

/* Clear all matches for the given regex.
 */
void clearRegex(vector<RegexOperator *> regex) {
    for (size_t i = 0; i < regex.size(); i++) {
        regex[i]->clearMatches();
    }
}
