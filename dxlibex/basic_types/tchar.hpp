/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_TCHAR_HPP_
#define DXLE_INC_BASIC_TYPES_TCHAR_HPP_

#if defined(__c2__) &&  __clang_major__ == 3 && __clang_minor__ == 8
//To avoid compile error
//C:\Program Files (x86)\Windows Kits\8.1\Include\um\combaseapi.h(229,21): error : unknown type name 'IUnknown'
//          static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
struct IUnknown;
#endif
#if defined(__c2__)
//Visual Studio 2015 Update 1 Clang with Microsoft CodeGen(clang 3.7)にはtchar.hまわりにバグあり、include順をいじるな
//https://connect.microsoft.com/VisualStudio/feedback/details/2122400
#include <Windows.h>
#endif//defined(__c2__)
#include <tchar.h>
#include <string>
#include <cstring>
#include "dxlibex/type_traits.hpp"
namespace dxle {
	using tstring = std::basic_string<TCHAR>;
	using std::size_t;
	inline size_t tstrlen(const char* s) { return std::strlen(s); }
	inline size_t tstrlen(const wchar_t* s) { return std::wcslen(s); }
}
#endif //DXLE_INC_BASIC_TYPES_TCHAR_HPP_
