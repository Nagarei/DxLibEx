/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_UTILITY_INFERIOR_STRING_REF_HPP_
#define DXLE_INC_UTILITY_INFERIOR_STRING_REF_HPP_
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits.hpp"
#include "dxlibex/basic_types/tchar.hpp"
#include <type_traits>
#include <string>
namespace dxle {
//!inline
namespace utility {
	template<typename CharType, bool is_char_type = is_char_type<CharType>::value> class basic_inferior_string_ref;
	template<typename CharType> class basic_inferior_string_ref<CharType, true> {
	public:
		using char_type = CharType;
	private:
		const char_type* str_;
	public:
		DXLE_CONSTEXPR basic_inferior_string_ref() : str_() {}
		basic_inferior_string_ref(const char_type* str) : str_(str) {}
		//DXLE_CONSTEXPR basic_inferior_string_ref(const char_type* str, std::size_t len) : str_(str) {}//do not provide to ensure that str_ is null-terminated.
		template<typename Allocator = std::allocator<char_type>>basic_inferior_string_ref(const std::basic_string<char_type, std::char_traits<char_type>, Allocator>& str) : str_(str.c_str()) {}
		DXLE_CONSTEXPR basic_inferior_string_ref(const basic_inferior_string_ref& o) : str_(o.str_) {}
		DXLE_CONSTEXPR basic_inferior_string_ref(basic_inferior_string_ref&& o) : str_(o.str_) {}
		DXLE_CXX14_CONSTEXPR basic_inferior_string_ref& operator=(const basic_inferior_string_ref& o) noexcept { this->str_ = o.str_; return *this; }
		DXLE_CXX14_CONSTEXPR basic_inferior_string_ref& operator=(basic_inferior_string_ref&& o) noexcept { this->str_ = o.str_; return *this; }
		const char_type* c_str() const noexcept { return this->str_; }
	};
	using inferior_string_ref = basic_inferior_string_ref<char>;
	using winferior_string_ref = basic_inferior_string_ref<wchar_t>;
	using tinferior_string_ref = basic_inferior_string_ref<TCHAR>;
}//namespace
using namespace utility;
}
#endif //DXLE_INC_UTILITY_INFERIOR_STRING_REF_HPP_
