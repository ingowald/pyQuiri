# pyQuiry (as in "Daiquiri") - A Python/pip Module for N-dimensional Primitive Range/Proximity/kNN/etc Queries

TLDR: `pyQuiri` is a pip-installable module for some quick yet 
reasonably-fast N-dimensional query operatoins such as k-nearest neighbors.

This project started mostly as a means of myself learning how to
create python bindings and pip-installable projects.

## Installing

Latest released version of this library should be installable via
`pip`.  For instructions on how to build from source see below.

## Bug reports and feature requests

For any found bugs or feature requests, please use the
github issue tracker at [https://github.com/ingowald/pyQuiri]

## pyQuiri Functionality

All N-dimensional point coordinates in pyQuiri are represented via
lists or tuples of N doubles, ints, etc. Input values can be either
tuples or lists; points returned by pyQuiri will be returned as lists.

pyQuiri can handle arbitrary dimensionality, but queries and data
must always match the dimensionality of the kd-tree that it operates with
(i.e. you can have both 2-, 3-, and 5-dimensional trees in your
program at the same time, but a tree created over 5-dimensional data
will only accept 5-dimensional data as both inputs and query coordinates)

```
Classes:
========

    KDTree

Key methods to set up query operations:
=======================================

    pyQuiri.kd_tree(N) -> creates a new KDTree object for N-dimensional data

    KDTree.add([coords],value) -> adds a new ([coords],value) pair

    KDTree.build() -> prepares the tree for executing queries

Query operations on a KDTree:
=============================

    KDTree.find([query_coords]) -> list of value(s) at these exact coords

    KDTree.find_closest([query_coords]) -> ([coords],[value(s)])
        => finds the closest data point, and returns both
           that point and all value(s) at that point

    KDTree.kNN(k,[query_coords],maxRange=inf) -> [ ([coords],[value(s)]) ]
        => runs a kNN query with given k and (optionally) maximum search radius.
           Returns a list of (coords:value) pairs that is sorted by distance.
           In case of more than one element at exactly the same distance the
           result list *can* contain more than k elements

    KDTree.all_points_in_range([coords_lower],[coords_upper]) -> ([coords],value])
        => finds all point:value pairs within given box, and returns those in a list

    KDTree.all_values_in_range([coords_lower],[coords_upper]) -> ([coords],value])
        => same as all_points_in_range, but returns only the values.

```

## Building

*Usually* you shouldn't need to build this project from source, and
should install it through pip instead. If you *do* want to build
directly from the github sources, this section describes how.

### Building as a pip project

To make this project compatible with pyPi and installable via pip I
followed the instructions from here
[https://dzone.com/articles/executable-package-pip-install].

To do that, you have to have a set of python tools installed, which you
can do via:

``` bash
sudo python3 -m pip install --upgrade testresources pip setuptools wheel
sudo python3 -m pip install tqdm
sudo python3 -m pip install --user --upgrade twine
```

Once those tools are available on your machines (I assume you have a C++ compiler etc)
you can then compile the project as a 'wheel' as follows:

``` bash
python3 setup.py bdist_wheel
```

To install that wheel locally:
``` bash
python3 -m pip install dist/<mystuff>.whl
```

And to upload to pypi where pip can find it (well, you'll probably have to be
me to do that :-) ):
``` bash
python3 -m twine upload dist/*
```

### Building locally w/o pip

Instead of buiding or installing this as a pip project you can also
(very useful during development...) build this project locally as a 
python-loadable shared library. To do this:

- build the source code via CMake (ie, `mkdir bin; cd bin; cmake ..; make` ... this should result in some `pyQuiri.cpython-....so` file, or something like this)

- make sure your `PYTHONPATH` contains the dir that contains that shared library.


## Shout-out

This library wouldn't be possible without Wenzel Jakob's `pybind11`
project - I thought template metaprogramming to be pure evil, but man,
with pybind11 you changed my worldview...

Also, thanks to NateM for giving me the high-level rundown of how pypi
and wheels work (I still shudder at it); and at the authors
of the above dzone article that explains how to build pip projects.
