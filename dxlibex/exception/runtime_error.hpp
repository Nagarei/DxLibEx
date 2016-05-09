/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_EXCEPTION_RUNTIME_ERROR_HPP_
#define DXLE_INC_EXCEPTION_RUNTIME_ERROR_HPP_
#include <stdexcept>
#include <string>
#include <cstdint>
#define DXLE_RUNTIME_ERROR_THROW_WITH_MESSAGE( MESSAGE ) throw dxle::runtime_error(__FILE__, __FUNCTION__, __LINE__, MESSAGE)
#define DXLE_RUNTIME_ERROR_THROW_WITH_MESSAGE_IF( EXPR, MESSAGE ) if( EXPR ) throw dxle::runtime_error(__FILE__, __FUNCTION__, __LINE__, #EXPR, MESSAGE)
namespace dxle {
	namespace exception{
		class runtime_error : public std::runtime_error {
		protected:
			runtime_error(const char* except_name, const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: std::runtime_error(
					std::string("exception : ") + except_name + '\n'
					+ "  in " + sorce_name + "\n"
					+ "  " + func_name + "() (line." + std::to_string(line) + ")\n"
					+ "  follow by below\n"
					+ "    " + expression
					+ ((msg.empty() || msg[0] == '\0') ? "\n" : "\n  MESSAGE : " + msg + "\n")
				)
			{}
			runtime_error(const char* except_name, const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: std::runtime_error(
					std::string("exception : ") + except_name + '\n'
					+ " in " + sorce_name + "\n"
					+ "  " + func_name + "() (line." + std::to_string(line) + ")\n"
					+ ((msg.empty() || msg[0] == '\0') ? "\n" : " MESSAGE : " + msg + "\n")
				)
			{}
		public:
			runtime_error(const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: runtime_error("runtime_error", sorce_name, func_name, line, expression, msg) {}
			runtime_error(const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: runtime_error("runtime_error", sorce_name, func_name, line, msg) {}
		};

	}
	using exception::runtime_error;
}
#endif //DXLE_INC_EXCEPTION_RUNTIME_ERROR_HPP_