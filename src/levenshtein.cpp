#include "levenshtein.h"

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


namespace levenshtein {

const float kMaxCost = 1e9;

std::vector<std::string> StrSplit(std::string text, std::string delimiter) {
  std::vector<std::string> result;
  do {
    size_t pos = text.find(delimiter);
    if (pos == std::string::npos) {
      result.push_back(text);
      break;
    }
    result.push_back(text.substr(0, pos));
    text = text.substr(pos + delimiter.size());
  } while (true);
  return result;
}


int levenshtein_with_edits(std::string ref, std::string hyp,
  std::string delimiter,
  std::vector<std::pair<int, int>>* align) {
  std::vector<std::string> s1 = StrSplit(ref, delimiter);
  std::vector<std::string> s2 = StrSplit(hyp, delimiter);
  int n1 = s1.size();
  int n2 = s2.size();
  std::vector<std::vector<int> > costs(n1 + 1);
  std::vector<std::vector<EditOp> > backptr(n1 + 1);
  for (int i = 0; i < n1 + 1; ++i) {  // ref
    costs[i].resize(n2 + 1, 0);
    backptr[i].resize(n2 + 1, Correct);
    costs[i][0] = i;  // deletions
  }
  for (int j = 0; j < n2; ++j) {  // hyp
    costs[0][j + 1] = j + 1;  // insertions
    for (int i = 0; i < n1; ++i) {  // ref
      // (i,j) <- (i,j-1)
      int ins = costs[i + 1][j] + 1;
      // (i,j) <- (i-1,j)
      int del = costs[i][j + 1] + 1;
      // (i,j) <- (i-1,j-1)
      int sub = costs[i][j] + (s1[i] != s2[j]);
      costs[i + 1][j + 1] = std::min({ ins, del, sub });
      if (costs[i + 1][j + 1] == ins) {
        backptr[i + 1][j + 1] = Insertion;
      }
      else if (costs[i + 1][j + 1] == del) {
        backptr[i + 1][j + 1] = Deletion;
      }
      else if (s1[i] == s2[j]) {
        backptr[i + 1][j + 1] = Correct;
      }
      else {
        backptr[i + 1][j + 1] = Substitution;
      }
    }
  }
  VLOG(1) << "Mincost " << costs[n1][n2];
  int i = n1;
  int j = n2;
  // Emits pairs (n1_pos, n2_pos) where n1_pos is a position in n1 and n2_pos
  // is a position in n2.
  while (i > 0 || j > 0) {
    VLOG(2) << "i: " << i << " j: " << j;
    EditOp ed_op;
    if (i >= 0 && j >= 0) {
      ed_op = backptr[i][j];
    }
    if (i >= 0 && j < 0) {
      ed_op = Deletion;
    }
    if (i < 0 && j >= 0) {
      ed_op = Insertion;
    }
    if (i < 0 && j < 0) {
      LOG(FATAL) << "Invalid alignment";
    }
    if (ed_op == Insertion) {
      align->emplace_back(std::make_pair(-1, j - 1));
      --j;
    }
    else if (ed_op == Deletion) {
      align->emplace_back(std::make_pair(i - 1, -1));
      --i;
    }
    else {
      align->emplace_back(std::make_pair(i - 1, j - 1));
      --i;
      --j;
    }
  }
  std::reverse(align->begin(), align->end());
  return costs[n1][n2];
}

std::string levenshtein_with_edits_string(std::string ref,
  std::string hyp,
  std::string delimiter) {
  std::vector<std::pair<int, int> > align;
  int edit_distance = levenshtein_with_edits(ref, hyp, delimiter,
    &align);
  std::string align_str;
  for (int i = 0; i < align.size(); ++i) {
    if (i != 0) {
      align_str += "_";
    }
    align_str += std::to_string(align[i].first) + "_" + std::to_string(align[i].second);
  }
  return std::to_string(edit_distance) + " " + align_str;
}

}  // namespace levenshtein
