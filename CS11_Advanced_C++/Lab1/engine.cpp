#include "engine.h"

#include <iostream>


// Set this to 1 if you need to see the output of the regular expression
// matching engine as it attempts to match.  You should not need to do this
// for the assignment, only if you decide to play with the engine itself.
#define VERBOSE 0


/* This helper function implements the core of the regular-expression matching
 * algorithm, a simple backtracking algorithm that will attempt to consume as
 * much of the input string as possible, but will backtrack where it can if it
 * finds it is unable to achieve matches.
 *
 * The function will attempt to find a match starting at the specific index
 * start.
 *
 * If the function cannot generate a match, it will return the range (-1, -1).
 */
Range findAtIndex(vector<RegexOperator *> regex, const string &s, int start) {
    if (VERBOSE) {
        cout << string(78, '-') << endl;
        cout << "Find regex in \"" << s << "\", starting at index " << start
             << endl;
    }
    
    Range matched(start, start);
    // Keep track of the parts of the regex we have applied, so that we can
    // figure out what needs backtracking.
    vector<RegexOperator *> applied;
    int opIndex = 0;
    while (opIndex < (int) regex.size()) {
        assert(opIndex == (int) applied.size());
        
        // Get the next operator to apply.
        RegexOperator *op = regex[opIndex];
        op->clearMatches();
        
        Range currentOp(matched.end, matched.end);

        if (VERBOSE)
            cout << "Attempting to apply operator " << opIndex << endl;

        // Apply the operator as many times as possible, up to the maximum
        // number of repetitions allowed.
        int numMatches = 0;
        while (op->getMaxRepeat() == -1 || numMatches < op->getMaxRepeat()) {
            Range iter(currentOp.end, currentOp.end);
            // If we get a match, record the range that we match on, so that
            // we can backtrack if needed.
            if (op->match(s, iter)) {
                op->pushMatch(iter);

                if (VERBOSE) {
                    cout << " * Matched range [" << iter.start << ", "
                         << iter.end << ")" << endl;
                }

                currentOp.end = iter.end;
                numMatches++;
            }
            else {
                break;
            }
        }

        // If we applied the operator at least as many times as required, then
        // we are good!
        if (numMatches >= op->getMinRepeat()) {
            // Successfully matched this operator!

            if (VERBOSE)
                cout << " * Success" << endl;

            // Record that the operator was applied, and update the
            // "matched range"
            applied.push_back(op);
            matched.end = currentOp.end;
            opIndex++;
        }
        else {
            // Match failure.  Need to backtrack.  If I backtrack to the very
            // beginning, match failed.
            
            if (VERBOSE) {
                cout << " * Failed to match at least " << op->getMinRepeat()
                     << " time(s)" << endl;
                
                cout << "Backtracking" << endl;
            }
            
            while (!applied.empty()) {
                RegexOperator *btOp = applied.back();
                if (btOp->numMatches() > btOp->getMinRepeat()) {
                    // The current operator has been applied more than the
                    // minimum number of times.  Remove one application of
                    // this operation, and retry from that point.
                    
                    if (VERBOSE) {
                        cout << " * Operator " << (opIndex - 1)
                             << " has been applied " << btOp->numMatches()
                             << " times (" << btOp->getMinRepeat()
                             << " required); trying one less" << endl;
                    }

                    Range popped = btOp->popMatch();
                    currentOp.end = popped.start;
                    matched.end = popped.start;

                    break;
                }
                else {
                    // The operator has been applied only the minimum number of
                    // times, but maybe we can't apply the operation at all
                    // yet.  Remove it from the sequence and try again.
                    
                    applied.pop_back();
                    opIndex--;

                    if (VERBOSE)
                        cout << " * Un-applying operator " << opIndex << endl;
                }
            }
            
            if (applied.empty()) {
                // We backtracked all the way to the beginning.  Total match
                // failure; nothing we do will achieve a match.
                
                if (VERBOSE)
                    cout << "Match failed, giving up" << endl;
                
                matched.start = -1;
                matched.end = -1;
                break;
            }
        }
    }

    if (VERBOSE) {
        if (applied.size() == regex.size()) {
            cout << "Match succeeded on range [" << matched.start << ", "
                 << matched.end << ")" << endl;
        }
        
        // cout << string(78, '-') << endl;
    }
    
    return matched;
}

/* Find the first match of regex in the string s
 *
 * This function iterates through each index in string
 * and checks for a match starting at that index. If
 * no match is found, it returns a range of Range(-1, -1).
 */
Range find(vector<RegexOperator *> regex, const string &s) {
    for (size_t i = 0; i < s.length(); i++) {
        auto range = findAtIndex(regex, s, i);
        if (range.start != -1 && range.end != -1) {
            return range;
        }
    }
    return Range(-1, -1);
}

/* Check if a string exactly matches a regex with all
 * characters consumed.
 */
bool match(vector<RegexOperator *> regex, const string &s) {
    auto range = find(regex, s);
    return range.start == 0 && (size_t)range.end == s.length();
}
