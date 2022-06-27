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

void kdtree();

namespace pyq {

  // struct Context {
  //   typedef std::shared_ptr<Context> SP;
  //   virtual ~Context() {}
  //   static Context::SP get();
  // };
  
  // struct CurrentContext {
  //   static Context::SP create()
  //   {
  //     handle = std::make_shared<Context>();
  //     return handle;
  //   }

  //   static Context::SP get()
  //   {
  //     if (!handle)
  //       throw std::runtime_error("#pyq: no context active"
  //                                " - did you forget pyq.context_create()?");
  //     return handle;
  //   }
    
  //   static Context::SP handle;
  // };
  // Context::SP CurrentContext::handle = {};

  // /*! creates a new context, and makes it current */
  // std::shared_ptr<Context> createContext()
  // {
  //   return CurrentContext::create();
  // }

  // KDTree::SP create_kd_tree
  
  // void helloWorld()
  // {kdtree();}
}

using namespace pyq;

PYBIND11_MODULE(pyQuiri, m) {

  // optional module docstring
  m.doc() = "pyQuiri: python module for spatial range/proximity queries (find_closest, kNN, etc)";

  // create one context; almost all functions tare then per context
  // m.def("context_create", &pyq::createContext,
  //       "Creates a 'pyq' Context object");
  // m.def("hello_world", &pyq::helloWorld,
  //       "says 'hello world'");

  m.def("kd_tree", &pyq::KDTree::create,
        "creates a new k-dimenional kd-tree object'");
  // m.def("create_kd_tree", &pyq::create_kd_tree,
  //       "creates a new k-dimenional kd-tree object'");

  // -------------------------------------------------------
  auto kdTree
    = pybind11::class_<pyq::KDTree,
                       std::shared_ptr<pyq::KDTree>>(m, "KDTree");
  kdTree.def("add", &pyq::KDTree::add, "add a new element");
  kdTree.def("build", &pyq::KDTree::find, "(re-)builds a kd-tree to prepare it for performing query operations");
  kdTree.def("find", &pyq::KDTree::find, "finds and returns a previously added element");

  
  // -------------------------------------------------------
  // auto context
  //   = pybind11::class_<pyq::Context,
  //                std::shared_ptr<Context>>(m, "Context");

  // context.def(pybind11::init<>());
}
