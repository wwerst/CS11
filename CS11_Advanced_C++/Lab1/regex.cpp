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

// MatchChar definition
MatchChar::MatchChar(char c) {
    match_char = c;
}

bool MatchChar::match(const string &s, Range &r) const {
    if (s.length() > r.start) {
        if (s[r.start] == match_char) {
            r.end = r.start + 1;
            return true;
        }
    }
    return false;
}

// MatchAny definition
MatchAny::MatchAny() { }

bool MatchAny::match(const string &s, Range &r) const {
    if (s.length() > r.start) {
        r.end = r.start + 1;
        return true;
    }
    return false;
}

// MatchFromSubset definition
MatchFromSubset::MatchFromSubset(string s) {
    chars = s;
}

bool MatchFromSubset::match(const string &s, Range &r) const {
    if (s.length() > r.start) {
        if (chars.find(s[r.start]) != string::npos) {
            r.end = r.start + 1;
            return true;
        }
    }
    return false;
}

// ExcludeFromSubset definition
ExcludeFromSubset::ExcludeFromSubset(string s) {
    chars = s;
}

bool ExcludeFromSubset::match(const string &s, Range &r) const {
    if (s.length() > r.start) {
        if (chars.find(s[r.start]) == string::npos) {
            r.end = r.start + 1;
            return true;
        }
    }
    return false;
}

// parseRegex function def

vector<RegexOperator *> parseRegex(const string &expr) {
    vector<RegexOperator *> regex_ops {};
    bool escaped = false;
    for (int i = 0; i < expr.length(); i++) {
        if (escaped) {
            RegexOperator* match_char = new MatchChar(expr[i]);
            regex_ops.push_back(match_char);
            escaped = false;
        }
        else {
            char c = expr[i];
            switch (c)
            {
                case '\\':
                    escaped = true;
                    break;
                case '.':
                    {
                        auto* match_any = new MatchAny();
                        regex_ops.push_back(match_any);
                    }
                    break;
                case '+': // Match previous 1 or more times
                    regex_ops.back()->setMinRepeat(1);
                    regex_ops.back()->setMaxRepeat(-1);
                    break;
                case '*': // Match previous 0 or more times
                    regex_ops.back()->setMinRepeat(0);
                    regex_ops.back()->setMaxRepeat(-1);
                    break;
                case '?': // Match previous 0 or 1 times
                    regex_ops.back()->setMinRepeat(0);
                    break;
                case '[': // Set operation
                    {
                        bool exclude = false;
                        string char_set {};
                        for (; i < expr.length(); i++) {
                            if (expr[i] == ']') {
                                break;
                            }
                            else if (expr[i] == '^') {
                                exclude = true;
                            } else {
                                char_set += expr[i];
                            }
                        }
                        if (exclude) {
                            auto* exclude_set = new ExcludeFromSubset(char_set);
                            regex_ops.push_back(exclude_set);   
                        } else {
                            auto* include_set = new MatchFromSubset(char_set);
                            regex_ops.push_back(include_set);  
                        }
                    }
                    break;
                default:
                    auto* match_char = new MatchChar(c);
                    regex_ops.push_back(match_char);
                    break;
            }
        }
    }
    return regex_ops;
}
