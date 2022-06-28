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
#include <stack>

namespace pyq {

  KDTree::KDTree(int K)
    : K(K)
  {
  }

    /*! checks that tree is built, and throws an exception if not */
  void KDTree::verifyTreeIsBuilt()
  {
    if (!root)
      throw std::runtime_error("pyQuiri::KDTree hasn't been built yet (-> kdTree.build()).");
  }

  /*! converts a std::vector<double> to a Coords class, and verifies
    that this has the same dimensionality of this tree - and
    throws an exception if thi sis not the case */
  Coords KDTree::makeCheckCoords(const std::vector<double> &_coords)
  {
    if (_coords.size() != K)
      throw py::type_error
        ("key in KDTree::find() does not match dimensionality of tree");
    return Coords(_coords);
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
    returns all elements (in un-specified order) that match these
    coordinates */
  py::list KDTree::find(const std::vector<double> &_coords)
  {
    verifyTreeIsBuilt();
    const Coords queryCoords = makeCheckCoords(_coords);
    
    py::list result;

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
  py::list KDTree::all_values_in_range(const std::vector<double> &_lower,
                                       const std::vector<double> &_upper)
  {
    PYQ_NOTIMPLEMENTED;
  }
  
  
  /*! finds the closest data point to given query point, and returns a
    tuple [ point, (values) ]; the 'values' is a *list* of all the
    values that share that data point (ie, it is always a list even if
    the input data set did not contain any duplicates) */
  std::tuple<std::vector<double>,py::list>
  KDTree::findClosest(const std::vector<double> &_coords,
                      const py::kwargs &kwargs)
  {
    if (objects.empty())
      return std::tuple<std::vector<double>,py::list>();
    
    verifyTreeIsBuilt();
    const Coords queryCoords = makeCheckCoords(_coords);
    
    std::stack<std::pair<double,Node::SP>> nodeStack;
    nodeStack.push(std::pair<double,Node::SP>{ 0.,root });
    
    Node::SP closestNode;
    double   closestDist = std::numeric_limits<double>::infinity();
    while (!nodeStack.empty()) {
      double subTreeMinDist = nodeStack.top().first;
      Node::SP node  = nodeStack.top().second;
      nodeStack.pop();

      if (subTreeMinDist >= closestDist)
        continue;
      
      assert(node);
      const Coords &nodeCoords = this->coords[node->items[0]];
      double dist = distance(nodeCoords,queryCoords);
      if (dist <= closestDist) {
        closestDist = dist;
        closestNode = node;
      }
      const double farSideMinDist = 
        std::max(subTreeMinDist,
                 abs(queryCoords[node->splitDim]-nodeCoords[node->splitDim]));
      const bool queryOnLeftSide = (queryCoords[node->splitDim] < nodeCoords[node->splitDim]);
      Node::SP closeChild = queryOnLeftSide?node->lChild:node->rChild;
      Node::SP farChild   = queryOnLeftSide?node->rChild:node->lChild;
      if (farChild)   nodeStack.push({farSideMinDist,farChild});
      if (closeChild) nodeStack.push({subTreeMinDist,closeChild});
    }
    
    if (!closestNode)
      return std::tuple<std::vector<double>,py::list>();
    
    const Coords &foundCoords = this->coords[closestNode->items[0]];
    py::list values;
    for (auto item : closestNode->items)
      values.append(this->objects[item]);
    
    return std::tuple<std::vector<double>,py::list>(foundCoords.coords,values);
  }

  /*! add a new element to this kdtree */
  void KDTree::add(const std::vector<double> &coords,
                   const py::object    &object)
  {
    if (coords.size() != K)
      throw py::type_error
        ("key in KDTree::add() does not match dimensionality of tree");
    
    this->coords.push_back(Coords(coords));
    this->objects.push_back(object);
    
    // invalidate the kd-tree:
    root = {};
  }

}
