﻿/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CONFIG_NO_MIN_MAX_H_
#define DXLE_INC_CONFIG_NO_MIN_MAX_H_

//Windows.hなどからくる min max マクロがdefineされるのを防ぐ

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#endif
