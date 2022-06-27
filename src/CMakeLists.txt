

#add_library(test
#  bindings.cpp
#  kdtree.cpp
#  )
pybind11_add_module(
  test
  bindings.cpp
  kdtree.cpp
  )
