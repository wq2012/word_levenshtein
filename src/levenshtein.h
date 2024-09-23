// Implementation of Levenshtein distance with edit operations in C++.

#ifndef LEVENSHTEIN_H_
#define LEVENSHTEIN_H_

#include <string>
#include <utility>
#include <vector>


namespace levenshtein {


// Edit Operations
enum EditOp { Correct = 0, Substitution = 1, Insertion = 2, Deletion = 3 };

// Edit distance of two sequences, represented as <del> separated strings.
// In addition, returns an alignment sequence, which specified for each
// s2 position i, a pair (x,y) where x denotes the position in s1
// and y denotes a position in s2.
int levenshtein_with_edits(std::string ref, std::string hyp,
                           std::string delimiter,
                           std::vector<std::pair<int, int> >* align);

// Python friendly format that returns a single string that contains both
// the edit distance and a string representation of the alignment.
std::string levenshtein_with_edits_string(std::string ref,
                                          std::string hyp,
                                          std::string delimiter);


}  // namespace levenshtein

#endif  // LEVENSHTEIN_H_