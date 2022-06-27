

#add_library(test
#  bindings.cpp
#  kdtree.cpp
#  )
pybind11_add_module(pyQuiri
  bindings.cpp
  KDTree.cpp
  )
target_include_directories(pyQuiri PUBLIC ${PROJECT_SOURCE_DIR})

