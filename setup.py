# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
import setuptools

__version__ = "0.0.3"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.

ext_modules = [
    Pybind11Extension(
        "word_levenshtein_cpp",
        ["src/main.cpp"],
        define_macros=[("VERSION_INFO", __version__)],
    ),
]

with open("README.md", "r") as file_object:
    LONG_DESCRIPTION = file_object.read()

setuptools.setup(
    name="word_levenshtein",
    version=__version__,
    author="Quan Wang",
    author_email="quanw@google.com",
    url="https://github.com/wq2012/word_levenshtein",
    description="Levenshtein in C++ for Python",
    long_description=LONG_DESCRIPTION,
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    packages=setuptools.find_packages(),
)
