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
#include <deque>
#include <queue>

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
  std::vector<py::object> KDTree::find(const std::vector<double> &_coords)
  {
    verifyTreeIsBuilt();
    const Coords queryCoords = makeCheckCoords(_coords);
    
    std::vector<py::object> result;
    
    Node::SP node = root;
    while (node) {
      const Coords &nodeCoords = this->coords[node->items[0]];
      if (nodeCoords == queryCoords) {
        for (auto item : node->items)
          result.push_back(objects[item]);
        return result;
      } else if (queryCoords[node->splitDim] < nodeCoords[node->splitDim])
        node = node->lChild;
      else
        node = node->rChild;
    }
    return result;
  }
    
  /*! returns a list with (only) the values value of all poitnts within given box */
  std::vector<py::object>
  KDTree::allValuesInRange(const std::vector<double> &_lower,
                           const std::vector<double> &_upper)
  {
    if (objects.empty())
      return {};
    
    verifyTreeIsBuilt();
    Box queryBox(makeCheckCoords(_lower),
                 makeCheckCoords(_upper));
    
    std::vector<py::object> result;
    std::stack<std::pair<Box,Node::SP>> nodeStack;
    nodeStack.push({Box::infinite(K),root});
    while (!nodeStack.empty()) {
      // pop latest from stack
      Box subtreeBounds = nodeStack.top().first;
      Node::SP node  = nodeStack.top().second;
      nodeStack.pop();

      // cull if not in range
      if (!overlaps(subtreeBounds,queryBox))
        continue;

      // process node itself
      const Coords &nodeCoords = this->coords[node->items[0]];
      if (overlaps(queryBox,nodeCoords))
        for (auto item : node->items)
          result.push_back(this->objects[item]);

      // push children
      if (node->lChild) {
        Box childBounds = subtreeBounds;
        childBounds.upper[node->splitDim] = nodeCoords[node->splitDim];
        nodeStack.push(std::pair<Box,Node::SP>{childBounds,node->lChild});
      }
      if (node->rChild) {
        Box childBounds = subtreeBounds;
        childBounds.lower[node->splitDim] = nodeCoords[node->splitDim];
        nodeStack.push(std::pair<Box,Node::SP>{childBounds,node->rChild});
      }
    }
    
    return result;//py::cast<py::list>(result);
  }


  /*! returns a list with (only) the values value of all poitnts within given box */
  std::vector<std::pair<std::vector<double>,py::object>> 
  KDTree::allPointsInRange(const std::vector<double> &_lower,
                           const std::vector<double> &_upper)
  {
    if (objects.empty())
      return {};//py::list{};
    
    verifyTreeIsBuilt();
    Box queryBox(makeCheckCoords(_lower),
                 makeCheckCoords(_upper));
    // std::vector<py::object> result;
    // py::list result;
    std::vector<std::pair<std::vector<double>,py::object>> result;
    
    std::stack<std::pair<Box,Node::SP>> nodeStack;
    nodeStack.push({Box::infinite(K),root});
    while (!nodeStack.empty()) {
      // pop latest from stack
      Box subtreeBounds = nodeStack.top().first;
      Node::SP node  = nodeStack.top().second;
      nodeStack.pop();

      // cull if not in range
      if (!overlaps(subtreeBounds,queryBox))
        continue;

      // process node itself
      const Coords &nodeCoords = this->coords[node->items[0]];
      if (overlaps(queryBox,nodeCoords))
        for (auto item : node->items) {
          result.push_back({nodeCoords.coords,this->objects[item]});
        }

      // push children
      if (node->lChild) {
        Box childBounds = subtreeBounds;
        childBounds.upper[node->splitDim] = nodeCoords[node->splitDim];
        nodeStack.push(std::pair<Box,Node::SP>{childBounds,node->lChild});
      }
      if (node->rChild) {
        Box childBounds = subtreeBounds;
        childBounds.lower[node->splitDim] = nodeCoords[node->splitDim];
        nodeStack.push(std::pair<Box,Node::SP>{childBounds,node->rChild});
      }
    }

    return result;
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

  /*! find k-nearest neighbors (kNN) to a query point */
  std::vector<std::tuple<std::vector<double>,py::object>>
  KDTree::kNN(int k,
              const std::vector<double> &_coords,
              double initMaxRadius)
  {
    if (objects.empty())
      return {};
    
    verifyTreeIsBuilt();
    const Coords queryPoint = makeCheckCoords(_coords);

    /* the list of current candidates at any point during traversal -
       will get updated with new nodes as they get found (possibly
       evicting other ones). Careful: nodes can contain more than one
       data point, so we need to separately track how many values we
       have in there */
    std::priority_queue<std::pair<double,Node::SP>> currentCandidateNodes;
    int    numValuesInCandidates = 0;
    double currentMaxRadius = initMaxRadius;

    /* the node(s) we still need to check for additional candidates */
    std::stack<std::pair<Box,Node::SP>> nodeStack;
    nodeStack.push({Box::infinite(K),root});
    while (!nodeStack.empty()) {
      // pop latest from stack
      Box subtreeBounds = nodeStack.top().first;
      Node::SP node  = nodeStack.top().second;
      nodeStack.pop();

      const double distToSubtree
        = distance(subtreeBounds,queryPoint);
      if (distToSubtree >= currentMaxRadius)
        // cull if entire subtree already out of range
        continue;

      // process node itself
      const Coords &nodeCoords = this->coords[node->items[0]];
      const double distToPoint = distance(nodeCoords,queryPoint);
      if (distToPoint <= currentMaxRadius) {
        // first, add this point, with all its values
        currentCandidateNodes.push({distToPoint,node});
        numValuesInCandidates += node->items.size();
        while (true) {
          const int numValuesInFurthestCandidate
            = currentCandidateNodes.top().second->items.size();
          if (numValuesInCandidates-numValuesInFurthestCandidate >= k) {
            /*! even if we drop the furthest one, we'd still have enough! */
            numValuesInCandidates -= numValuesInFurthestCandidate;
            currentCandidateNodes.pop();
            continue;
          }
          break;
        }
        if (numValuesInCandidates >= k)
          currentMaxRadius = currentCandidateNodes.top().first;
      }

      // push children
      if (node->lChild) {
        Box childBounds = subtreeBounds;
        childBounds.upper[node->splitDim] = nodeCoords[node->splitDim];
        const double dist = distance(childBounds,queryPoint);
        if (dist <= currentMaxRadius)
          nodeStack.push({childBounds,node->lChild});
      }
      if (node->rChild) {
        Box childBounds = subtreeBounds;
        childBounds.lower[node->splitDim] = nodeCoords[node->splitDim];
        const double dist = distance(childBounds,queryPoint);
        if (dist <= currentMaxRadius)
          nodeStack.push({childBounds,node->rChild});
      }
    }

    std::vector<int> finalCandidates;
    while (!currentCandidateNodes.empty()) {
      for (auto item : currentCandidateNodes.top().second->items)
        finalCandidates.push_back(item);
      currentCandidateNodes.pop();
    }
    // reverse the list, so we'll report result sorted by distance, closest comes first
    for (int i=0;i<finalCandidates.size()/2;i++)
      std::swap(finalCandidates[i],finalCandidates[finalCandidates.size()-1-i]);

    std::vector<std::tuple<std::vector<double>,py::object>> result;
    for (auto item : finalCandidates) {
      double dist = distance(this->coords[item],queryPoint);
      std::tuple<std::vector<double>,py::object> entry
        (this->coords[item].coords,
         this->objects[item]);
      result.push_back(entry);
    }
    
    return result;    
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
