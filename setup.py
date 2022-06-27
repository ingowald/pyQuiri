#!/usr/bin/env python3

from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

__version__ = "0.1.1"

ext_modules = [
    Pybind11Extension(
        "python_example",
        sorted(glob("src/*.cpp")),  # Sort source files for reproducibility
    ),
    ]

setup(name="pyquiri",
   version=__version__,
   author="Ingo Wald",
   author_email="ingowald@gmail.com",
   url="https://gitlab.com/ingowald/pyQuiri",
   description="A test version of pyQuiri that uses pybind11",
   long_description="",
   ext_modules=ext_modules,
   extras_require={"test": "pytest"},
   # Currently, build_ext only provides an optional "highest supported C++
   # level" feature, but in the future it may provide more features.
#   cmdclass={"build_ext": build_ext},
   zip_safe=False)




