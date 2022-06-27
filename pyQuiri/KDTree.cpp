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
  }

  KDTree::Node::SP KDTree::buildRec(std::vector<int> &items)
  {
    if (items.empty()) return {};
    
    Box bounds(K);
    for (auto id : items)
      bounds.grow(coords[id]);
    
    if (items.size() == 1 || bounds.lower == bounds.upper) {
      KDTree::Node::SP node = std::make_shared<KDTree::Node>();
      node->items = items;
      node->splitDim = -1;
      return node;
    }

    int splitDim = widestDimension(bounds);
    double mid = 0.5*(bounds.lower[splitDim]+bounds.upper[splitDim]);
    int closestItem = -1;
    double closestDist = std::numeric_limits<double>::infinity();
    for (auto item : items) {
      double dist = abs(coords[item][splitDim] - mid);
      if (dist < closestDist) {
        closestDist = dist;
        closestItem = item;
      }
    }

    std::vector<int> left, right, same;
    const Coords &nodeCoords = this->coords[closestItem];
    for (auto item : items) {
      const Coords &coords = this->coords[item];
      if (coords == nodeCoords)
        same.push_back(item);
      else if (coords[splitDim] < nodeCoords[splitDim])
        left.push_back(item);
      else
        right.push_back(item);
    }
    items.clear();
    
    KDTree::Node::SP node = std::make_shared<KDTree::Node>();
    node->splitDim = splitDim;
    node->items    = same;
    node->lChild   = buildRec(left);
    node->rChild   = buildRec(right);
    return node;
  }
  
  /*! build kd-tree - MUST be done before querying anything */
  void KDTree::build()
  {
    if (root)
      // tree is already built!
      return;
    
    std::vector<int> items(objects.size());
    for (int i=0;i<objects.size();i++)
      items[i] = i;
    root = buildRec(items);
  }

  /*! performs (exact) element search for the given coordinates and
    returns all elemnets (in un-specified order) that match these
    coordinates */
  pybind11::list KDTree::find(const std::vector<double> &_coords)
  {
    Coords queryCoords = _coords;
    if (queryCoords.size() != K)
      throw pybind11::type_error
        ("key in KDTree::find() does not match dimensionality of tree");
    
    pybind11::list result;
    if (!root)
      throw std::runtime_error("pyQuiri::KDTree hasn't been built yet (-> kdTree.build()).");

    Node::SP node = root;
    while (node) {
      const Coords &nodeCoords = this->coords[node->items[0]];
      if (nodeCoords == queryCoords) {
        for (auto item : node->items)
          result.append(objects[item]);
        return result;
      } else if (queryCoords[node->splitDim] < nodeCoords[node->splitDim])
        node = node->lChild;
      else
        node = node->rChild;
    }
    return result;
  }
    
    /*! returns a list with (only) the values value of all poitnts within given box */
  pybind11::list KDTree::all_values_in_range(const std::vector<double> &_lower,
                                             const std::vector<double> &_upper)
  {
    PYQ_NOTIMPLEMENTED;
  }
  
  

  /*! add a new element to this kdtree */
  void KDTree::add(const std::vector<double> &coords,
                   const pybind11::object    &object)
  {
    if (coords.size() != K)
      throw pybind11::type_error
        ("key in KDTree::add() does not match dimensionality of tree");
    
    this->coords.push_back(Coords(coords));
    this->objects.push_back(object);
    
    // invalidate the kd-tree:
    root = {};
  }

}
