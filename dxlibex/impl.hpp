/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_IMPL_H_
#define DXLE_INC_IMPL_H_

#if 0 //(old version)
//実装用ファイルです。
//開発者以外がここの機能を使うのはお勧めできません

#include "dxlibex/config/no_min_max.h"
#include "dxlibex/dxlibex.hpp"
#include <type_traits>
#include <new>
//各関数の定義

namespace dxle
{
	//----------DxLibSys----------//
	namespace nDxLibSys
	{

		//!DxLib_Initを行う。<br>
		//!失敗するとint型の例外を投げる。<br>
		//!初回のみ有効。<br>
		//!WinMainからのreturnでDxLibEndを呼び出します。
		inline void DxLibSys::Init()
		{
			static DxLibSys initer;
		}
		//!DxLib_Endを行う
		inline void DxLibSys::End()DXLE_NOEXCEPT_OR_NOTHROW
		{
			DxLib::DxLib_End();
		}
		//!DXライブラリの内部で使用している構造体をゼロ初期化して、
		//!DxLib_Init の前に行った設定を無効化する( DxLib_Init の前でのみ有効 )
		inline int DxLibSys::GlobalStructInitialize()DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::DxLib_GlobalStructInitialize();
		}
		//!DXライブラリが初期化されているかどうかを取得する
		//!@return true:初期化されている  false:されていない
		inline bool DxLibSys::IsInit()DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::DxLib_IsInit() != FALSE;
		}
		//!ウインドウズのメッセージループに代わる処理を行う
		inline int DxLibSys::ProcessMessage()
		{
			return DxLib::ProcessMessage();
		}
		DxLibSys::DxLibSys() {
			if (DxLib::DxLib_Init() == -1) {
				throw - 1;
			}
		}
		DxLibSys::~DxLibSys()DXLE_NOEXCEPT_OR_NOTHROW {
			DxLibSys::End();
		}
	}

	//----------Wait ウエイト系関数----------//


	//----------カウンタ系----------//
	namespace nCounter
	{

		//!ミリ秒単位の精度を持つカウンタの現在値を得る
		inline Counter Counter::GetNowCount(bool UseRDTSCFlag)DXLE_NOEXCEPT_OR_NOTHROW {
			return Counter(DxLib::GetNowCount(UseRDTSCFlag));
		}
		//! 1000をかけてHiPerformanceCounterとみなすようにする
		inline HiPerformanceCounter Counter::CastToHiPerformance()const DXLE_NOEXCEPT_OR_NOTHROW {
			return Cast<LONGLONG>::Do(static_cast<LONGLONG>(count()) * 1000);
		}
		//!マイクロ秒単位の精度を持つカウンタの現在値を得る
		inline HiPerformanceCounter HiPerformanceCounter::GetNowCount(bool UseRDTSCFlag)DXLE_NOEXCEPT_OR_NOTHROW {
			return HiPerformanceCounter(DxLib::GetNowHiPerformanceCount(UseRDTSCFlag));
		}
		//! 1000で割ってCounterとみなすようにする
		inline Counter HiPerformanceCounter::CastToLowPerformance()const DXLE_NOEXCEPT_OR_NOTHROW {
			return Cast<int>::Do(static_cast<int>(count() / 1000));
		}
	}

	//----------乱数系----------//


	//----------ログ系----------//


	//----------非同期読み込み関係----------//


	//----------ハンドル関係----------//


	//----------マウス関係----------//
	namespace nMouse
	{
		inline MouseState::MouseState() DXLE_NOEXCEPT_OR_NOTHROW
			: LeftButton(ButtonState::Released)
			, MiddleButton(ButtonState::Released)
			, RightButton(ButtonState::Released)
			, ScrollWheelValue(0)
			, X(0)
			, XButton1(ButtonState::Released)
			, XButton2(ButtonState::Released)
			, Y(0)
		{}
		//!ゲームでは通常、GetState を使用して実際のマウスの状態を取得します。
		//!このコンストラクターは、代わりにゲーム自体の入力サブシステム内で渡すための
		//!マウス入力をシミュレートするために使用されます。
		//!@param[in] x              マウスの水平位置。
		//!@param[in] y              マウスの垂直位置。
		//!@param[in] scrollWheel    マウス スクロール ホイールの値。
		//!@param[in] leftButton     左マウス ボタンの状態。
		//!@param[in] middleButton   中マウス ボタンの状態。
		//!@param[in] rightButton    右マウス ボタンの状態。
		//!@param[in] xButton1       XBUTTON1 の状態。
		//!@param[in] xButton2       XBUTTON2 の状態。
		inline MouseState::MouseState(int x, int y, int scrollWheel
			, ButtonState leftButton, ButtonState middleButton, ButtonState rightButton
			, ButtonState xButton1, ButtonState xButton2) DXLE_NOEXCEPT_OR_NOTHROW
			: LeftButton(leftButton)
			, MiddleButton(middleButton)
			, RightButton(rightButton)
			, ScrollWheelValue(scrollWheel)
			, X(x)
			, XButton1(xButton1)
			, XButton2(xButton2)
			, Y(y)
		{}
		inline bool MouseState::operator==(const MouseState& right)DXLE_NOEXCEPT_OR_NOTHROW
		{
			return (
				(LeftButton == right.LeftButton) &&
				(MiddleButton == right.MiddleButton) &&
				(RightButton == right.RightButton) &&
				(ScrollWheelValue == right.ScrollWheelValue) &&
				(X == right.X) &&
				(XButton1 == right.XButton1) &&
				(XButton2 == right.XButton2) &&
				(Y == right.Y)
				);
		}

		inline MouseInputInfo::MouseInputInfo()DXLE_NOEXCEPT_OR_NOTHROW
			: X(0)
			, Y(0)
			, Type(0)
		{}
		inline MouseInputInfo::MouseInputInfo(int x, int y, int type)DXLE_NOEXCEPT_OR_NOTHROW
			: X(x)
			, Y(y)
			, Type(type)
		{}

		//!マウスの現在の状態 (マウスの位置や押されているボタンなど) を取得します。
		//!@param ScrollWheelValueReset GetMouseWheelRotVolの引数CounterResetと同じ
		MouseState Mouse::GetState(bool ScrollWheelValueReset)DXLE_NOEXCEPT_OR_NOTHROW
		{
			int x, y;
			DxLib::GetMousePoint(&x, &y);

			int scrollWheel = DxLib::GetMouseWheelRotVol(ScrollWheelValueReset);

			int pushed_button = DxLib::GetMouseInput();
			ButtonState
				leftButton = ((pushed_button & MOUSE_INPUT_LEFT) ? ButtonState::Pressed : ButtonState::Released)
				, middleButton = ((pushed_button & MOUSE_INPUT_MIDDLE) ? ButtonState::Pressed : ButtonState::Released)
				, rightButton = ((pushed_button & MOUSE_INPUT_RIGHT) ? ButtonState::Pressed : ButtonState::Released)
				, xButton1 = ((pushed_button & MOUSE_INPUT_4) ? ButtonState::Pressed : ButtonState::Released)
				, xButton2 = ((pushed_button & MOUSE_INPUT_5) ? ButtonState::Pressed : ButtonState::Released)
				;
			return MouseState(x, y, scrollWheel, leftButton, middleButton, rightButton, xButton1, xButton2);
		}
		//!マウスのクリック履歴をまとめて取得する
		//!参照：DxLib::GetMouseInputLog http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N36
		//!@attention 内部でGetMouseInputLogのLogDeleteにTRUEを渡して呼び出します
		std::queue<MouseInputInfo> Mouse::GetMouseInputLog()
		{
			std::queue<MouseInputInfo> result;
			int x, y, pushed_button;
			while (DxLib::GetMouseInputLog(&pushed_button, &x, &y, TRUE) == 0)
			{
				result.push(MouseInputInfo(x, y, pushed_button));
			}
			return result;
		}
	}

	//----------タッチパネル入力関係関数----------//


	//----------メモリ確保系関数----------//
	namespace DxMemory
	{
#if 0
		template<typename... Args>
		//!確保に失敗したらnew_handlerを呼び出す or std::bad_allocを投げるDxRealloc
		//!@param[in] Memory DxReallocの引数Memoryと同じ
		//!@param[in] size DxReallocの引数AllocSizeと同じ
		//!@param[in] File DxReallocの引数Fileと同じ
		//!@param[in] Line DxReallocの引数Lineと同じ
		//!@return DxReallocの戻り値と同じ
		void* DxNewWithLog(void* Memory, size_t size, const char *File/*__FILE__*/, int Line/*__LINE__*/)
		{
			//参照　http://cpplover.blogspot.jp/2010/07/operator-new.html

			// Executes a loop: Within the loop,
			for (;;)
			{
				// the function first attempts to allocate the requested storage.
				void* ptr = DxLib::DxRealloc(Memory, size, File, Line);

				// if the attempt is successful
				if (ptr != nullptr)
				{
					return ptr;
				}

				// Otherwise, 
				std::new_handler handler = std::set_new_handler(nullptr);
				std::set_new_handler(handler);

				// if the argument in the most recent call to set_new_handler() was a null pointer,
				if (handler == nullptr)
				{// throws bad_alloc.
					throw std::bad_alloc();
				}

				// Otherwise, the function calls the current new_handler function.
				handler();
				// If the called function returns, the loop repeats.
			}
		}
		template<typename T>
		template<typename... Args>
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		inline Dx_Unique_Ptr<T> Dx_Unique_Ptr<T>::NewWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, Args&&... args)
		{
			void* ptr = DxNewWithLog(nullptr, sizeof(T), File, Line);
			try {
				return Dx_Unique_Ptr<T>(new(ptr) T(std::forward<Args>(args)...));
			}
			catch (...) {
				//コンストラクタが例外を投げた
				DxLib::DxFree(ptr);
				throw;
			}
		}
		template<typename T>
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		//!@param[in] n    配列の要素数
		inline Dx_Unique_Ptr<T[]> Dx_Unique_Ptr<T[]>::NewWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n)
		{
			void* ptr = DxNewWithLog(nullptr, sizeof(T) * n, File, Line);
			try {
				return Dx_Unique_Ptr<T[]>(new(ptr) T[n]);
			}
			catch (...) {
				//コンストラクタが例外を投げた
				DxLib::DxFree(ptr);
				throw;
			}
		}
#if 0
		template<typename T>
		//!メモリの再確保を行う（配列版のみ）
		//!@attention 途中で例外が投げられた場合baceは必ず無効になります
		//!@param[in,out] bace 基になるDx_Unique_Ptr
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		//!@param[in] n    新しい配列の要素数
		void Dx_Unique_Ptr<T[]>::NewWithLogRun(This_T&& bace, const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n)
		{
			void* ptr;
			try {
				ptr = DxNewWithLog(bace.ptr.get(), sizeof(T) * n, File, Line);
			}
			catch (...) {
				bace.reset();
				throw;
			}

			bace.ptr.release();
			try {
				bace.ptr.reset(new(ptr) T[n]);
			}
			catch (...) {
				//コンストラクタが例外を投げた
				DxLib::DxFree(ptr);
				throw;
			}
		}
#endif
		template<typename T, typename... Args, typename>
		//!Dx_Unique_Ptrオブジェクトを構築するヘルパ関数（単一オブジェクト版）
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		inline Dx_Unique_Ptr<T> MakeUniqueDxWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, Args&&... args)
		{
			return Dx_Unique_Ptr<T>::NewWithLog(File, Line, std::forward<Args>(args)...);
		}
		template<typename T, typename>
		//!Dx_Unique_Ptrオブジェクトを構築するヘルパ関数（配列版）
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		inline Dx_Unique_Ptr<T> MakeUniqueDxWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n)
		{
			return Dx_Unique_Ptr<T>::NewWithLog(File, Line, n);
		}
#endif
	}



}//namespace dxle
#endif //#if 0 //(old version)


//インクルードガード
#endif
