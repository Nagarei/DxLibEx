/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_TCHAR_HPP_
#define DXLE_INC_BASIC_TYPES_TCHAR_HPP_

#include <Windows.h>
#include <tchar.h>
//Visual Studio 2015 Update 1 Clang with Microsoft CodeGen(clang 3.7)にはtchar.hまわりにバグあり、include順をいじるな
//https://connect.microsoft.com/VisualStudio/feedback/details/2122400
#include <string>
#include <cstring>
#include <filesystem>//in gcc(branch 5 or later), -lstdc++fs is required.
#include "dxlibex/type_traits.hpp"
namespace dxle {
	using tstring = std::basic_string<TCHAR>;
#if defined(_MSC_VER) && _MSC_VER == 1800
	namespace fs = std::tr2::sys;
#else
	namespace fs = std::experimental::filesystem;//まだ標準には入っていない。boost::filesystemでも良かったけどmingw-clang以外では利用できそうだったので。
#endif
	using tpath = fs::basic_path<tstring, std::conditional_t<std::is_same<TCHAR, char>::value, fs::path_traits, fs::wpath_traits>>;
	using std::size_t;
	inline size_t tstrlen(const char* s) { return std::strlen(s); }
	inline size_t tstrlen(const wchar_t* s) { return std::wcslen(s); }
}
#endif //DXLE_INC_BASIC_TYPES_TCHAR_HPP_
