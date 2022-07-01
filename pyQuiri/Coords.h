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

  /*! helper class to represent N-dimensionsinal double-precision
    points / coordinates */
  struct Coords {
    Coords(int N, double defaultValue = 0.);
    Coords(const Coords &other) : coords(other.coords) {}
    Coords(const std::vector<double> &other) : coords(other) {}
    
    inline double &operator[](int i) { return coords[i]; }
    inline double  operator[](int i) const { return coords[i]; }
    inline int size() const { return (int)coords.size(); }
    
    std::vector<double> coords;
  };

  /*! sets a to the (compnent-wise) minimum of a and b */
  inline void   set_min(Coords &a, const Coords &b);
  
  /*! sets a to the (compnent-wise) maximum of a and b */
  inline void   set_max(Coords &a, const Coords &b);

  /*! returns a coords that is the component-wise mimimum of a and b */
  inline Coords min(const Coords &a, const Coords &b);

  /*! returns a coords that is the component-wise maximum of a and b */
  inline Coords max(const Coords &a, const Coords &b);

  /*! returns the dimension i at which a[i] >= a[j] for any other j */
  inline int    arg_max(const Coords &a);
  
  /* reutrns the dot product of two coords */
  inline double dot(const Coords &a, const Coords &b);

  /*! computes the L2 distance between two points */
  inline double distance(const Coords &a, const Coords &b);

  /*! returns difference vector between to coordinates */
  inline Coords operator-(const Coords &a, const Coords &b);

  /*! returns if two coordinates are the same */
  inline bool   operator==(const Coords &a, const Coords &b);

  /*! pretty-prints a coordinate class */
  inline std::ostream &operator<<(std::ostream &out, const Coords &coords);


  // ==================================================================
  // IMPLEMENTATION
  // vvvvvvvvvvvvvv
  // ==================================================================

  
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

  inline Coords::Coords(int N, double defaultValue) : coords(N)
  {
    for (auto &v : coords) v = defaultValue;
  }
} // ::pyq
