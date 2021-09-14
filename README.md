# pyQuery - A Module for Fast Python Primitive/Proximity Queries

This is a first test repo for a library that will do (reasonably)
fast primitmive/proximity queries (such as k-nearest neighbors)
in python.


# Status

For now still in the stage of figuring out how to build native C++
libraries with pybind11 in a way that they can be
distributed/installed w/ pip.

# Internal Notes for Myself:

## notes on how to create a pip project

follow this one: https://dzone.com/articles/executable-package-pip-install

``` bash
sudo python3 -m pip install --upgrade testresources pip setuptools wheel
sudo python3 -m pip install tqdm
sudo python3 -m pip install --user --upgrade twine
```
