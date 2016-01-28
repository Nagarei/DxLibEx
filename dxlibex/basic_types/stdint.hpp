/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_STDINT_HPP_
#define DXLE_INC_BASIC_TYPES_STDINT_HPP_

#include <cstdint>

// N4296 17.6.1.2 Headers 4    p.449
// In the C ++ standard library, however, the declarations (except for names which are defined as macros in C)
// are within namespace scope (3.3.6) of the namespace std.
// It is unspecified whether these names are first declared
// within the global namespace scope and are then injected into namespace std
// by explicit using-declarations (7.3.3).

namespace dxle {
	using std::int8_t; using std::int16_t;
	using std::int32_t; using std::int64_t;
	using std::uint8_t; using std::uint16_t;
	using std::uint32_t; using std::uint64_t;

	using std::int_least8_t; using std::int_least16_t;
	using std::int_least32_t;  using std::int_least64_t;
	using std::uint_least8_t; using std::uint_least16_t;
	using std::uint_least32_t; using std::uint_least64_t;

	using std::int_fast8_t; using std::int_fast16_t;
	using std::int_fast32_t;  using std::int_fast64_t;
	using std::uint_fast8_t; using std::uint_fast16_t;
	using std::uint_fast32_t; using std::uint_fast64_t;

	using std::intmax_t; using std::intptr_t;
	using std::uintmax_t; using std::uintptr_t;
}
#endif //DXLE_INC_BASIC_TYPES_STDINT_HPP_
