# pyQuiry (as in "Daiquiri") - A Module for Fast Python Primitive Range/Proximity Queries

This is a first test repo for a library that will do (reasonably)
fast primitmive/proximity queries (such as k-nearest neighbors)
in python.


# Status

For now still in the stage of figuring out how to build native C++
libraries with pybind11 in a way that they can be
distributed/installed w/ pip.

# Internal Notes for Myself:

## general notes on how to create a pip project (used in first version)

follow this one: https://dzone.com/articles/executable-package-pip-install

``` bash
sudo python3 -m pip install --upgrade testresources pip setuptools wheel
sudo python3 -m pip install tqdm
sudo python3 -m pip install --user --upgrade twine
```

Then compiling:

``` bash
python3 setup.py bdist_wheel
```

Installing locally:
``` bash
python3 -m pip install dist/<mystuff>.whl
```

... and uploading to pypi/pip repo
``` bash
python3 -m twine upload dist/*
```



## direct install through `pip install <clonedRepo>` w/ `pybind11`

following the sample project from `https://github.com/pybind/python_example`
(note this is somewhat outdated)

main issue(s):

- requires pybind11 through pip, not as git submodule

- doesn't use cmake to build; not sure how to deal w/ multiple input files?

