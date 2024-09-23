#include <pybind11/pybind11.h>
#include "levenshtein.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(levenshtein, m) {

    m.def("levenshtein_with_edits_string", &levenshtein::levenshtein_with_edits_string, R"pbdoc(
        Edit distance of two sequences.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}