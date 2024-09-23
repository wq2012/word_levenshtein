import unittest
import word_levenshtein


class LevenshteinTest(unittest.TestCase):

  def test_str_split(self):
    splitted = word_levenshtein.str_split("a b c", " ")
    self.assertEqual(splitted, ["a", "b", "c"])

if __name__ == "__main__":
  unittest.main()
