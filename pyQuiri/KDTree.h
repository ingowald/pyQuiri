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

#pragma once

#include "pyQuiri/common.h"

namespace pyq {

  typedef std::shared_ptr<pybind11::object> PyHandle;
  
  struct KDTree {
    typedef std::shared_ptr<KDTree> SP;

    KDTree(int K);
    
    static SP create(int K) { return std::make_shared<KDTree>(K); }
    
    /*! add a new element to this kdtree */
    void add(const double *coords, PyHandle object);
    
    /*! build kd-tree - MUST be done before querying anything */
    void build();
    
    std::vector<double> coords;
    std::vector<PyHandle> objects;

    /*! the number of dimensions */
    const int K;
    bool is_built = false;
  };
  
}

