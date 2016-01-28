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

namespace dxle {
	using tstring = std::basic_string<TCHAR>;
}
#endif //DXLE_INC_BASIC_TYPES_TCHAR_HPP_
