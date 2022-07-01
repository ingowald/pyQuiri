

#add_library(test
#  bindings.cpp
#  kdtree.cpp
#  )
pybind11_add_module(pyQuiri
  # the C++ sources
  common.h
  Coords.h
  Box.h
  KDTree.h
  KDTree.cpp

  # the actual python bindings
  bindings.cpp
  )
target_include_directories(pyQuiri PUBLIC ${PROJECT_SOURCE_DIR})

