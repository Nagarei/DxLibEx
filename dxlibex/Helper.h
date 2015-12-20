/*=============================================================================
  Copyright (C) 2015 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_HELPER_H_
#define DXLE_INC_HELPER_H_

//実装用ファイルです。
//開発者以外がここの機能を使うのはお勧めできません
#include "dxlibex/config/no_min_max.h"
#include <cassert>
#include "dxlibex/config/defines.h"

namespace dxle{
//!実装用のものが集まる名前空間です。開発者以外がここの機能を使うのはお勧めできません。
namespace impl{

	template<typename T>
	//!実装用
	//!カウンタ系
	//!GetNowCountの戻り値を誤って生のまま使うことを防ぐ
	class Counter_template
	{
	private:
		typedef Counter_template<T> This_T;
	public:
		//ここのpublic関数はUIに直結するので破壊的変更は可能な限り避けるように

		Counter_template()DXLE_NOEXCEPT_OR_NOTHROW : value(-1){}

		//! 整数型の値を取得する
		inline const T& GetInt()const DXLE_NOEXCEPT_OR_NOTHROW{ return count(); }
		//! 整数型の値を取得する
		inline const T& count()const DXLE_NOEXCEPT_OR_NOTHROW{ return value; }
		//! 整数型の値を取得する
		explicit inline operator T()const DXLE_NOEXCEPT_OR_NOTHROW{ return count(); }

		inline const This_T& operator+()const DXLE_NOEXCEPT_OR_NOTHROW{ return *this; }
		inline This_T        operator-()const DXLE_NOEXCEPT_OR_NOTHROW{ return This_T(-value); }

		inline This_T operator+(const T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return This_T(value + other); }
		inline This_T operator-(const T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return This_T(value - other); }
		inline friend This_T operator+(const T& other, const This_T& bace) DXLE_NOEXCEPT_OR_NOTHROW{ assert(bace.value != -1); return This_T(other + bace.value); }
		inline friend This_T operator-(const T& other, const This_T& bace) DXLE_NOEXCEPT_OR_NOTHROW{ assert(bace.value != -1); return This_T(other - bace.value); }
		inline This_T& operator+=(const T& other) DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); value += other; return *this; }
		inline This_T& operator-=(const T& other) DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); value -= other; return *this; }

		inline T operator-(const This_T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return value - other.value; }

		inline bool operator< (const This_T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return value <  other.value; }
		inline bool operator> (const This_T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return value >  other.value; }
		inline bool operator<=(const This_T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return value <= other.value; }
		inline bool operator>=(const This_T& other)const DXLE_NOEXCEPT_OR_NOTHROW{ assert(value != -1); return value >= other.value; }

	protected:
		explicit Counter_template(T param)DXLE_NOEXCEPT_OR_NOTHROW : value(param){}

		template<typename U>
		struct Cast : public Counter_template<U>{
			explicit Cast(const U& param) :Counter_template<U>(param){}
			//!Counter_template<T>からCounter_template<U>にキャストする
			inline static Counter_template<U> Do(const U& value)DXLE_NOEXCEPT_OR_NOTHROW
			{
				return Counter_template<U>(Cast<U>(value));
			}
		};
	private:
		T value;
	};


	template<typename Child>
	//!ハンドル自動削除実装用
	class Unique_Handle_Bace
	{
	protected:
		typedef Unique_Handle_Bace<Child> Bace_T;
	private:
		//コピー禁止
		Unique_Handle_Bace(const Bace_T&) = delete;
		Unique_Handle_Bace& operator=(const Bace_T&) = delete;
	public:
		Unique_Handle_Bace()DXLE_NOEXCEPT_OR_NOTHROW
			:handle(-1)
		{}

		//所有権の譲渡
		Unique_Handle_Bace(Bace_T&& other)DXLE_NOEXCEPT_OR_NOTHROW
			: handle(other.handle)
		{
			other.handle = -1;
			SetDeleteHandleFlag(handle, &handle);
		}
		//所有権の譲渡
		Unique_Handle_Bace& operator=(Bace_T&& other)DXLE_NOEXCEPT_OR_NOTHROW
		{
			if (this == &other) { return *this; }
			handle = other.handle;
			other.handle = -1;
			SetDeleteHandleFlag(handle, &handle);
			return *this;
		}

		~Unique_Handle_Bace() DXLE_NOEXCEPT_OR_NOTHROW{
			//リソース解放
			static_cast<Child*>(this)->Delete();
		}
	protected:
		//間違えて他の種類のハンドルを持たないようにprotectedにしておく
		Unique_Handle_Bace(int param_handle)DXLE_NOEXCEPT_OR_NOTHROW
			: handle(param_handle)
		{
			SetDeleteHandleFlag(handle, &handle);
		}

		//!このオブジェクトを無効化し、今まで使っていたハンドルを返す
		//!自動削除も無効化される
		inline int ReleaseRun()DXLE_NOEXCEPT_OR_NOTHROW{
			int temp = GetHandle();
			SetDeleteHandleFlag(handle, nullptr);
			handle = -1;
			return temp;
		}
		int GetHandle()const DXLE_NOEXCEPT_OR_NOTHROW{ return handle; }
	private:
		int handle;
	};
	template<typename Child>
	//!ハンドルの指すオブジェクト実装用
	class Unique_HandledObject_Bace
	{
	protected:
		typedef Unique_HandledObject_Bace<Child> Bace_T;
	private:
		//コピー禁止
		Unique_HandledObject_Bace(const Bace_T&) = delete;
		Unique_HandledObject_Bace& operator=(const Bace_T&) = delete;
	public:
		Unique_HandledObject_Bace()DXLE_NOEXCEPT_OR_NOTHROW
			: handle(-1)
		{}

		//所有権の譲渡
		Unique_HandledObject_Bace(Bace_T&& other)DXLE_NOEXCEPT_OR_NOTHROW
			: handle(other.handle)
		{
			other.handle = -1;
		}
		//所有権の譲渡
		Unique_HandledObject_Bace& operator=(Bace_T&& other)DXLE_NOEXCEPT_OR_NOTHROW
		{
			if (this == &other) { return *this; }
			handle = other.handle;
			DxLib::SetDeleteHandleFlag(handle, &handle);
			other.handle = -1;
			return *this;
		}

	protected:
		//間違えて他の種類のハンドルを持たないようにprotectedにしておく
		Unique_HandledObject_Bace(int param_handle)DXLE_NOEXCEPT_OR_NOTHROW
			: handle(param_handle)
		{
			DxLib::SetDeleteHandleFlag(handle, &handle);
		}
		virtual ~Unique_HandledObject_Bace() DXLE_NOEXCEPT_OR_NOTHROW {
			//リソース解放
			static_cast<Child*>(this)->Delete();
		}
		int GetHandle()const DXLE_NOEXCEPT_OR_NOTHROW { return handle; }
		void SetHandle_IMPL(int new_handle) { handle = (param_handle); DxLib::SetDeleteHandleFlag(handle, &handle); }
	private:
		int handle;
	};

}//namespace impl
}//namespace dxle

#endif
