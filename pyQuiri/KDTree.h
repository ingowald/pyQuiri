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

  typedef std::shared_ptr<py::object> PyHandle;

  struct Coords {
    Coords(int N, double defaultValue = 0.) : coords(N)
    {
      for (auto &v : coords) v = defaultValue;
    }
    Coords(const Coords &other) : coords(other.coords) {}
    Coords(const std::vector<double> &other) : coords(other) {}
    
    inline double &operator[](int i) { return coords[i]; }
    inline double  operator[](int i) const { return coords[i]; }
    inline int size() const { return (int)coords.size(); }
    
    std::vector<double> coords;
  };

  inline void set_min(Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    for (int i=0;i<a.size();i++)
      a[i] = std::min(a[i],b[i]);
  }
  
  inline void set_max(Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    for (int i=0;i<a.size();i++)
      a[i] = std::max(a[i],b[i]);
  }
  
  inline Coords min(const Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    Coords res(a.size());
    for (int i=0;i<res.size();i++)
      res[i] = std::min(a[i],b[i]);
    return res;
  }
  inline Coords max(const Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    Coords res(a.size());
    for (int i=0;i<res.size();i++)
      res[i] = std::max(a[i],b[i]);
    return res;
  }
  inline int arg_max(const Coords &a)
  {
    int res = 0;
    for (int i=1;i<a.size();i++)
      if (a[i] > a[res]) res = i;
    return res;
  }
  inline Coords operator-(const Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    Coords res(a.size());
    for (int i=0;i<a.size();i++)
      res[i] = a[i] - b[i];
    return res;
  }

  inline double dot(const Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    double res = 0.f;
    for (int i=0;i<a.size();i++)
      res += a[i] * b[i];
    return res;
  }
  inline double distance(const Coords &a, const Coords &b)
  {
    Coords diff = a - b;
    return sqrt(dot(diff,diff));
  }


  inline bool operator==(const Coords &a, const Coords &b)
  {
    assert(a.size() == b.size());
    for (int i=0;i<a.size();i++)
      if (a[i] != b[i]) return false;
    return true;
  }
  inline std::ostream &operator<<(std::ostream &out, const Coords &coords)
  {
    out << "(" << coords[0];
    for (int i=1;i<coords.size();i++)
      out << "," << coords[i];
    out << ")";
    return out;
  }
  

  struct Box {
    Box(int N)
      : lower(N, +std::numeric_limits<double>::infinity()),
        upper(N, -std::numeric_limits<double>::infinity())
    {}
    Box(const Box &other) = default;
    Box(const Coords &lower, const Coords &upper)
      : lower(lower), upper(upper)
    {}

    /*! returns an N-dimensoinal, infinite box */
    static Box infinite(int N)
    {
      return Box(Coords(N, -std::numeric_limits<double>::infinity()),
                 Coords(N, +std::numeric_limits<double>::infinity()));
    }
    void grow(const Coords &other) {
      set_min(lower,other);
      set_max(upper,other);
    }
      
    Box including(const Coords &point) const
    { return Box(min(lower,point),max(upper,point)); }

    inline int size() const { return lower.size(); }
    Coords lower, upper;
  };
  inline int widestDimension(const Box &box) { return arg_max(box.upper - box.lower); }

  inline bool overlaps(const Box &a, const Box &b)
  {
    assert(a.size() == b.size());
    for (int i=0;i<a.size();i++)
      if (a.lower[i] > b.upper[i] ||
          a.upper[i] < b.lower[i])
        return false;
    return true;
  }
  
  inline bool overlaps(const Box &a, const Coords &b)
  {
    assert(a.size() == b.size());
    for (int i=0;i<a.size();i++)
      if (a.lower[i] > b[i] ||
          a.upper[i] < b[i])
        return false;
    return true;
  }
  
  inline std::ostream &operator<<(std::ostream &out, const Box &box)
  { out << "{" << box.lower << "," << box.upper << "}"; return out; }
  
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
    py::list find(const std::vector<double> &coords);

    /*! finds the closest data point to given query point, and returns
        a tuple [ point, (values) ]; the 'values' is a *list* of all
        the values that share that data point (ie, it is always a list
        even if the input data set did not contain any duplicates) */
    std::tuple<std::vector<double>,py::list>
    //    py::tuple
    findClosest(const std::vector<double> &coords, const py::kwargs &kwargs);

    /*! returns a list with all key:value pairs with the given point and radius */
    py::list all_points_in_radius(const std::vector<double> &coords,
                                  double radius);

    /*! returns a list with all key:value pairs with the given box */
    std::vector<std::pair<std::vector<double>,py::object>>
    allPointsInRange(const std::vector<double> &lower,
                     const std::vector<double> &upper);
    
    /*! returns a list with (only) the values value of all poitnts within given box */
    py::list allValuesInRange(const std::vector<double> &lower,
                              const std::vector<double> &upper);

    /*! returns a list with (only) the values value of all poitnts within given radius */
    py::list all_values_in_radius(const std::vector<double> &coords,
                                        double radius);
    
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

