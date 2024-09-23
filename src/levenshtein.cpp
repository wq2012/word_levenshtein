#include "levenshtein.h"

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "absl/container/btree_map.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"


namespace levenshtein {

const float kMaxCost = 1e9;

int levenshtein_with_edits(absl::string_view ref, absl::string_view hyp,
                           absl::string_view delimiter,
                           std::vector<std::pair<int, int>>* align) {
  std::vector<std::string> s1 = absl::StrSplit(ref, delimiter);
  std::vector<std::string> s2 = absl::StrSplit(hyp, delimiter);
  int n1 = s1.size();
  int n2 = s2.size();
  std::vector<std::vector<int> > costs(n1+1);
  std::vector<std::vector<EditOp> > backptr(n1+1);
  for (int i = 0; i < n1+1; ++i) {  // ref
    costs[i].resize(n2+1, 0);
    backptr[i].resize(n2+1, Correct);
    costs[i][0] = i;  // deletions
  }
  for (int j = 0; j < n2; ++j) {  // hyp
    costs[0][j+1] = j+1;  // insertions
    for (int i = 0; i < n1; ++i) {  // ref
      // (i,j) <- (i,j-1)
      int ins = costs[i+1][j] + 1;
      // (i,j) <- (i-1,j)
      int del = costs[i][j+1] + 1;
      // (i,j) <- (i-1,j-1)
      int sub = costs[i][j] + (s1[i] != s2[j]);
      costs[i + 1][j + 1] = std::min({ins, del, sub});
      if (costs[i+1][j+1] == ins) {
        backptr[i+1][j+1] = Insertion;
      } else if (costs[i+1][j+1] == del) {
        backptr[i+1][j+1] = Deletion;
      } else if (s1[i] == s2[j]) {
        backptr[i+1][j+1] = Correct;
      } else {
        backptr[i+1][j+1] = Substitution;
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
    if (i >=0 && j >= 0) {
      ed_op = backptr[i][j];
    }
    if (i >=0 && j < 0) {
      ed_op = Deletion;
    }
    if (i < 0 && j >= 0) {
      ed_op = Insertion;
    }
    if (i < 0 && j < 0) {
      LOG(FATAL) << "Invalid alignment";
    }
    if (ed_op == Insertion) {
      align->emplace_back(std::make_pair(-1, j-1));
      --j;
    } else if (ed_op == Deletion) {
      align->emplace_back(std::make_pair(i-1, -1));
      --i;
    } else {
      align->emplace_back(std::make_pair(i-1, j-1));
      --i;
      --j;
    }
  }
  std::reverse(align->begin(), align->end());
  return costs[n1][n2];
}

std::string levenshtein_with_edits_string(absl::string_view ref,
                                          absl::string_view hyp,
                                          absl::string_view delimiter) {
  std::vector<std::pair<int, int> > align;
  int edit_distance = levenshtein_with_edits(ref, hyp, delimiter,
                                             &align);
  std::string align_str;
  for (int i = 0; i < align.size(); ++i) {
    if (i != 0) {
      absl::StrAppend(&align_str, "_");
    }
    absl::StrAppend(&align_str, align[i].first, "_", align[i].second);
  }
  return absl::StrCat(edit_distance, " ", align_str);
}

std::string encode_path(absl::Span<const StdArc> arcs) {
  // Encode path as string. Arcs are separated by " " and we return
  // for each arc ilabel, olabel, and weight, all separated by "_".
  // This is to make our life easier for cython.
  std::ostringstream oss;
  for (const StdArc &a : arcs)
      oss << a.ilabel << "_" << a.olabel << "_" << a.weight.Value() << " ";
  return oss.str();
}


}  // namespace levenshtein
