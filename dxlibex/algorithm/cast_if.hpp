/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_ALGORITHM_CAST_IF_HPP_
#define DXLE_INC_ALGORITHM_CAST_IF_HPP_
#include "dxlibex/config/defines.h"
namespace dxle {
	namespace detail {

		template<typename from, typename to, bool do_cast,
			bool is_nothrow_move_constructible =
				std::is_nothrow_move_constructible<from>::value>
		struct static_cast_if_helper{
			DXLE_CONSTEXPR to operator()(const from& n)
				const DXLE_NOEXCEPT_IF_EXPR(static_cast<double>(std::declval<from>()))
			{
				return static_cast<double>(n);
			}
		};

		template<typename from, typename to>
		struct static_cast_if_helper<to, from, false, false>{
			DXLE_CONSTEXPR to operator()(const from& n)
				const DXLE_NOEXCEPT_IF(std::is_nothrow_copy_constructible<from>::value)
			{
				return n;
			}
		};

		template<typename from, typename to>
		struct static_cast_if_helper<to, from, false, true>{

			DXLE_CONSTEXPR to operator()(const from& n)
				const DXLE_NOEXCEPT_IF(std::is_nothrow_copy_constructible<from>::value)
			{
				return n;
			}
			DXLE_CONSTEXPR to operator()(from&& n)
				const DXLE_NOEXCEPT_OR_NOTHROW
			{
				return std::move(n);
			}
		};
	}//namespace detail

	template<typename from, typename to, bool do_cast>
	DXLE_CONSTEXPR to static_cast_if(const from& n)
		DXLE_NOEXCEPT_IF_EXPR((
			dxle::detail::static_cast_if_helper<to, from, do_cast>()(
				std::move(std::declval<from>())
			)
		))
	{
		return dxle::detail::static_cast_if_helper<from, to, do_cast>()(std::move(n));
	}

	template<typename from, typename to, bool do_cast>
	DXLE_CONSTEXPR to static_cast_if(from&& n)
		DXLE_NOEXCEPT_IF_EXPR((
			dxle::detail::static_cast_if_helper<to, from, do_cast>()(
				std::move(std::declval<from>())
			)
		))
	{
		return dxle::detail::static_cast_if_helper<from, to, do_cast>()(std::move(n));
	}
}//namespace dxle
#endif //DXLE_INC_ALGORITHM_CAST_IF_HPP_
