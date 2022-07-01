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

#include "pyQuiri/Coords.h"

namespace pyq {

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

  /*! computes the smalest L2 distnace between a point and any point
      in the given box box */
  inline double distance(const Box &box, const Coords &point)
  {
    assert(a.size() == b.size());
    Coords closestPoint = min(max(box.lower,point),box.upper);
    return distance(closestPoint,point);
  }
  
  inline std::ostream &operator<<(std::ostream &out, const Box &box)
  { out << "{" << box.lower << "," << box.upper << "}"; return out; }
  
} // ::pyq


