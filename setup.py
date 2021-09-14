#!/usr/bin/env python3

import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()


setuptools.setup(
     name='pyquiri',  
     version='0.1',
     scripts=['pyqTest'] ,
     author="Ingo Wald",
     author_email="ingowald@gmail.com",
     description="A first test repo for a python 'geometry queries' library",
     long_description=long_description,
   long_description_content_type="text/markdown",
     url="https://gitlab.com/ingowald/pyQuiri",
     packages=setuptools.find_packages(),
     classifiers=[
         "Programming Language :: Python :: 3",
         "License :: OSI Approved :: MIT License",
         "Operating System :: OS Independent",
     ],
 )
