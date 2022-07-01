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
  m.doc()
    =
    "pyQuiri: python module for N-dimensional spatial range/proximity queries (find_closest, kNN, etc)\n"
    "\n"
    "All N-dimensional point coordinates in pyQuiri are represented via\n"
    "lists or tuples of N doubles, ints, etc. Input values can be either\n"
    "tuples or lists; points returned by pyQuiri will be returned as lists.\n"
    "\n"
    "pyQuiri can handle arbitrary dimensionality, but queries and data\n"
    "must always match the dimensionality of the kd-tree that it operates with\n"
    "(i.e. you can have both 2-, 3-, and 5-dimensional trees in your\n"
    "program at the same time, but a tree created over 5-dimensional data\n"
    "will only accept 5-dimensional data as both inputs and query coordinates)\n"
    "\n"
    "Classes:\n"
    "========\n"
    "\n"
    "    KDTree\n"
    "\n"
    "Key methods to set up query operations:\n"
    "=======================================\n"
    "\n"
    "    pyQuiri.kd_tree(N) -> creates a new KDTree object for N-dimensional data\n"
    "\n"
    "    KDTree.add([coords],value) -> adds a new ([coords],value) pair\n"
    "\n"
    "    KDTree.build() -> prepares the tree for executing queries\n"
    "\n"
    "Query operations on a KDTree:\n"
    "=============================\n"
    "\n"
    "    KDTree.find([query_coords]) -> list of value(s) at these exact coords\n"
    "\n"
    "    KDTree.find_closest([query_coords]) -> ([coords],[value(s)])\n"
    "        => finds the closest data point, and returns both\n"
    "           that point and all value(s) at that point\n"
    "\n"
    "    KDTree.kNN(k,[query_coords],maxRange=inf) -> [ ([coords],[value(s)]) ]\n"
    "        => runs a kNN query with given k and (optionally) maximum search radius.\n"
    "           Returns a list of (coords:value) pairs that is sorted by distance.\n"
    "           In case of more than one element at exactly the same distance the\n"
    "           result list *can* contain more than k elements\n"
    "\n"
    "    KDTree.all_points_in_range([coords_lower],[coords_upper]) -> ([coords],value])\n"
    "        => finds all point:value pairs within given box, and returns those in a list\n"
    "\n"
    "    KDTree.all_values_in_range([coords_lower],[coords_upper]) -> ([coords],value])\n"
    "        => same as all_points_in_range, but returns only the values.\n"
    ;

  m.def("kd_tree", &pyq::KDTree::create,
        "creates a new k-dimenional kd-tree object'");

  // -------------------------------------------------------
  auto kdTree
    = py::class_<pyq::KDTree,
                 std::shared_ptr<pyq::KDTree>>(m, "KDTree");
  kdTree.doc() = "k-d tree data structure that can store and perform various queries on k-dimensional point/value data"
    ;
  
  kdTree.def
    ("add",
     &pyq::KDTree::add,
     "Adds a new (coordinates,object) tuple to the tree");
  kdTree.def
    ("build",
     &pyq::KDTree::build,
     "(re-)builds the kd-tree to prepare it for performing query operations");
  kdTree.def
    ("find",
     &pyq::KDTree::find,
     "returns a list of all the elements whose coordinates match the queried coordinates"
     ,py::arg("query_point")
     );
  kdTree.def
    ("find_closest",
     &pyq::KDTree::findClosest,
     "find closest data point(s), and return tuple [coords, (values)].");
  kdTree.def
    ("all_values_in_range",
     &pyq::KDTree::allValuesInRange,
     "finds all values in given query range (ie, in a k-dimensional box).");
  kdTree.def
    ("all_points_in_range",
     &pyq::KDTree::allPointsInRange,
     "finds all points in given query range (ie, in a k-dimensional box).");
  kdTree.def
    ("knn",
     &pyq::KDTree::kNN,
     "find k-nearest neighbors (kNN) to a query point.",
     py::arg("k"),
     py::arg("query_point"),
     py::arg("max_radius")=std::numeric_limits<double>::infinity());
  
}
