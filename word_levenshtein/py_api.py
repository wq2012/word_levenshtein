import word_levenshtein_cpp

def levenshtein_with_edits(ref: str, hyp: str
) -> tuple[int, list[tuple[int, int]]]:
  str_result = word_levenshtein_cpp.levenshtein_with_edits(
      ref, hyp, " ")
  distance_str, align_str = str_result.split(" ")
  align_parts = align_str.split("_")
  align = []
  for part in align_parts:
    align.append(tuple(map(int, part.split(","))))
  return int(distance_str), align
