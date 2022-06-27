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

namespace pyq {

  KDTree::KDTree(int K)
    : K(K)
  {
    PING;
  }

  /*! build kd-tree - MUST be done before querying anything */
  void KDTree::build()
  {
    PING;
    is_built = true;
  }

  inline bool KDTree::matches(const std::vector<double> &coords, const int idx)
  {
    for (int i=0;i<K;i++)
      if (this->coords[idx*K+i] != coords[i])
        return false;
    return true;
  }


  /*! performs (exact) element search for the given coordinates -
    returns whatever object got addded at these coordinates, or
    null if it's not in this tree */
  pybind11::object KDTree::find(const std::vector<double> &coords)
  {
    if (coords.size() != K)
      throw pybind11::type_error
        ("key in KDTree::find() does not match dimensionality of tree");
    
    for (int i=0;i<objects.size();i++)
      if (matches(coords,i)) return objects[i];
    throw pybind11::key_error("key not found in KDTree::find");
  }
    

  /*! add a new element to this kdtree */
  void KDTree::add(const std::vector<double> &coords,
                   const pybind11::object    &object)
  {
    if (coords.size() != K)
      throw pybind11::type_error
        ("key in KDTree::add() does not match dimensionality of tree");
    
    for (auto c : coords)
      this->coords.push_back(c);
    this->objects.push_back(object);
    is_built = false;
  }

}
