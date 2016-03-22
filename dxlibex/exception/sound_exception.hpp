/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_EXCEPTION_SOUND_EXCEPTION_HPP_
#define DXLE_INC_EXCEPTION_SOUND_EXCEPTION_HPP_
#include "dxlibex/exception/runtime_error.hpp"
#define DXLE_SOUND_ERROR_THROW_WITH_MESSAGE( MESSAGE ) throw dxle::sound_error(__FILE__, __FUNCTION__, __LINE__, MESSAGE)
#define DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF( EXPR, MESSAGE ) if( EXPR ) throw dxle::sound_error(__FILE__, __FUNCTION__, __LINE__, #EXPR, MESSAGE)

namespace dxle {
	namespace exception{
		class sound_error : public dxle::exception::runtime_error {
		public:
			sound_error(const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: dxle::exception::runtime_error("sound_error", sorce_name, func_name, line, expression, msg) {}
			sound_error(const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: dxle::exception::runtime_error("sound_error", sorce_name, func_name, line, msg) {}
		};

	}
	using exception::sound_error;
}

#endif //DXLE_INC_EXCEPTION_SOUND_EXCEPTION_HPP_