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

#include "pyQuiri/Box.h"

namespace pyq {

  typedef std::shared_ptr<py::object> PyHandle;

  struct KDTree {
    typedef std::shared_ptr<KDTree> SP;

    KDTree(int K);
    
    static SP create(int K) { return std::make_shared<KDTree>(K); }

    /*! add a new element to this kdtree */
    // void add(const py::list &coords,
    //          const py::object    &object);
    void add(const std::vector<double> &coords,
             const py::object    &object);
    
    /*! performs (exact) element search for the given coordinates and
      returns all elemnets (in un-specified order) that match these
      coordinates */
    std::vector<py::object> find(const std::vector<double> &coords);

    /*! finds the closest data point to given query point, and returns
      a tuple [ point, (values) ]; the 'values' is a *list* of all
      the values that share that data point (ie, it is always a list
      even if the input data set did not contain any duplicates) */
    std::tuple<std::vector<double>,py::list>
    //    py::tuple
    findClosest(const std::vector<double> &coords, const py::kwargs &kwargs);

    /*! returns a list with all key:value pairs with the given point and radius */
    std::vector<std::tuple<std::vector<double>,py::object>>
    all_points_in_radius(const std::vector<double> &coords,
                         double radius);
    
    /*! find k-nearest neighbors (kNN) to a query point */
    std::vector<std::tuple<std::vector<double>,py::object>>
    kNN(int k,
        const std::vector<double> &coords,
        double maxRadius=std::numeric_limits<double>::infinity());
    
    /*! returns a list with all key:value pairs with the given box */
    std::vector<std::pair<std::vector<double>,py::object>>
    allPointsInRange(const std::vector<double> &lower,
                     const std::vector<double> &upper);
    
    /*! returns a list with (only) the values value of all poitnts within given box */
    std::vector<py::object>
    allValuesInRange(const std::vector<double> &lower,
                     const std::vector<double> &upper);
    
    // /*! returns a list with (only) the values value of all poitnts within given radius */
    // std::vector<py::object>
    // all_values_in_radius(const std::vector<double> &coords,
    //                      double radius);
    
    /*! build kd-tree - MUST be done before querying anything */
    void build();

  private:
    struct Node {
      typedef std::shared_ptr<Node> SP;

      int splitDim;
      
      std::vector<int>      items;
      Node::SP              lChild,rChild;
    };

    Node::SP buildRec(std::vector<int> &objectIDs);

    /*! checks that tree is built, and throws an exception if not */
    void verifyTreeIsBuilt();
    
    /*! converts a std::vector<double> to a Coords class, and verifies
      that this has the same dimensionality of this tree - and
      throws an exception if thi sis not the case */
    Coords makeCheckCoords(const std::vector<double> &);

    /*! one entry per input data point, containing the coordinates of each data point */
    std::vector<Coords>     coords;
    
    /*! one entry per input data point, containing the value for the given data point */
    std::vector<py::object> objects;
    
    /*! the root of the kd-tree, if built; or {} if not */
    Node::SP                root;
    
    /*! the number of dimensions */
    const int K;
  };
  
}

