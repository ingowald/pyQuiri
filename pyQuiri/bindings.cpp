// ======================================================================== //
// Copyright 2022-2022 Ingo Wald                                            //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "pyQuiri/KDTree.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

PYBIND11_MODULE(pyQuiri, m) {

  // optional module docstring
  m.doc() = "pyQuiri: python module for spatial range/proximity queries (find_closest, kNN, etc)";

  m.def("kd_tree", &pyq::KDTree::create,
        "creates a new k-dimenional kd-tree object'");

  // -------------------------------------------------------
  auto kdTree
    = py::class_<pyq::KDTree,
                 std::shared_ptr<pyq::KDTree>>(m, "KDTree");
  kdTree.doc() = "tree that can store and query k-dimensional point/value data";
  
  kdTree.def
    ("add",
     &pyq::KDTree::add,
     "Adds a new object with given coordinates to the tree");
  kdTree.def
    ("build",
     &pyq::KDTree::build,
     "(re-)builds a kd-tree to prepare it for performing query operations");
  kdTree.def
    ("find",
     &pyq::KDTree::find,
     "returns a list of all the elements with given coordinates.");
  kdTree.def
    ("findClosest",
     &pyq::KDTree::findClosest,
     "find closest data point(s), and return tuple [coords, (values)].");
  
}
