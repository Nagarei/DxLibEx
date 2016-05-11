/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_WIN32API_WRAP_EXCEPTION_ERROR_HANDLING_HELPER_HPP_
#define DXLE_INC_WIN32API_WRAP_EXCEPTION_ERROR_HANDLING_HELPER_HPP_
#include "dxlibex/config/no_min_max.h"
#include "dxlibex/type_traits.hpp"
#include <Windows.h>
#include <cstddef>
#include <string>
namespace dxle {
namespace win32api {
	namespace exception {
		namespace detail {
			template<typename ...Ts> DWORD format_message(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, char* lpBuffer, DWORD nSize, Ts... args) {
				return ::FormatMessageA(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, args...);
			}
			template<typename ...Ts> DWORD format_message(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, wchar_t* lpBuffer, DWORD nSize, Ts... args) {
				return ::FormatMessageW(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, args...);
			}
		}
		template<typename CharType, enable_if_t<is_char_type<CharType>::value, nullptr_t> = nullptr>
		std::basic_string<CharType> handle_error_helper() { return handle_error_helper<CharType>(::GetLastError()); }
		template<typename CharType, enable_if_t<is_char_type<CharType>::value, nullptr_t> = nullptr>
		std::basic_string<CharType> handle_error_helper(const DWORD err_code, const std::size_t buffer_len = 2000, const std::size_t loop_cnt = 0) {
			using str_t = std::basic_string<CharType>;
			str_t str;
			str.resize(buffer_len);
			const auto len = detail::format_message(
				//FORMAT_MESSAGE_ALLOCATE_BUFFERを利用しない理由はWindows10ではLocalFree関数がSDKにないのでHeapFree関数を使う必要があり、条件分けが面倒なため。
				//よって2000*sizeof(TCHAR) byteを決め打ちする。
				//https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351%28v=vs.85%29.aspx
				FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
				nullptr, err_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &str[0], static_cast<DWORD>(str.size()), nullptr
			);
			if (!len) return (loop_cnt < 4) ? handle_error_helper(err_code, buffer_len * 2, loop_cnt + 1) : str_t();//エラー時はバッファーが足りなかったことを考え再帰
			str.resize(len);
			return str;
		}
	}
}
//using namespace win32api;//dxle名前空間には読み込まない
}

#endif //DXLE_INC_WIN32API_WRAP_EXCEPTION_ERROR_HANDLING_HELPER_HPP_
