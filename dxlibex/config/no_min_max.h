#ifndef DXLE_INC_CONFIG_NO_MIN_MAX_HPP_
#define DXLE_INC_CONFIG_NO_MIN_MAX_HPP_

//min max マクロ無効化

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
