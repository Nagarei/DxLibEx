/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_EXCEPTION_OUT_OF_RANGE_HPP_
#define DXLE_INC_EXCEPTION_OUT_OF_RANGE_HPP_
#include <stdexcept>
#include <string>
#include <cstdint>
#define DXLE_OUT_OF_RANGE_THROW_WITH_MESSAGE( MESSAGE ) throw dxle::out_of_range(__FILE__, __FUNCTION__, __LINE__, MESSAGE)
#define DXLE_OUT_OF_RANGE_THROW_WITH_MESSAGE_IF( EXPR, MESSAGE ) if( EXPR ) throw dxle::out_of_range(__FILE__, __FUNCTION__, __LINE__, #EXPR, MESSAGE)
namespace dxle {
	namespace exception{
		class out_of_range : public std::out_of_range {
		protected:
			out_of_range(const char* except_name, const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: std::out_of_range(
					std::string("exception : ") + except_name + '\n'
					+ "  in " + sorce_name + "\n"
					+ "  " + func_name + "() (line." + std::to_string(line) + ")\n"
					+ "  follow by below\n"
					+ "    " + expression
					+ ((msg.empty() || msg[0] == '\0') ? "\n" : "\n  MESSAGE : " + msg + "\n")
				)
			{}
			out_of_range(const char* except_name, const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: std::out_of_range(
					std::string("exception : ") + except_name + '\n'
					+ " in " + sorce_name + "\n"
					+ "  " + func_name + "() (line." + std::to_string(line) + ")\n"
					+ ((msg.empty() || msg[0] == '\0') ? "\n" : " MESSAGE : " + msg + "\n")
				)
			{}
		public:
			out_of_range(const char* sorce_name, const char* func_name, std::uint64_t line, const char* expression, const std::string& msg)
				: out_of_range("out_of_range", sorce_name, func_name, line, expression, msg) {}
			out_of_range(const char* sorce_name, const char* func_name, std::uint64_t line, const std::string& msg)
				: out_of_range("out_of_range", sorce_name, func_name, line, msg) {}
		};

	}
	using exception::out_of_range;
}
#endif //DXLE_INC_EXCEPTION_OUT_OF_RANGE_HPP_
