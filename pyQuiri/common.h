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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <memory>

#ifndef _USE_MATH_DEFINES
#  define _USE_MATH_DEFINES
#endif
#include <math.h> // using cmath causes issues under Windows

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <assert.h>
#include <string>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <sstream>
#ifdef __GNUC__
#include <execinfo.h>
#include <sys/time.h>
#endif

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#if !defined(WIN32)
#include <signal.h>
#endif

#if defined(_MSC_VER)
//&& !defined(__PRETTY_FUNCTION__)
#  define __PRETTY_FUNCTION__ __FUNCTION__
#endif


#ifndef PRINT
# define PRINT(var) std::cout << #var << "=" << var << std::endl;
#ifdef __WIN32__
# define PING std::cout << __FILE__ << "::" << __LINE__ << ": " << __FUNCTION__ << std::endl;
#else
# define PING std::cout << __FILE__ << "::" << __LINE__ << ": " << __PRETTY_FUNCTION__ << std::endl;
#endif
#endif


#define PYQ_NOTIMPLEMENTED throw std::runtime_error(std::string(__PRETTY_FUNCTION__)+" not implemented")

#ifdef WIN32
# define PYQ_TERMINAL_RED ""
# define PYQ_TERMINAL_GREEN ""
# define PYQ_TERMINAL_LIGHT_GREEN ""
# define PYQ_TERMINAL_YELLOW ""
# define PYQ_TERMINAL_BLUE ""
# define PYQ_TERMINAL_LIGHT_BLUE ""
# define PYQ_TERMINAL_RESET ""
# define PYQ_TERMINAL_DEFAULT PYQ_TERMINAL_RESET
# define PYQ_TERMINAL_BOLD ""

# define PYQ_TERMINAL_MAGENTA ""
# define PYQ_TERMINAL_LIGHT_MAGENTA ""
# define PYQ_TERMINAL_CYAN ""
# define PYQ_TERMINAL_LIGHT_RED ""
#else
# define PYQ_TERMINAL_RED "\033[0;31m"
# define PYQ_TERMINAL_GREEN "\033[0;32m"
# define PYQ_TERMINAL_LIGHT_GREEN "\033[1;32m"
# define PYQ_TERMINAL_YELLOW "\033[1;33m"
# define PYQ_TERMINAL_BLUE "\033[0;34m"
# define PYQ_TERMINAL_LIGHT_BLUE "\033[1;34m"
# define PYQ_TERMINAL_RESET "\033[0m"
# define PYQ_TERMINAL_DEFAULT PYQ_TERMINAL_RESET
# define PYQ_TERMINAL_BOLD "\033[1;1m"

# define PYQ_TERMINAL_MAGENTA "\e[35m"
# define PYQ_TERMINAL_LIGHT_MAGENTA "\e[95m"
# define PYQ_TERMINAL_CYAN "\e[36m"
# define PYQ_TERMINAL_LIGHT_RED "\033[1;31m"
#endif

#ifdef _MSC_VER
# define PYQ_ALIGN(alignment) __declspec(align(alignment)) 
#else
# define PYQ_ALIGN(alignment) __attribute__((aligned(alignment)))
#endif

namespace py = pybind11;

namespace pyq {
}

