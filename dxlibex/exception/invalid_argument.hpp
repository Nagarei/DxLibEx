/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_EXCEPTION_INVAID_ARGUMENT_HPP_
#define DXLE_INC_EXCEPTION_INVAID_ARGUMENT_HPP_
#include <stdexcept>
#include <string>
#include <cstdint>
#define DXLE_INVAID_ARGUMENT_THROW_WITH_MESSAGE( MESSAGE ) throw dxle::invalid_argument(__FILE__, __FUNCTION__, __LINE__, MESSAGE)
#define DXLE_INVAID_ARGUMENT_THROW_WITH_MESSAGE_IF( EXPR, MESSAGE ) if( EXPR ) throw dxle::invalid_argument(__FILE__, __FUNCTION__, __LINE__, #EXPR, MESSAGE)
namespace dxle {
	namespace exception{
		class invalid_argument : public std::invalid_argument {
		protected:
			invalid_argument(const char* except_name, const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: std::invalid_argument(
					std::string("exception : ") + except_name + '\n'
					+ "  in " + sorce_name + "\n"
					+ "  " + func_name + "() (line." + std::to_string(line) + ")\n"
					+ "  follow by below\n"
					+ "    " + expression
					+ ((msg.empty() || msg[0] == '\0') ? "\n" : "\n  MESSAGE : " + msg + "\n")
				)
			{}
			invalid_argument(const char* except_name, const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: std::invalid_argument(
					std::string("exception : ") + except_name + '\n'
					+ " in " + sorce_name + "\n"
					+ "  " + func_name + "() (line." + std::to_string(line) + ")\n"
					+ ((msg.empty() || msg[0] == '\0') ? "\n" : " MESSAGE : " + msg + "\n")
				)
			{}
		public:
			invalid_argument(const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: invalid_argument("invalid_argument", sorce_name, func_name, line, expression, msg) {}
			invalid_argument(const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: invalid_argument("invalid_argument", sorce_name, func_name, line, msg) {}
		};

	}
	using exception::invalid_argument;
}
#endif //DXLE_INC_EXCEPTION_INVAID_ARGUMENT_HPP_