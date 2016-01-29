/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_DXLIB_H_
#define DXLE_INC_DXLIB_H_

#include "dxlibex/config/no_min_max.h"
#include "dxlibex/basic_types.hpp"
#include "dxlibex/Color.hpp"
#include "dxlibex/cstdlib.hpp"
//#include "dxlibex/DxLibWrap.h"
#include "dxlibex/Graph2D.h"
#include "dxlibex/math.hpp"
#include "dxlibex/thread.h"
#include "dxlibex/type_traits.hpp"

#if 0

#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <queue>
#include "dxlibex/basic_types.hpp"
#include "dxlibex/Helper.h"
#include "dxlibex/config/defines.h"
namespace dxle
{
	//!DxLibExの公開クラスが全て入ったnamespaceです
	namespace DxLibEx_Classes{}

	//!DxLib_Endのし忘れを防ぎます
	namespace nDxLibSys
	{
		//!初期化終了系関数、メッセージ処理関数
		//!DxLib_Endのし忘れを防ぎます
		//!（注：exitやabortには対応できません）
		class DxLibSys
		{
		public:
			//----------初期化終了系関数----------//

			//!DxLib_Initを行う。<br>
			//!失敗するとint型の例外を投げる。<br>
			//!初回のみ有効。<br>
			//!WinMainからのreturnでDxLibEndを呼び出します。
			static void Init();
			//!DxLib_Endを行う
			static void End()DXLE_NOEXCEPT_OR_NOTHROW;

			//!DXライブラリの内部で使用している構造体をゼロ初期化して、
			//!DxLib_Init の前に行った設定を無効化する( DxLib_Init の前でのみ有効 )
			static int GlobalStructInitialize()DXLE_NOEXCEPT_OR_NOTHROW;
			//!DXライブラリが初期化されているかどうかを取得する
			//!@return true:初期化されている  false:されていない
			static bool IsInit()DXLE_NOEXCEPT_OR_NOTHROW;


			//----------メッセージ処理関数----------//

			//!ウインドウズのメッセージループに代わる処理を行う
			static int ProcessMessage();

		private:
			DxLibSys();
			~DxLibSys()DXLE_NOEXCEPT_OR_NOTHROW;
		};
	}
	namespace DxLibEx_Classes { using nDxLibSys::DxLibSys; }


	//----------ウエイト系----------//

	//!ウエイト系
	namespace Wait
	{
		//! 指定の時間だけ処理をとめる
		int WaitTimer(int WaitTime)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::WaitTimer(WaitTime); }
#ifndef DX_NON_INPUT
		//! キーの入力待ちを行う
		int WaitKey(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::WaitKey(); }
#endif // DX_NON_INPUT
	}


	//----------カウンタ系----------//

	//!GetNowCount等の誤用を防ぎます
	namespace nCounter
	{
		//!カウンタ系
		//!GetNowCountの戻り値を誤って生のまま使うことを防ぐ
		//!高精度バージョン
		class HiPerformanceCounter;
		//!カウンタ系
		//!GetNowCountの戻り値を誤って生のまま使うことを防ぐ
		class Counter : public impl::Counter_template<int>
		{
		public:
			//! ミリ秒単位の精度を持つカウンタの現在値を得る
			static Counter GetNowCount(bool UseRDTSCFlag = false)DXLE_NOEXCEPT_OR_NOTHROW;
			//! ミリ秒単位の精度を持つカウンタの現在値を得る
			static inline Counter now(bool UseRDTSCFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return GetNowCount(UseRDTSCFlag); }

			//! 1000をかけてHiPerformanceCounterとみなすようにする
			HiPerformanceCounter CastToHiPerformance()const DXLE_NOEXCEPT_OR_NOTHROW;

			Counter()DXLE_NOEXCEPT_OR_NOTHROW {}
			Counter(const Counter_template<int>& param)DXLE_NOEXCEPT_OR_NOTHROW :Counter_template(param) {}
		protected:
			explicit Counter(int param)DXLE_NOEXCEPT_OR_NOTHROW : Counter_template(param) {}
		};
		//!カウンタ系
		//!GetNowCountの戻り値を誤って生のまま使うことを防ぐ
		//!高精度バージョン
		class HiPerformanceCounter : public impl::Counter_template<LONGLONG>
		{
		public:
			//! マイクロ秒単位の精度を持つカウンタの現在値を得る
			static HiPerformanceCounter GetNowCount(bool UseRDTSCFlag = false)DXLE_NOEXCEPT_OR_NOTHROW;
			//! マイクロ秒単位の精度を持つカウンタの現在値を得る
			static inline HiPerformanceCounter now(bool UseRDTSCFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return GetNowCount(UseRDTSCFlag); }

			//! 1000で割ってCounterとみなすようにする
			Counter CastToLowPerformance()const DXLE_NOEXCEPT_OR_NOTHROW;

			HiPerformanceCounter()DXLE_NOEXCEPT_OR_NOTHROW {}
			HiPerformanceCounter(const Counter_template<LONGLONG>& param)DXLE_NOEXCEPT_OR_NOTHROW :Counter_template(param) {}
		protected:
			explicit HiPerformanceCounter(LONGLONG param)DXLE_NOEXCEPT_OR_NOTHROW : Counter_template(param) {}
		};
	}
	namespace DxLibEx_Classes { 
		using nCounter::Counter;
		using nCounter::HiPerformanceCounter;
	}


	//----------乱数系----------//

	//!乱数系
	namespace DxRand
	{
		//! 乱数を取得する
		//!@param[in] RandMax 返って来る値の最大値
		inline int GetRand(int RandMax)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetRand(RandMax); }
		//! 乱数の初期値を設定する
		inline int SRand(int Seed)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SRand(Seed); }
	};


	//----------ログ系----------//

	//!ログ系
	namespace DxLog
	{
#ifndef DX_NON_LOG
		// エラー処理関数

		//! ログファイル( Log.txt ) に文字列を出力する
		inline int ErrorLogAdd(const TCHAR *ErrorStr)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ErrorLogAdd(ErrorStr); }
		template<typename... Args>
		//! 書式付きで ログファイル( Log.txt ) に文字列を出力する( 書式は printf と同じ )
		inline int ErrorLogFmtAdd(const TCHAR *FormatString, Args&&... args){ return DxLib::ErrorLogFmtAdd(FormatString, std::forward<Args>(args)...); }
		//! ログファイル( Log.txt ) に出力する文字列の前に付けるタブの数を一つ増やす
		inline int ErrorLogTabAdd(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ErrorLogTabAdd(); }
		//! ログファイル( Log.txt ) に出力する文字列の前に付けるタブの数を一つ減らす
		inline int ErrorLogTabSub(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ErrorLogTabSub(); }
		//! ログファイル( Log.txt ) に出力する文字列の前に起動してからの時間を付けるかどうかを設定する( TRUE:付ける( デフォルト)  FALSE:付けない )
		inline int SetUseTimeStampFlag(bool UseFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseTimeStampFlag(UseFlag); }
		template<typename... Args>
		//! ErrorLogFmtAdd と同じ機能の関数
		inline int AppLogAdd(const TCHAR *String, Args&&... args){ return DxLib::AppLogAdd(String, std::forward<Args>(args)...); }

		//! ログファイル( Log.txt ) を作成するかどうかを設定する( TRUE:作成する( デフォルト )  FALSE:作成しない )、DxLib_Init の前でのみ使用可能　
		inline int SetOutApplicationLogValidFlag(bool Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetOutApplicationLogValidFlag(Flag); }
		//! ログファイル( Log.txt ) を保存するディレクトリパスを設定する
		inline int SetApplicationLogSaveDirectory(const TCHAR *DirectoryPath)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetApplicationLogSaveDirectory(DirectoryPath); }
		//! ログファイル名に日付をつけるかどうかをセットする( TRUE:付ける  FALSE:付けない( デフォルト ) )
		inline int SetUseDateNameLogFile(bool Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseDateNameLogFile(Flag); }

#ifndef DX_NON_PRINTF_DX

		// ログ出力機能関数

		//! printfDx の結果を画面に出力するかどうかを設定する、TRUE:出力を行う  FALSE:出力を行わない( printfDx を実行すると内部で SetLogDrawOutFlag( TRUE ) ; が呼ばれます )
		inline int SetLogDrawOutFlag(bool DrawFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetLogDrawOutFlag(DrawFlag); }
		//! printfDx の結果を画面に出力するかどうかの設定を取得する( 戻り値  TRUE:出力を行う  FALSE:出力を行わない )
		inline int GetLogDrawFlag(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetLogDrawFlag(); }
		//! printfDx の結果を画面に出力する際に使用するフォントのサイズを設定する
		inline int SetLogFontSize(int Size)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetLogFontSize(Size); }

		// 簡易画面出力関数

		template<typename... Args>
		//! printf と同じ引数で画面に文字列を表示するための関数
		inline int printfDx(const TCHAR *FormatString, Args&&... args){ return DxLib::printfDx(FormatString, std::forward<Args>(args)...); }
		//! printfDx の結果をリセットするための関数
		inline int clsDx(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::clsDx(); }
#endif // DX_NON_PRINTF_DX

#endif // DX_NON_LOG
	}


	//----------非同期読み込み関係----------//

	//!非同期読み込み関係
	namespace DxAsyncLoad
	{
#ifndef DX_NON_ASYNCLOAD
		//! 読み込み処理系の関数で非同期読み込みを行うかどうかを設定する( 非同期読み込みに対応している関数のみ有効 )
		//!@param[in] Flag: TRUE:非同期読み込みを行う  FALSE:非同期読み込みを行わない( デフォルト )
		inline int SetUseASyncLoadFlag(bool Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseASyncLoadFlag(Flag); }
		//! ハンドルの非同期読み込みが完了しているかどうかを取得する
		//!@return TRUE:まだ完了していない  FALSE:完了している  -1:エラー
		inline int CheckHandleASyncLoad(int Handle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::CheckHandleASyncLoad(Handle); }
		//! ハンドルの非同期読み込み処理の戻り値を取得する( 非同期読み込み中の場合は一つ前の非同期読み込み処理の戻り値が返ってきます )
		inline int GetHandleASyncLoadResult(int Handle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetHandleASyncLoadResult(Handle); }
		//! 非同期読み込み中の処理の数を取得する
		inline int GetASyncLoadNum(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetASyncLoadNum(); }
#endif // DX_NON_ASYNCLOAD
	}


	//----------ハンドル関係----------//

	//!ハンドル関係
	namespace DxHandle
	{
		//!ハンドルが削除されたときに－１が設定される変数を登録する
		//!@attention DxLibExによって管理されているハンドルに使用してはいけない
		inline int SetDeleteHandleFlag(int Handle, int *DeleteFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetDeleteHandleFlag(Handle, DeleteFlag); }
	}


	//----------マウス関係----------//

	//!XNA互換のマウス入力など
	namespace nMouse
	{
		//!XNA互換
		enum class ButtonState : bool
		{
			//!The button is released.（ボタンは放されている）
			Released = true,
			//!The button is pressed.（ボタンは押されている）
			Pressed = false,
		};
		//!XNA互換
		//!マウスの現在の状態 (マウスの位置や押されているボタンなど)
		struct MouseState
		{
			MouseState()DXLE_NOEXCEPT_OR_NOTHROW;

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
			MouseState(int x, int y, int scrollWheel, ButtonState leftButton, ButtonState middleButton, ButtonState rightButton, ButtonState xButton1, ButtonState xButton2)DXLE_NOEXCEPT_OR_NOTHROW;

			bool operator !=(const MouseState& right)DXLE_NOEXCEPT_OR_NOTHROW { return !(*this == right); }
			bool operator ==(const MouseState& right)DXLE_NOEXCEPT_OR_NOTHROW;
			//! マウスの左ボタンの状態
			ButtonState LeftButton;
			//! マウスの中央ボタンの状態。
			ButtonState MiddleButton;
			//! マウスの右ボタンの状態
			ButtonState RightButton;
			//! マウスのスクロール ホイールの累積値 (ゲーム開始時点or最後にGetState(true)が呼び出されてから)
			int ScrollWheelValue;
			//! マウス カーソルの水平位置 
			int X;
			//! XBUTTON1 の状態
			ButtonState XButton1;
			//! XBUTTON2 の状態
			ButtonState XButton2;
			//! マウス カーソルの垂直位置
			int Y;
		};
		//! 押されたボタンの情報
		struct MouseInputInfo
		{
			//! マウス カーソルの水平位置
			int X;
			//! マウス カーソルの垂直位置
			int Y;
			//!押されたボタン( MOUSE_INPUT_LEFT 等 )
			int Type;

			MouseInputInfo()DXLE_NOEXCEPT_OR_NOTHROW;
			MouseInputInfo(int x, int y, int type)DXLE_NOEXCEPT_OR_NOTHROW;
		};
		//! マウス関係
		//! XNA互換+α
		class Mouse
		{
		public:
			//!マウスの現在の状態 (マウスの位置や押されているボタンなど) を取得します。
			//!@param ScrollWheelValueReset GetMouseWheelRotVolの引数CounterResetと同じ
			static MouseState GetState(bool ScrollWheelValueReset = false)DXLE_NOEXCEPT_OR_NOTHROW;
			//!ウィンドウの左上隅を基準とした、マウス カーソルの位置を設定します。
			static inline void SetPosition(int x, int y)DXLE_NOEXCEPT_OR_NOTHROW { SetMousePoint(x, y); }

			//!マウスのクリック履歴をまとめて取得する
			//!参照：DxLib::GetMouseInputLog http://homepage2.nifty.com/natupaji/DxLib/function/dxfunc_input.html#R5N36
			//!@attention 内部でGetMouseInputLogのLogDeleteにTRUEを渡して呼び出します
			static std::queue<MouseInputInfo> GetMouseInputLog();
		};


		//--------以下DxLibの機械変換

		//! マウスポインタの表示状態を設定する
		//!@param[in] DispFlag:マウスポインタを表示するかどうか( TRUE:表示する  FALSE:表示しない )
		inline int SetMouseDispFlag(bool DispFlag) DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::SetMouseDispFlag(DispFlag); }
		//! マウスポインタの位置を取得する
		inline int GetMousePoint(int *XBuf, int *YBuf) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMousePoint(XBuf, YBuf); }
		//! マウスポインタの位置を設定する
		inline int SetMousePoint(int PointX, int PointY) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetMousePoint(PointX, PointY); }
#ifndef DX_NON_INPUT
		//! マウスのボタンの押下状態を取得する
		inline int GetMouseInput(void) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMouseInput(); }
		//! 垂直マウスホイールの回転量を取得する
		inline int GetMouseWheelRotVol(bool CounterReset = true) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMouseWheelRotVol(CounterReset); }
		//! 水平マウスホイールの回転量を取得する
		inline int GetMouseHWheelRotVol(bool CounterReset = true) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMouseHWheelRotVol(CounterReset); }
		//! 垂直マウスホイールの回転量を取得する( 戻り値が float 型 )
		inline float GetMouseWheelRotVolF(bool CounterReset = true) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMouseWheelRotVolF(CounterReset); }
		//! 水平マウスホイールの回転量を取得する( 戻り値が float 型 )
		inline float GetMouseHWheelRotVolF(int CounterReset = true) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMouseHWheelRotVolF(CounterReset); }
		//! マウスのクリック情報を一つ取得する
		//!@param[out] Button:クリックされたボタン( MOUSE_INPUT_LEFT 等 )を格納する変数のアドレス
		//!@param[out] ClickX:クリックされた時のＸ座標を格納する変数のアドレス
		//!@param[out] ClickY:クリックされた時のＹ座標を格納する変数のアドレス
		//!@param[out] LogDelete:取得したクリック情報一つ分をログから削除するかどうか( TRUE:削除する  FALSE:削除しない、つまり次にこの関数が呼ばれたときに同じ値を取得することになる )
		//!@return  0:クリック情報取得できた　-1:クリック情報が無かった、つまり前回の呼び出し( または起動時から最初の呼び出し )の間に一度もマウスのボタンがクリックされなかった )
		inline int GetMouseInputLog(int *Button, int *ClickX, int *ClickY, bool LogDelete = true) DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMouseInputLog(Button, ClickX, ClickY, LogDelete); }
#endif // DX_NON_INPUT

	}
	namespace DxLibEx_Classes { 
		using nMouse::ButtonState;
		using nMouse::MouseState;
		using nMouse::MouseInputInfo;
		using nMouse::Mouse;
	}


	//----------タッチパネル入力関係関数----------//

	//!タッチパネル入力関係
	namespace Touch
	{
		//! タッチされている数を取得する
		inline int GetTouchInputNum(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetTouchInputNum(); }

		//! タッチの情報を取得する
		inline int GetTouchInput(int InputNo, int *PositionX, int *PositionY, int *ID = NULL, int *Device = NULL)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetTouchInput(InputNo, PositionX, PositionY, ID, Device); }

		//! ストックされているタッチ情報の数を取得する
		inline int GetTouchInputLogNum(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetTouchInputLogNum(); }

		//! ストックされているタッチ情報から一番古い情報をひとつ取得する
		inline TOUCHINPUTDATA GetTouchInputLogOne(int PeekFlag = FALSE)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetTouchInputLogOne(PeekFlag); }

		//! ストックされているタッチ情報から古い順に指定数バッファに取得する( 戻り値  -1:エラー  0以上:取得した情報の数 )
		inline int GetTouchInputLog(TOUCHINPUTDATA *TouchData, int GetNum, int PeekFlag = FALSE)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetTouchInputLog(TouchData, GetNum, PeekFlag); }
	}

	//----------メモリ確保系関数----------//

	//!DxLibの動的確保系関数をスマートポインタで使う(現在使用不可)
	namespace DxMemory
	{
#if 0
		//デストラクタ呼び出しコードを組んでないので使えません

		//----------Dx_Unique_Ptr----------//

		template<typename T>
		class Dx_Unique_Ptr
		{
			typedef Dx_Unique_Ptr<T> This_T;
		public:
			typedef T* pointer;//!所有するリソースのポインタ型T*
			typedef T element_type;//!要素型T

			//----------メモリ確保系関数(New)
			//単一オブジェクト版

			template<typename... Args>
			//!@param[in] File DxAllocの引数Fileと同じ
			//!@param[in] Line DxAllocの引数Lineと同じ
			static This_T NewWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, Args&&... args);
			template<typename... Args>
			static This_T New(Args&&... args){ return NewWithLog(__FILE__, __LINE__, std::forward<Args>(args)...); }

			//----------コンストラクタ
		public:
			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Unique_Ptr() DXLE_NOEXCEPT_OR_NOTHROW{}
			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Unique_Ptr(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{}
			//!otherから*thisに、所有権を譲渡する。
			Dx_Unique_Ptr(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)){}
		private:
			//!ポインタからの構築禁止(実装用コンストラクタ)
			explicit Dx_Unique_Ptr(pointer param, void* resource) DXLE_NOEXCEPT_OR_NOTHROW : ptr(param, Deleter(resource)){}
		public:
			template <typename U, typename = typename std::enable_if<!std::is_array<U>::value && std::is_convertible<typename Dx_Unique_Ptr<U>::pointer, pointer>::value>::type>
			//!変換可能なポインタ型を持つDx_Unique_Ptrオブジェクトからの変換。
			//!以下の条件を満たさない場合、この関数はオーバーロード解決の候補から外れる：
			//!　・Dx_Unique_Ptr<U>::pointerが、pointerに暗黙変換可能な型であること。
			//!　・Uが配列型ではないこと。
			Dx_Unique_Ptr(Dx_Unique_Ptr<U>&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)) {}

			//----------operator=
		public:
			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			This_T& operator=(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = nullptr; return *this; }
			//!otherから*thisに、所有権を譲渡する。
			This_T& operator=(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = std::move(other.ptr); return *this; }
		public:
			template <typename U, typename = typename std::enable_if<!std::is_array<U>::value && std::is_convertible<typename Dx_Unique_Ptr<U>::pointer, pointer>::value>::type>
			//!変換可能なポインタ型を持つDx_Unique_Ptrオブジェクトからの変換。
			//!以下の条件を満たさない場合、この関数はオーバーロード解決の候補から外れる：
			//!　・Dx_Unique_Ptr<U>::pointerが、pointerに暗黙変換可能な型であること。
			//!　・Uが配列型ではないこと。
			This_T& operator=(Dx_Unique_Ptr<U>&& other) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = std::move(other.ptr); return *this; }

			//----------その他のメンバ関数

			//! リソースの所有権を放棄する。
			//! *thisが保持しているリソースの所有権を放棄する。リソースを解放するのではなく、解放する責任を放棄する。
			//! この関数を呼び出したあと、このクラスのデストラクタでは、現在保持しているリソースを解放しなくなる。
			//!@post この関数を呼び出したあと、get()メンバ関数はnullptrを返す。
			//!@return 放棄したリソースを返す。
			pointer release() DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.release(); }

			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			void reset() DXLE_NOEXCEPT_OR_NOTHROW{ ptr.reset(); }
			//!otherから*thisに、所有権を譲渡する。
			void reset(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = std::move(other.ptr); }

			//!リソースを取得する。保持しているポインタを返す。
			//!@attention リソースの所有権は*thisが持っているので、この関数によって返されたポインタに対して、リソース解放をしてはならない。
			pointer get() const DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.get(); }

			//!有効なリソースを所有しているかを判定する。
			explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.operator bool(); }

			//----------単一オブジェクト版固有のメンバ関数

			//!ポインタを間接参照する。
			//!単一オブジェクト版固有のメンバ関数
			typename std::add_lvalue_reference<T>::type operator*() const DXLE_NOEXCEPT_OR_NOTHROW { return *ptr; }

			//!ポインタを通してオブジェクトにアクセスする。
			//!単一オブジェクト版固有のメンバ関数
			pointer operator->() const DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.get(); }

		private:
			struct Deleter{
				void operator()(T* param_ptr) const DXLE_NOEXCEPT_OR_NOTHROW{
					param_ptr->~T();
					operator delete(param_ptr, resource);
					DxLib::DxFree(param_ptr);
				}
				DXLE_CONSTEXPR Deleter() DXLE_NOEXCEPT_OR_NOTHROW :resource(nullptr) {}
				DXLE_CONSTEXPR Deleter(void* param) DXLE_NOEXCEPT_OR_NOTHROW :resource(param) {}
				void* resource;
			};

			std::unique_ptr<T, Deleter> ptr;
		};
		template<>
		class Dx_Unique_Ptr<void>
		{
			template<typename>struct DUMY : std::true_type {};
			template<typename... Args>
			Dx_Unique_Ptr(Args&&...) {
				static_assert(DUMY<Args>::value, "can't use void in Dx_Unique_Ptr.");
			}
		};
		template<typename T>
		class Dx_Unique_Ptr<T[]>
		{
			typedef Dx_Unique_Ptr<T[]> This_T;
		public:
			typedef T* pointer;//!所有するリソースのポインタ型T*
			typedef T element_type;//!要素型T

			//----------メモリ確保系関数(New)
			//配列版

			//!@param[in] File DxAllocの引数Fileと同じ
			//!@param[in] Line DxAllocの引数Lineと同じ
			//!@param[in] n    配列の要素数
			static This_T NewWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n);
			//!@param[in] n    配列の要素数
			static This_T New(size_t n) { return NewWithLog(__FILE__, __LINE__, n); }
	#if 0
			//!メモリの再確保を行う（配列版のみ）
			//!@attention 途中で例外が投げられた場合baceは必ず無効になります
			//!@param[in,out] bace 基になるDx_Unique_Ptr
			//!@param[in] File DxAllocの引数Fileと同じ
			//!@param[in] Line DxAllocの引数Lineと同じ
			//!@param[in] n    新しい配列の要素数
			static This_T ReallocWithLog(This_T& bace, const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n);
			//!メモリの再確保を行う（配列版のみ）
			//!@attention 途中で例外が投げられた場合baceは必ず無効になります
			//!@param[in,out] bace 基になるDx_Unique_Ptr
			//!@param[in] n    新しい配列の要素数
			static This_T Realloc(This_T& bace, size_t n) { return ReallocWithLog(bace, __FILE__, __LINE__, n); }
	#endif


			//----------コンストラクタ
		public:
			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Unique_Ptr() DXLE_NOEXCEPT_OR_NOTHROW {}
			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Unique_Ptr(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW {}
			//!otherから*thisに、所有権を譲渡する。
			Dx_Unique_Ptr(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)) {}
		private:
			//!コピー構築禁止。(delete)
			Dx_Unique_Ptr(const This_T& other) DXLE_NOEXCEPT_OR_NOTHROW;// = delete
			//!ポインタからの構築禁止(実装用コンストラクタ)
			explicit Dx_Unique_Ptr(pointer param) DXLE_NOEXCEPT_OR_NOTHROW : ptr(param) {}

			//----------operator=
		public:
			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			This_T& operator=(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW { ptr = nullptr; return *this; }
			//!otherから*thisに、所有権を譲渡する。
			This_T& operator=(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW { ptr = std::move(other.ptr); return *this; }
		private:
			//!コピー禁止。(delete)
			This_T& operator=(const This_T& other) DXLE_NOEXCEPT_OR_NOTHROW;// = delete

			//----------その他のメンバ関数

			//! リソースの所有権を放棄する。
			//! *thisが保持しているリソースの所有権を放棄する。リソースを解放するのではなく、解放する責任を放棄する。
			//! この関数を呼び出したあと、このクラスのデストラクタでは、現在保持しているリソースを解放しなくなる。
			//!@post この関数を呼び出したあと、get()メンバ関数はnullptrを返す。
			//!@return 放棄したリソースを返す。
			pointer release() DXLE_NOEXCEPT_OR_NOTHROW { return ptr.release(); }

			//!所有権を持たない、空のDx_Unique_Ptrオブジェクトを構築する。
			void reset() DXLE_NOEXCEPT_OR_NOTHROW { ptr.reset(); }
			//!otherから*thisに、所有権を譲渡する。
			void reset(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW { ptr = std::move(other.ptr); }

			//!リソースを取得する。保持しているポインタを返す。
			//!@attention リソースの所有権は*thisが持っているので、この関数によって返されたポインタに対して、リソース解放をしてはならない。
			pointer get() const DXLE_NOEXCEPT_OR_NOTHROW { return ptr.get(); }

			//!有効なリソースを所有しているかを判定する。
			explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW { return ptr.operator bool(); }

			//----------配列版固有のメンバ関数

			//!任意の位置の要素にアクセスする。
			//!配列版固有のメンバ関数
			T& operator[](size_t i) const { return ptr[i]; }

		private:
			struct Deleter {
				void operator()(T* param_ptr) const DXLE_NOEXCEPT_OR_NOTHROW { DxLib::DxFree(param_ptr); }
			private:
				template<typename U>
				void operator()(U* param_ptr) const DXLE_NOEXCEPT_OR_NOTHROW;// = delete
			};

			std::unique_ptr<T, Deleter> ptr;
		};
		template<typename T, size_t N>
		class Dx_Unique_Ptr<T[N]>{
			static_assert(N && !N, "can't use fixed size arrays in Dx_Unique_Ptr. you can use std::array<T, N> instead.");
		};
		//----------非メンバ関数
		
		template <typename T1, typename T2>
		bool operator==(const Dx_Unique_Ptr<T1>& a, const Dx_Unique_Ptr<T2>& b){ return a.get() == b.get(); }
		template <typename T>
		bool operator==(const Dx_Unique_Ptr<T>& x, nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{ return !x; }
		template <typename T>
		bool operator==(nullptr_t, const Dx_Unique_Ptr<T>& x) DXLE_NOEXCEPT_OR_NOTHROW{ return !x; }
		template <typename T1, typename T2>
		bool operator!=(const Dx_Unique_Ptr<T1>& a, const Dx_Unique_Ptr<T2>& b){ return a.get() != b.get(); }
		template <typename T>
		bool operator!=(const Dx_Unique_Ptr<T>& x, nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{ return static_cast<bool>(x); }
		template <typename T>
		bool operator!=(nullptr_t, const Dx_Unique_Ptr<T>& x) DXLE_NOEXCEPT_OR_NOTHROW{ return static_cast<bool>(x); }

		template<typename T, typename... Args, typename = typename std::enable_if<!std::is_array<T>::value>::type>
		//!Dx_Unique_Ptrオブジェクトを構築するヘルパ関数（単一オブジェクト版）
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		Dx_Unique_Ptr<T> MakeUniqueDxWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, Args&&... args);
		template<typename T, typename = typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0>::type>
		//!Dx_Unique_Ptrオブジェクトを構築するヘルパ関数（配列版）
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		Dx_Unique_Ptr<T> MakeUniqueDxWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n);

		template<typename T, typename... Args, typename = typename std::enable_if<!std::is_array<T>::value>::type>
		//!Dx_Unique_Ptrオブジェクトを構築するヘルパ関数（単一オブジェクト版）
		Dx_Unique_Ptr<T> MakeUniqueDx(Args&&... args) { return MakeUniqueDxWithLog<T>(__FILE__, __LINE__ ,std::forward<Args>(args)...); }
		template<typename T, typename = typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0>::type>
		//!Dx_Unique_Ptrオブジェクトを構築するヘルパ関数（配列版）
		Dx_Unique_Ptr<T> MakeUniqueDx(size_t n) { return MakeUniqueDxWithLog<T>(__FILE__, __LINE__, n); }

		//----------Dx_Shared_Ptr----------//

		template<typename T>
		class Dx_Shared_Ptr
		{
			typedef Dx_Shared_Ptr<T> This_T;
		public:
			typedef T* pointer;//!所有するリソースのポインタ型T*
			typedef T element_type;//!要素型T

			//----------メモリ確保系関数(New)
			//単一オブジェクト版

			template<typename... Args>
			//!@param[in] File DxAllocの引数Fileと同じ
			//!@param[in] Line DxAllocの引数Lineと同じ
			static This_T NewWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, Args&&... args);
			template<typename... Args>
			static This_T New(Args&&... args){ return NewWithLog(__FILE__, __LINE__, std::forward<Args>(args)...); }

			//----------コンストラクタ
		public:
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Shared_Ptr() DXLE_NOEXCEPT_OR_NOTHROW{}
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Shared_Ptr(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{}
			//!otherから*thisに、所有権を譲渡する。
			Dx_Shared_Ptr(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)){}
		private:
			//!ポインタからの構築禁止(実装用コンストラクタ)
			explicit Dx_Shared_Ptr(pointer param) DXLE_NOEXCEPT_OR_NOTHROW : ptr(param){}
		public:
			template <typename U, typename = typename std::enable_if<!std::is_array<U>::value && std::is_convertible<typename Dx_Shared_Ptr<U>::pointer, pointer>::value>::type>
			//!変換可能なポインタ型を持つDx_Shared_Ptrオブジェクトからの変換。
			//!以下の条件を満たさない場合、この関数はオーバーロード解決の候補から外れる：
			//!　・Dx_Shared_Ptr<U>::pointerが、pointerに暗黙変換可能な型であること。
			//!　・Uが配列型ではないこと。
			Dx_Shared_Ptr(Dx_Shared_Ptr<U>&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)){}

			//----------operator=
		public:
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			This_T& operator=(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{ reset(); return *this; }
			//!otherから*thisに、所有権を譲渡する。
			This_T& operator=(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = std::move(other.ptr); return *this; }
		public:
			template <typename U, typename = typename std::enable_if<!std::is_array<U>::value && std::is_convertible<typename Dx_Shared_Ptr<U>::pointer, pointer>::value>::type>
			//!変換可能なポインタ型を持つDx_Shared_Ptrオブジェクトからの変換。
			//!以下の条件を満たさない場合、この関数はオーバーロード解決の候補から外れる：
			//!　・Dx_Shared_Ptr<U>::pointerが、pointerに暗黙変換可能な型であること。
			//!　・Uが配列型ではないこと。
			This_T& operator=(Dx_Shared_Ptr<U>&& other) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = std::move(other.ptr); return *this; }

			//----------その他のメンバ関数

			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			void reset() DXLE_NOEXCEPT_OR_NOTHROW{ ptr.reset(); }
			//!otherから*thisに、所有権を譲渡する。
			void reset(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW{ ptr = std::move(other.ptr); }

			//!リソースを取得する。保持しているポインタを返す。
			//!@attention リソースの所有権は*thisが持っているので、この関数によって返されたポインタに対して、リソース解放をしてはならない。
			pointer get() const DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.get(); }

			//!有効なリソースを所有しているかを判定する。
			explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.operator bool(); }

			//----------単一オブジェクト版固有のメンバ関数

			//!ポインタを間接参照する。
			//!単一オブジェクト版固有のメンバ関数
			typename std::add_lvalue_reference<T>::type operator*() const DXLE_NOEXCEPT_OR_NOTHROW { return *ptr; }

			//!ポインタを通してオブジェクトにアクセスする。
			//!単一オブジェクト版固有のメンバ関数
			pointer operator->() const DXLE_NOEXCEPT_OR_NOTHROW{ return ptr.get(); }

		private:
			struct Deleter{
				void operator()(T* param_ptr) const DXLE_NOEXCEPT_OR_NOTHROW{ DxLib::DxFree(param_ptr); }
			};

			std::shared_ptr<T, Deleter> ptr;
		};
		template<>
		class Dx_Shared_Ptr<void>
		{
			typedef Dx_Shared_Ptr<void> This_T;
		public:
			typedef void* pointer;//!所有するリソースのポインタ型T*
			typedef void element_type;//!要素型T

			//----------コンストラクタ
		public:
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Shared_Ptr() DXLE_NOEXCEPT_OR_NOTHROW : ptr(nullptr){}
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Shared_Ptr(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW : ptr(nullptr) {}
			//!otherから*thisに、所有権を譲渡する。
			Dx_Shared_Ptr(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)) {}
		private:
			//!コピー構築禁止。(delete)
			Dx_Shared_Ptr(const This_T& other) DXLE_NOEXCEPT_OR_NOTHROW;// = delete
			//!ポインタからの構築禁止(実装用コンストラクタ)
			explicit Dx_Shared_Ptr(pointer param) DXLE_NOEXCEPT_OR_NOTHROW : ptr(param) {}
		public:
			template <typename U, typename = typename std::enable_if<!std::is_array<U>::value && std::is_convertible<typename Dx_Shared_Ptr<U>::pointer, pointer>::value>::type>
			//!変換可能なポインタ型を持つDx_Shared_Ptrオブジェクトからの変換。
			//!以下の条件を満たさない場合、この関数はオーバーロード解決の候補から外れる：
			//!　・Dx_Shared_Ptr<U>::pointerが、pointerに暗黙変換可能な型であること。
			//!　・Uが配列型ではないこと。
			Dx_Shared_Ptr(Dx_Shared_Ptr<U>&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(other.release()) {}

			//----------operator=
		public:
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			This_T& operator=(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW { reset(); return *this; }
			//!otherから*thisに、所有権を譲渡する。
			This_T& operator=(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW { reset(std::move(other)); return *this; }
		private:
			//!コピー禁止。(delete)
			This_T& operator=(const This_T& other) DXLE_NOEXCEPT_OR_NOTHROW;// = delete
		public:
			template <typename U, typename = typename std::enable_if<!std::is_array<U>::value && std::is_convertible<typename Dx_Shared_Ptr<U>::pointer, pointer>::value>::type>
			//!変換可能なポインタ型を持つDx_Shared_Ptrオブジェクトからの変換。
			//!以下の条件を満たさない場合、この関数はオーバーロード解決の候補から外れる：
			//!　・Dx_Shared_Ptr<U>::pointerが、pointerに暗黙変換可能な型であること。
			//!　・Uが配列型ではないこと。
			This_T& operator=(Dx_Shared_Ptr<U>&& other) DXLE_NOEXCEPT_OR_NOTHROW { reset(); ptr = other.release(); return *this; }

			//----------その他のメンバ関数

			//! リソースの所有権を放棄する。
			//! *thisが保持しているリソースの所有権を放棄する。リソースを解放するのではなく、解放する責任を放棄する。
			//! この関数を呼び出したあと、このクラスのデストラクタでは、現在保持しているリソースを解放しなくなる。
			//!@post この関数を呼び出したあと、get()メンバ関数はnullptrを返す。
			//!@return 放棄したリソースを返す。
			pointer release() DXLE_NOEXCEPT_OR_NOTHROW { auto temp = ptr; ptr = nullptr; return temp; }

			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			void reset() DXLE_NOEXCEPT_OR_NOTHROW { DxLib::DxFree(ptr); ptr = nullptr; }
			//!otherから*thisに、所有権を譲渡する。
			void reset(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW { if (this != &other) { reset(); ptr = other.release(); } }

			//!リソースを取得する。保持しているポインタを返す。
			//!@attention リソースの所有権は*thisが持っているので、この関数によって返されたポインタに対して、リソース解放をしてはならない。
			pointer get() const DXLE_NOEXCEPT_OR_NOTHROW { return ptr; }

			//!有効なリソースを所有しているかを判定する。
			explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW { return ptr != nullptr; }

			//----------デストラクタ

			~Dx_Shared_Ptr() DXLE_NOEXCEPT_OR_NOTHROW { reset(); }
		private:
			void* ptr;
		};
		template<typename T>
		class Dx_Shared_Ptr<T[]>
		{
			typedef Dx_Shared_Ptr<T[]> This_T;
		public:
			typedef T* pointer;//!所有するリソースのポインタ型T*
			typedef T element_type;//!要素型T

			//----------メモリ確保系関数(New)
			//配列版

			//!@param[in] File DxAllocの引数Fileと同じ
			//!@param[in] Line DxAllocの引数Lineと同じ
			//!@param[in] n    配列の要素数
			static This_T NewWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n);
			//!@param[in] n    配列の要素数
			static This_T New(size_t n) { return NewWithLog(__FILE__, __LINE__, n); }
	#if 0
			//!メモリの再確保を行う（配列版のみ）
			//!@attention 途中で例外が投げられた場合baceは必ず無効になります
			//!@param[in,out] bace 基になるDx_Shared_Ptr
			//!@param[in] File DxAllocの引数Fileと同じ
			//!@param[in] Line DxAllocの引数Lineと同じ
			//!@param[in] n    新しい配列の要素数
			static This_T ReallocWithLog(This_T& bace, const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n);
			//!メモリの再確保を行う（配列版のみ）
			//!@attention 途中で例外が投げられた場合baceは必ず無効になります
			//!@param[in,out] bace 基になるDx_Shared_Ptr
			//!@param[in] n    新しい配列の要素数
			static This_T Realloc(This_T& bace, size_t n) { return ReallocWithLog(bace, __FILE__, __LINE__, n); }
	#endif


			//----------コンストラクタ
		public:
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Shared_Ptr() DXLE_NOEXCEPT_OR_NOTHROW {}
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			DXLE_CONSTEXPR Dx_Shared_Ptr(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW {}
			//!otherから*thisに、所有権を譲渡する。
			Dx_Shared_Ptr(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW : ptr(std::move(other.ptr)) {}
		private:
			//!コピー構築禁止。(delete)
			Dx_Shared_Ptr(const This_T& other) DXLE_NOEXCEPT_OR_NOTHROW;// = delete
			//!ポインタからの構築禁止(実装用コンストラクタ)
			explicit Dx_Shared_Ptr(pointer param) DXLE_NOEXCEPT_OR_NOTHROW : ptr(param) {}

			//----------operator=
		public:
			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			This_T& operator=(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW { ptr = nullptr; return *this; }
			//!otherから*thisに、所有権を譲渡する。
			This_T& operator=(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW { ptr = std::move(other.ptr); return *this; }
		private:
			//!コピー禁止。(delete)
			This_T& operator=(const This_T& other) DXLE_NOEXCEPT_OR_NOTHROW;// = delete

			//----------その他のメンバ関数

			//! リソースの所有権を放棄する。
			//! *thisが保持しているリソースの所有権を放棄する。リソースを解放するのではなく、解放する責任を放棄する。
			//! この関数を呼び出したあと、このクラスのデストラクタでは、現在保持しているリソースを解放しなくなる。
			//!@post この関数を呼び出したあと、get()メンバ関数はnullptrを返す。
			//!@return 放棄したリソースを返す。
			pointer release() DXLE_NOEXCEPT_OR_NOTHROW { return ptr.release(); }

			//!所有権を持たない、空のDx_Shared_Ptrオブジェクトを構築する。
			void reset() DXLE_NOEXCEPT_OR_NOTHROW { ptr.reset(); }
			//!otherから*thisに、所有権を譲渡する。
			void reset(This_T&& other) DXLE_NOEXCEPT_OR_NOTHROW { ptr = std::move(other.ptr); }

			//!リソースを取得する。保持しているポインタを返す。
			//!@attention リソースの所有権は*thisが持っているので、この関数によって返されたポインタに対して、リソース解放をしてはならない。
			pointer get() const DXLE_NOEXCEPT_OR_NOTHROW { return ptr.get(); }

			//!有効なリソースを所有しているかを判定する。
			explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW { return ptr.operator bool(); }

			//----------配列版固有のメンバ関数

			//!任意の位置の要素にアクセスする。
			//!配列版固有のメンバ関数
			T& operator[](size_t i) const { return ptr[i]; }

		private:
			struct Deleter {
				void operator()(T* param_ptr) const DXLE_NOEXCEPT_OR_NOTHROW { DxLib::DxFree(param_ptr); }
			private:
				template<typename U>
				void operator()(U* param_ptr) const DXLE_NOEXCEPT_OR_NOTHROW;// = delete
			};

			std::shared_ptr<T, Deleter> ptr;
		};
		template<typename T, size_t N>
		class Dx_Shared_Ptr<T[N]>{
			static_assert(N && !N, "can't use fixed size arrays in Dx_Shared_Ptr. you can use std::array<T, N> instead.");
		};
		//----------非メンバ関数
		
		template <typename T1, typename T2>
		bool operator==(const Dx_Shared_Ptr<T1>& a, const Dx_Shared_Ptr<T2>& b){ return a.get() == b.get(); }
		template <typename T>
		bool operator==(const Dx_Shared_Ptr<T>& x, nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{ return !x; }
		template <typename T>
		bool operator==(nullptr_t, const Dx_Shared_Ptr<T>& x) DXLE_NOEXCEPT_OR_NOTHROW{ return !x; }
		template <typename T1, typename T2>
		bool operator!=(const Dx_Shared_Ptr<T1>& a, const Dx_Shared_Ptr<T2>& b){ return a.get() != b.get(); }
		template <typename T>
		bool operator!=(const Dx_Shared_Ptr<T>& x, nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW{ return static_cast<bool>(x); }
		template <typename T>
		bool operator!=(nullptr_t, const Dx_Shared_Ptr<T>& x) DXLE_NOEXCEPT_OR_NOTHROW{ return static_cast<bool>(x); }

		template<typename T, typename... Args, typename = typename std::enable_if<!std::is_array<T>::value>::type>
		//!Dx_Shared_Ptrオブジェクトを構築するヘルパ関数（単一オブジェクト版）
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		Dx_Shared_Ptr<T> MakeSharedDxWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, Args&&... args);
		template<typename T, typename = typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0>::type>
		//!Dx_Shared_Ptrオブジェクトを構築するヘルパ関数（配列版）
		//!@param[in] File DxAllocの引数Fileと同じ
		//!@param[in] Line DxAllocの引数Lineと同じ
		Dx_Shared_Ptr<T> MakeSharedDxWithLog(const char *File/*__FILE__*/, int Line/*__LINE__*/, size_t n);

		template<typename T, typename... Args, typename = typename std::enable_if<!std::is_array<T>::value>::type>
		//!Dx_Shared_Ptrオブジェクトを構築するヘルパ関数（単一オブジェクト版）
		Dx_Shared_Ptr<T> MakeSharedDx(Args&&... args) { return MakeSharedDxWithLog<T>(__FILE__, __LINE__ ,std::forward<Args>(args)...); }
		template<typename T, typename = typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0>::type>
		//!Dx_Shared_Ptrオブジェクトを構築するヘルパ関数（配列版）
		Dx_Shared_Ptr<T> MakeSharedDx(size_t n) { return MakeSharedDxWithLog<T>(__FILE__, __LINE__, n); }

#endif
		//----------DxLibの機械変換

		//! 指定のサイズのメモリを確保する
		//!@param[in] AllocSize 確保するメモリのサイズ( 単位:byte )
		//!@param[in] File DxAllocを呼んだソースファイル名( デバッグ用 )
		//!@param[in] Line DxAllocを呼んだソースファイル中の行番号( デバッグ用 )
		//!@return NULL:メモリの確保失敗   NULL以外:確保したメモリ領域の先頭アドレス )
		inline void* DxAlloc(size_t AllocSize, const char *File = NULL, int Line = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxAlloc(AllocSize, File, Line); }
		//! 指定のサイズのメモリを確保して、０で埋める、初期化後に０で埋める以外は DxAlloc と動作は同じ
		inline void* DxCalloc(size_t AllocSize, const char *File = NULL, int Line = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxCalloc(AllocSize, File, Line); }
		//! メモリの再確保を行う
		//!@param[in] Memory 再確保を行うメモリ領域の先頭アドレス( DxAlloc の戻り値 )
		//!@param[in] File DxAllocを呼んだソースファイル名( デバッグ用 )
		//!@param[in] Line DxAllocを呼んだソースファイル中の行番号( デバッグ用 )
		//!@return NULL:メモリの再確保失敗　NULL以外:再確保した新しいメモリ領域の先頭アドレス
		inline void* DxRealloc(void *Memory, size_t AllocSize, const char *File = NULL, int Line = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxRealloc(Memory, AllocSize, File, Line); }
		//!@param[in] Memory 解放するメモリ領域の先頭アドレス( DxAlloc の戻り値 )
		inline void DxFree(void *Memory)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxFree(Memory); }
		//! 列挙対象にするメモリの確保容量をセットする
		inline size_t DxSetAllocSizeTrap(size_t Size)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxSetAllocSizeTrap(Size); }
		//! ＤＸライブラリ内でメモリ確保が行われる時に情報を出力するかどうかをセットする
		inline int DxSetAllocPrintFlag(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxSetAllocPrintFlag(Flag); }
		//! DxAlloc や DxCalloc で確保しているメモリサイズを取得する
		inline size_t DxGetAllocSize(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxGetAllocSize(); }
		//! DxAlloc や DxCalloc で確保しているメモリの数を取得する
		inline int DxGetAllocNum(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxGetAllocNum(); }
		//! DxAlloc や DxCalloc で確保しているメモリを列挙する
		inline void DxDumpAlloc(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxDumpAlloc(); }
		//! DxAlloc や DxCalloc で確保しているメモリの状況を描画する
		inline void DxDrawAlloc(int x, int y, int Width, int Height)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxDrawAlloc(x, y, Width, Height); }
		//! 確保したメモリ情報が破壊されていないか調べる( -1:破壊あり  0:なし )
		inline int DxErrorCheckAlloc(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxErrorCheckAlloc(); }
		//! メモリが確保、解放が行われる度に確保しているメモリの容量を出力するかどうかのフラグをセットする
		inline int DxSetAllocSizeOutFlag(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxSetAllocSizeOutFlag(Flag); }
		//! メモリの確保、解放が行われる度に確保しているメモリ確保情報が破損していないか調べるかどうかのフラグをセットする
		inline int DxSetAllocMemoryErrorCheckFlag(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DxSetAllocMemoryErrorCheckFlag(Flag); }

	}
#if 0
	namespace DxLibEx_Classes { 
		using DxMemory::Dx_Unique_Ptr;
		using DxMemory::MakeUniqueDx;
		using DxMemory::MakeUniqueDxWithLog;
		using DxMemory::Dx_Shared_Ptr;
		using DxMemory::MakeSharedDx;
		using DxMemory::MakeSharedDxWithLog;
	}
#endif


	//----------DxBaseFunc.cpp 関数プロトタイプ宣言----------//

	//!文字コード関係ユーティリティ
	namespace DxChar
	{
		// 文字コード関係

		//! 文字列の文字コード形式を別の文字コード形式に変換する
		//!@param[in] SrcCharCodeFormat DX_CHARCODEFORMAT_SHIFTJIS 等
		//!@param[in] DestCharCodeFormat DX_CHARCODEFORMAT_SHIFTJIS 等
		inline int ConvertStringCharCodeFormat(int SrcCharCodeFormat, const void *SrcString, int DestCharCodeFormat, void *DestStringBuffer)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ConvertStringCharCodeFormat(SrcCharCodeFormat, SrcString, DestCharCodeFormat, DestStringBuffer); }
		//! 文字列の引数の文字コード形式を設定する( 文字列描画系関数とその他一部関数を除く )( UNICODE版では無効 )
		//!@param[in] CharCodeFormat DX_CHARCODEFORMAT_SHIFTJIS 等
		inline int SetUseCharCodeFormat(int CharCodeFormat)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseCharCodeFormat(CharCodeFormat); }

		// 文字列関係

		//! strcpy と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strcpyDx(TCHAR *Dest, const TCHAR *Src)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strcpyDx(Dest, Src); }
		//! 位置指定付き strcpy、Pos はコピー開始位置　( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strpcpyDx(TCHAR *Dest, const TCHAR *Src, int Pos)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpcpyDx(Dest, Src, Pos); }
		//! 位置指定付き strcpy、Pos はコピー開始位置( 全角文字も 1 扱い )　( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strpcpy2Dx(TCHAR *Dest, const TCHAR *Src, int Pos)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpcpy2Dx(Dest, Src, Pos); }
		//! strncpy と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strncpyDx(TCHAR *Dest, const TCHAR *Src, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strncpyDx(Dest, Src, Num); }
		//! strncpy の Num が文字数( 全角文字も 1 扱い )になったもの、終端に必ずヌル文字が代入される( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strncpy2Dx(TCHAR *Dest, const TCHAR *Src, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strncpy2Dx(Dest, Src, Num); }
		//! strncpy の文字列の終端からの文字数指定版( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strrncpyDx(TCHAR *Dest, const TCHAR *Src, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strrncpyDx(Dest, Src, Num); }
		//! strncpy の文字列の終端からの文字数( 全角文字も 1 扱い )指定版、終端に必ずヌル文字が代入される( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strrncpy2Dx(TCHAR *Dest, const TCHAR *Src, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strrncpy2Dx(Dest, Src, Num); }
		//! strncpy のコピー開始位置指定版、Pos はコピー開始位置、Num は文字数( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strpncpyDx(TCHAR *Dest, const TCHAR *Src, int Pos, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpncpyDx(Dest, Src, Pos, Num); }
		//! strncpy のコピー開始位置指定版、Pos はコピー開始位置( 全角文字も 1 扱い )、Num は文字数( 全角文字も 1 扱い )、終端に必ずヌル文字が代入される( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strpncpy2Dx(TCHAR *Dest, const TCHAR *Src, int Pos, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpncpy2Dx(Dest, Src, Pos, Num); }
		//! strcat と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline void strcatDx(TCHAR *Dest, const TCHAR *Src)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strcatDx(Dest, Src); }
		//! strlen と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strlenDx(const TCHAR *Str)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strlenDx(Str); }
		//! strlen の戻り値が文字数( 全角文字も 1 扱い )になったもの( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strlen2Dx(const TCHAR *Str)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strlen2Dx(Str); }
		//! strcmp と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strcmpDx(const TCHAR *Str1, const TCHAR *Str2)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strcmpDx(Str1, Str2); }
		//! stricmp と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int stricmpDx(const TCHAR *Str1, const TCHAR *Str2)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::stricmpDx(Str1, Str2); }
		//! strncmp と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strncmpDx(const TCHAR *Str1, const TCHAR *Str2, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strncmpDx(Str1, Str2, Num); }
		//! strncmp の Num が文字数( 全角文字も 1 扱い )になったもの( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strncmp2Dx(const TCHAR *Str1, const TCHAR *Str2, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strncmp2Dx(Str1, Str2, Num); }
		//! strncmp の比較開始位置指定版、Pos が Str1 の比較開始位置、Num が文字数( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strpncmpDx(const TCHAR *Str1, const TCHAR *Str2, int Pos, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpncmpDx(Str1, Str2, Pos, Num); }
		//! strncmp の比較開始位置指定版、Pos が Str1 の比較開始位置( 全角文字も 1 扱い )、Num が文字数( 全角文字も 1 扱い )( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strpncmp2Dx(const TCHAR *Str1, const TCHAR *Str2, int Pos, int Num)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpncmp2Dx(Str1, Str2, Pos, Num); }
		//! 文字列の指定の位置の文字コードを取得する、Pos は取得する位置、CharNums は文字数を代入する変数のアドレス、戻り値は文字コード( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline DWORD strgetchrDx(const TCHAR *Str, int Pos, int *CharNums = NULL)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strgetchrDx(Str, Pos, CharNums); }
		//! 文字列の指定の位置の文字コードを取得する、Pos は取得する位置( 全角文字も 1 扱い )、CharNums は文字数を代入する変数のアドレス、戻り値は文字コード( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline DWORD strgetchr2Dx(const TCHAR *Str, int Pos, int *CharNums = NULL)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strgetchr2Dx(Str, Pos, CharNums); }
		//! 文字列の指定の位置に文字コードを書き込む、Pos は書き込む位置、CharCode は文字コード、戻り値は書き込んだ文字数( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strputchrDx(TCHAR *Str, int Pos, DWORD CharCode)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strputchrDx(Str, Pos, CharCode); }
		//! 文字列の指定の位置に文字コードを書き込む、Pos は書き込む位置( 全角文字も 1 扱い )、CharCode は文字コード、戻り値は書き込んだ文字数( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strputchr2Dx(TCHAR *Str, int Pos, DWORD CharCode)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strputchr2Dx(Str, Pos, CharCode); }
		//! 文字列の指定の位置のアドレスを取得する、Pos は取得する位置　( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline const TCHAR * strposDx(const TCHAR *Str, int Pos)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strposDx(Str, Pos); }
		//! 文字列の指定の位置のアドレスを取得する、Pos は取得する位置( 全角文字も 1 扱い )　( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline const TCHAR * strpos2Dx(const TCHAR *Str, int Pos)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strpos2Dx(Str, Pos); }
		//! strstr と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline const TCHAR * strstrDx(const TCHAR *Str1, const TCHAR *Str2)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strstrDx(Str1, Str2); }
		//! strstr の戻り値が文字列先頭からの文字数( 全角文字も 1 扱い ) になったもの( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strstr2Dx(const TCHAR *Str1, const TCHAR *Str2)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strstr2Dx(Str1, Str2); }
		//! strrstr と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline const TCHAR * strrstrDx(const TCHAR *Str1, const TCHAR *Str2)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strrstrDx(Str1, Str2); }
		//! strrstr の戻り値が文字列先頭からの文字数( 全角文字も 1 扱い ) になったもの( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strrstr2Dx(const TCHAR *Str1, const TCHAR *Str2)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strrstr2Dx(Str1, Str2); }
		//! strchr と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline const TCHAR * strchrDx(const TCHAR *Str, DWORD CharCode)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strchrDx(Str, CharCode); }
		//! strchr の戻り値が文字列先頭からの文字数( 全角文字も 1 扱い ) になったもの( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strchr2Dx(const TCHAR *Str, DWORD CharCode)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strchr2Dx(Str, CharCode); }
		//! strrchr と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline const TCHAR * strrchrDx(const TCHAR *Str, DWORD CharCode)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strrchrDx(Str, CharCode); }
		//! strrchr の戻り値が文字列先頭からの文字数( 全角文字も 1 扱い ) になったもの( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int strrchr2Dx(const TCHAR *Str, DWORD CharCode)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::strrchr2Dx(Str, CharCode); }
		//! strupr と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline TCHAR * struprDx(TCHAR *Str)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::struprDx(Str); }
		//! vsprintf と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int vsprintfDx(TCHAR *Buffer, const TCHAR *FormatString, va_list Arg)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::vsprintfDx(Buffer, FormatString, Arg); }
		//! sprintf と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		template<typename... Args> inline int sprintfDx(TCHAR *Buffer, const TCHAR *FormatString, Args&&... args)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::sprfDx(Buffer, FormatString, std::forward<Args>(args)...); }
		//! itoa と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline TCHAR * itoaDx(int Value, TCHAR *Buffer, int Radix)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::itoaDx(Value, Buffer, Radix); }
		//! atoi と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int atoiDx(const TCHAR *Str)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::atoiDx(Str); }
		//! atof と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline double atofDx(const TCHAR *Str)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::atofDx(Str); }
		//! vsscanf と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		inline int vsscanfDx(const TCHAR *String, const TCHAR *FormatString, va_list Arg)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::vsscanfDx(String, FormatString, Arg); }
		//! sscanf と同等の機能( マルチバイト文字列版では文字コード形式として SetUseCharCodeFormat で設定した形式が使用されます )
		template<typename... Args> inline int sscanfDx(const TCHAR *String, const TCHAR *FormatString, Args&&... args)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::sscanfDx(String, FormatString, std::forward<Args>(args)...); }

	}


	//----------ネットワーク----------//

	//!ネットワーク
	namespace Network
	{
#ifndef DX_NON_NETWORK
		// 通信関係

		//! 通信メッセージの処理をする関数
		inline int ProcessNetMessage(int RunReleaseProcess = FALSE)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ProcessNetMessage(RunReleaseProcess); }

		//! ＤＮＳサーバーを使ってホスト名からＩＰアドレスを取得する( IPv4版 )
		inline int GetHostIPbyName(const TCHAR *HostName, IPDATA* IPDataBuf)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetHostIPbyName(HostName, IPDataBuf); }
		//! ＤＮＳサーバーを使ってホスト名からＩＰアドレスを取得する( IPv6版 )
		inline int GetHostIPbyName_IPv6(const TCHAR *HostName, IPDATA_IPv6 *IPDataBuf)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetHostIPbyName_IPv6(HostName, IPDataBuf); }
		//! 他マシンに接続する( IPv4版 )
		inline int ConnectNetWork(IPDATA      IPData, int Port = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ConnectNetWork(IPData, Port); }
		//! 他マシンに接続する( IPv6版 )
		inline int ConnectNetWork_IPv6(IPDATA_IPv6 IPData, int Port = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ConnectNetWork_IPv6(IPData, Port); }
		//! 他マシンに接続する( IPv4版 )、非同期版
		inline int ConnectNetWork_ASync(IPDATA      IPData, int Port = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ConnectNetWork_ASync(IPData, Port); }
		//! 他マシンに接続する( IPv6版 )、非同期版
		inline int ConnectNetWork_IPv6_ASync(IPDATA_IPv6 IPData, int Port = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ConnectNetWork_IPv6_ASync(IPData, Port); }
		//! 接続を受けられる状態にする( IPv4版 )
		inline int PreparationListenNetWork(int Port = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::PreparationListenNetWork(Port); }
		//! 接続を受けられる状態にする( IPv6版 )
		inline int PreparationListenNetWork_IPv6(int Port = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::PreparationListenNetWork_IPv6(Port); }
		//! 接続を受けつけ状態の解除
		inline int StopListenNetWork(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::StopListenNetWork(); }
		//! 接続を終了する
		inline int CloseNetWork(int NetHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::CloseNetWork(NetHandle); }

		//! 接続状態を取得する
		inline int GetNetWorkAcceptState(int NetHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNetWorkAcceptState(NetHandle); }
		//! 受信データの量を得る
		inline int GetNetWorkDataLength(int NetHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNetWorkDataLength(NetHandle); }
		//! 未送信のデータの量を得る 
		inline int GetNetWorkSendDataLength(int NetHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNetWorkSendDataLength(NetHandle); }
		//! 新たに接続した通信回線を得る
		inline int GetNewAcceptNetWork(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNewAcceptNetWork(); }
		//! 接続を切断された通信回線を得る
		inline int GetLostNetWork(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetLostNetWork(); }
		//! 接続先のＩＰを得る( IPv4版 )
		inline int GetNetWorkIP(int NetHandle, IPDATA      *IpBuf)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNetWorkIP(NetHandle, IpBuf); }
		//! 接続先のＩＰを得る( IPv6版 )
		inline int GetNetWorkIP_IPv6(int NetHandle, IPDATA_IPv6 *IpBuf)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNetWorkIP_IPv6(NetHandle, IpBuf); }
		//! 自分のＩＰを得る
		inline int GetMyIPAddress(IPDATA *IpBuf, int IpBufLength = 1, int *IpNum = NULL)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetMyIPAddress(IpBuf, IpBufLength, IpNum); }
		//! 接続のタイムアウトまでの時間を設定する
		inline int SetConnectTimeOutWait(int Time)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetConnectTimeOutWait(Time); }
		//! ＤＸライブラリの通信形態を使うかどうかをセットする
		inline int SetUseDXNetWorkProtocol(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseDXNetWorkProtocol(Flag); }
		//! ＤＸライブラリの通信形態を使うかどうかを取得する
		inline int GetUseDXNetWorkProtocol(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetUseDXNetWorkProtocol(); }
		//! SetUseDXNetWorkProtocol の別名
		inline int SetUseDXProtocol(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseDXProtocol(Flag); }
		//! GetUseDXNetWorkProtocol の別名
		inline int GetUseDXProtocol(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetUseDXProtocol(); }
		//! 接続が切断された直後に接続ハンドルを解放するかどうかのフラグをセットする
		inline int SetNetWorkCloseAfterLostFlag(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetNetWorkCloseAfterLostFlag(Flag); }
		//! 接続が切断された直後に接続ハンドルを解放するかどうかのフラグを取得する
		inline int GetNetWorkCloseAfterLostFlag(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetNetWorkCloseAfterLostFlag(); }

		//! 受信したデータを読み込む
		inline int NetWorkRecv(int NetHandle, void *Buffer, int Length)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkRecv(NetHandle, Buffer, Length); }
		//! 受信したデータを読み込む、読み込んだデータはバッファから削除されない
		inline int NetWorkRecvToPeek(int NetHandle, void *Buffer, int Length)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkRecvToPeek(NetHandle, Buffer, Length); }
		//! 受信したデータをクリアする
		inline int NetWorkRecvBufferClear(int NetHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkRecvBufferClear(NetHandle); }
		//! データを送信する
		inline int NetWorkSend(int NetHandle, const void *Buffer, int Length)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkSend(NetHandle, Buffer, Length); }

		//! UDPを使用した通信を行うソケットハンドルを作成する( RecvPort を -1 にすると送信専用のソケットハンドルになります )
		inline int MakeUDPSocket(int RecvPort = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::MakeUDPSocket(RecvPort); }
		//! UDPを使用した通信を行うソケットハンドルを作成する( RecvPort を -1 にすると送信専用のソケットハンドルになります )( IPv6版 )
		inline int MakeUDPSocket_IPv6(int RecvPort = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::MakeUDPSocket_IPv6(RecvPort); }
		//! UDPを使用した通信を行うソケットハンドルを削除する
		inline int DeleteUDPSocket(int NetUDPHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DeleteUDPSocket(NetUDPHandle); }
		//! UDPを使用した通信で指定のＩＰにデータを送信する、Length は最大65507、SendPort を -1 にすると MakeUDPSocket に RecvPort で渡したポートが使用されます
		//!@preturn 0以上;送信できたデータサイズ  -1:エラー  -2:送信データが大きすぎる  -3:送信準備ができていない
		inline int NetWorkSendUDP(int NetUDPHandle, IPDATA SendIP, int SendPort, const void *Buffer, int Length)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkSendUDP(NetUDPHandle, SendIP, SendPort, Buffer, Length); }
		//! UDPを使用した通信で指定のＩＰにデータを送信する、Length は最大65507、SendPort を -1 にすると MakeUDPSocket に RecvPort で渡したポートが使用されます( 戻り値  0以上;送信できたデータサイズ  -1:エラー  -2:送信データが大きすぎる  -3:送信準備ができていない  )( IPv6版 )
		inline int NetWorkSendUDP_IPv6(int NetUDPHandle, IPDATA_IPv6  SendIP, int SendPort, const void *Buffer, int Length)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkSendUDP_IPv6(NetUDPHandle, SendIP, SendPort, Buffer, Length); }
		//! UDPを使用した通信でデータを受信する、Peek に TRUE を渡すと受信に成功してもデータを受信キューから削除しません( 戻り値  0以上:受信したデータのサイズ  -1:エラー  -2:バッファのサイズが足りない  -3:受信データがない )
		inline int NetWorkRecvUDP(int NetUDPHandle, IPDATA* RecvIP, int *RecvPort, void *Buffer, int Length, int Peek)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkRecvUDP(NetUDPHandle, RecvIP, RecvPort, Buffer, Length, Peek); }
		//! UDPを使用した通信でデータを受信する、Peek に TRUE を渡すと受信に成功してもデータを受信キューから削除しません( 戻り値  0以上:受信したデータのサイズ  -1:エラー  -2:バッファのサイズが足りない  -3:受信データがない )( IPv6版 )
		inline int NetWorkRecvUDP_IPv6(int NetUDPHandle, IPDATA_IPv6 *RecvIP, int *RecvPort, void *Buffer, int Length, int Peek)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::NetWorkRecvUDP_IPv6(NetUDPHandle, RecvIP, RecvPort, Buffer, Length, Peek); }
		//! UDPを使用した通信で新たな受信データが存在するかどうかを調べる( 戻り値  -1:エラー  TRUE:受信データあり  FALSE:受信データなし )
		inline int CheckNetWorkRecvUDP(int NetUDPHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::CheckNetWorkRecvUDP(NetUDPHandle); }
#endif
	}


	//----------文字入力関連----------//

	//!文字入力関連
	namespace InputString
	{
#ifndef DX_NON_INPUTSTRING
		// 文字コードバッファ操作関係

		//! 文字コードバッファに文字コードをストックする
		inline int StockInputChar(TCHAR CharCode )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::StockInputChar(CharCode); }
		//! 文字コードバッファをクリアする
		inline int ClearInputCharBuf(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ClearInputCharBuf(); }
		//! 文字コードバッファに溜まったデータから文字コードを一つ取得する
		inline TCHAR GetInputChar(int DeleteFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetInputChar( DeleteFlag ); }
		//! 文字コードバッファに溜まったデータから文字コードを一つ取得する、バッファになにも文字コードがない場合は文字コードがバッファに一文字分溜まるまで待つ
		inline TCHAR GetInputCharWait(int DeleteFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetInputCharWait( DeleteFlag ); }

		//! 文字コードバッファに溜まったデータから１文字分取得する
		inline int GetOneChar(TCHAR *CharBuffer, int DeleteFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetOneChar(CharBuffer, DeleteFlag ); }
		//! 文字コードバッファに溜まったデータから１文字分取得する、バッファに何も文字コードがない場合は文字コードがバッファに一文字分溜まるまで待つ
		inline int GetOneCharWait(TCHAR *CharBuffer, int DeleteFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetOneCharWait(CharBuffer, DeleteFlag ); }
		//! 指定の文字コードがアスキーコントロールコードか調べる
		inline int GetCtrlCodeCmp(TCHAR Char )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetCtrlCodeCmp( Char ); }
#endif // DX_NON_INPUTSTRING

#ifndef DX_NON_KEYEX
		//! 画面上に入力中の文字列を描画する
		inline int DrawIMEInputString(int x, int y, int SelectStringNum )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DrawIMEInputString(x, y, SelectStringNum ); }
		//! ＩＭＥを使用するかどうかを設定する
		inline int SetUseIMEFlag(int UseFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseIMEFlag( UseFlag ); }
		//! ＩＭＥで入力できる最大文字数を MakeKeyInput の設定に合わせるかどうかをセットする( TRUE:あわせる  FALSE:あわせない(デフォルト) )
		inline int SetInputStringMaxLengthIMESync(int Flag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetInputStringMaxLengthIMESync( Flag ); }
		//! ＩＭＥで一度に入力できる最大文字数を設定する( 0:制限なし  1以上:指定の文字数で制限 )
		inline int SetIMEInputStringMaxLength(int Length )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetIMEInputStringMaxLength( Length ); }
#endif // DX_NON_KEYEX

		//! 全角文字、半角文字入り乱れる中から指定の文字数での半角文字数を得る
		inline int GetStringPoint(const TCHAR *String, int Point )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetStringPoint( String, Point ); }
		//! 全角文字、半角文字入り乱れる中から指定の半角文字数での文字数を得る
		inline int GetStringPoint2(const TCHAR *String, int Point )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetStringPoint2( String, Point ); }
		//! 全角文字、半角文字入り乱れる中から文字数を取得する
		inline int GetStringLength(const TCHAR *String )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetStringLength( String ); }

#ifndef DX_NON_FONT
		//! 描画可能領域に収まるように改行しながら文字列を描画
		inline int DrawObtainsString(int x, int y, int AddY, const TCHAR *String, unsigned int StrColor, unsigned int StrEdgeColor = 0 , int FontHandle = -1 , unsigned int SelectBackColor = 0xffffffff , unsigned int SelectStrColor = 0 , unsigned int SelectStrEdgeColor = 0xffffffff , int SelectStart = -1 , int SelectEnd = -1 )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DrawObtainsString(x, y, AddY, String, StrColor, StrEdgeColor, FontHandle , SelectBackColor, SelectStrColor, SelectStrEdgeColor, SelectStart, SelectEnd); }
		//! 描画可能領域に収まるように改行しながら文字列を描画( クリップが文字単位 )
		inline int DrawObtainsString_CharClip(int x, int y, int AddY, const TCHAR *String, unsigned int StrColor, unsigned int StrEdgeColor = 0 , int FontHandle = -1 , unsigned int SelectBackColor = 0xffffffff , unsigned int SelectStrColor = 0 , unsigned int SelectStrEdgeColor = 0xffffffff , int SelectStart = -1 , int SelectEnd = -1 )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DrawObtainsString_CharClip(x, y, AddY, String, StrColor, StrEdgeColor, FontHandle, SelectBackColor, SelectStrColor, SelectStrEdgeColor, SelectStart, SelectEnd); }
#endif // DX_NON_FONT
		//! 描画可能領域に収まるように補正を加えながら矩形を描画
		inline int DrawObtainsBox(int x1, int y1, int x2, int y2, int AddY, unsigned int Color, int FillFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DrawObtainsBox(x1, y1, x2, y2, AddY, Color, FillFlag); }

#ifndef DX_NON_KEYEX

		//! 文字列の入力取得
		inline int InputStringToCustom(int x, int y, int BufLength, TCHAR *StrBuffer, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag, int DoubleCharOnlyFlag = FALSE )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::InputStringToCustom(x, y, BufLength, StrBuffer, CancelValidFlag, SingleCharOnlyFlag, NumCharOnlyFlag, DoubleCharOnlyFlag); }

		//! 文字列の入力取得
		inline int KeyInputString(int x, int y, int CharMaxLength, TCHAR *StrBuffer, int CancelValidFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::KeyInputString(x, y, CharMaxLength, StrBuffer, CancelValidFlag ); }
		//! 半角文字列のみの入力取得
		inline int KeyInputSingleCharString(int x, int y, int CharMaxLength, TCHAR *StrBuffer, int CancelValidFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::KeyInputSingleCharString(x, y, CharMaxLength, StrBuffer, CancelValidFlag ); }
		//! 数値の入力取得
		inline int KeyInputNumber(int x, int y, int MaxNum, int MinNum, int CancelValidFlag )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::KeyInputNumber(x, y, MaxNum, MinNum, CancelValidFlag); }

		//! IMEの入力モード文字列を取得する
		inline int GetIMEInputModeStr(TCHAR *GetBuffer )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetIMEInputModeStr(GetBuffer); }
		//! IMEで入力中の文字列の情報を取得する
		inline const IMEINPUTDATA* GetIMEInputData(	void )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetIMEInputData(); }
#if defined( DX_GCC_COMPILE ) || defined( __ANDROID )
		//! ( SetKeyInputStringColor2 の旧関数 )InputString関数使用時の文字の各色を変更する
		inline int SetKeyInputStringColor(ULONGLONG NmlStr, ULONGLONG NmlCur, ULONGLONG IMEStrBack, ULONGLONG IMECur, ULONGLONG IMELine, ULONGLONG IMESelectStr, ULONGLONG IMEModeStr , ULONGLONG NmlStrE = 0 , ULONGLONG IMESelectStrE = 0 , ULONGLONG IMEModeStrE = 0 , ULONGLONG IMESelectWinE = 0xffffffffffffffffULL ,	ULONGLONG IMESelectWinF = 0xffffffffffffffffULL , ULONGLONG SelectStrBackColor = 0xffffffffffffffffULL , ULONGLONG SelectStrColor = 0xffffffffffffffffULL , ULONGLONG SelectStrEdgeColor = 0xffffffffffffffffULL, ULONGLONG IMEStr = 0xffffffffffffffffULL, ULONGLONG IMEStrE = 0xffffffffffffffffULL  )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputStringColor(NmlStr, NmlCur, IMEStrBack, IMECur, IMELine, IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE, IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor, IMEStr, IMEStrE); }
#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID )
		//! ( SetKeyInputStringColor2 の旧関数 )InputString関数使用時の文字の各色を変更する 
		inline int SetKeyInputStringColor(ULONGLONG NmlStr, ULONGLONG NmlCur, ULONGLONG IMEStrBack, ULONGLONG IMECur, ULONGLONG IMELine, ULONGLONG IMESelectStr, ULONGLONG IMEModeStr , ULONGLONG NmlStrE = 0 , ULONGLONG IMESelectStrE = 0 , ULONGLONG IMEModeStrE = 0 , ULONGLONG IMESelectWinE = 0xffffffffffffffff ,	ULONGLONG IMESelectWinF = 0xffffffffffffffff , ULONGLONG SelectStrBackColor = 0xffffffffffffffff , ULONGLONG SelectStrColor = 0xffffffffffffffff , ULONGLONG SelectStrEdgeColor = 0xffffffffffffffff, ULONGLONG IMEStr = 0xffffffffffffffff, ULONGLONG IMEStrE = 0xffffffffffffffff )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputStringColor(NmlStr, NmlCur, IMEStrBack, IMECur, IMELine, IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE, IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor, IMEStr, IMEStrE); }
#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID )
		//! InputString関数使用時の文字の各色を変更する
		//!@param[in] TargetColor DX_KEYINPSTRCOLOR_NORMAL_STR 等
		inline int SetKeyInputStringColor2(int TargetColor, unsigned int Color )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputStringColor2(TargetColor, Color ); }
		//! SetKeyInputStringColor2 で設定した色をデフォルトに戻す
		//!@param[in] TargetColor DX_KEYINPSTRCOLOR_NORMAL_STR 等
		inline int ResetKeyInputStringColor2(int TargetColor )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ResetKeyInputStringColor2( TargetColor ); }
		//! キー入力文字列描画関連で使用するフォントのハンドルを変更する(-1でデフォルトのフォントハンドル)
		inline int SetKeyInputStringFont(int FontHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputStringFont( FontHandle ); }
		//! キー入力文字列処理の入力文字数が限界に達している状態で、文字列の末端部分で入力が行われた場合の処理モードを変更する
		//!@param[in] EndCharaMode DX_KEYINPSTR_ENDCHARAMODE_OVERWRITE 等
		inline int SetKeyInputStringEndCharaMode(int EndCharaMode )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputStringEndCharaMode( EndCharaMode ); }
		//! 入力モード文字列を描画する
		inline int DrawKeyInputModeString(int x, int y )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DrawKeyInputModeString(x, y); }

		//! キー入力データ初期化
		inline int InitKeyInput(void)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::InitKeyInput(); }
		//! 新しいキー入力ハンドルの作成
		inline int MakeKeyInput(int MaxStrLength, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag, int DoubleCharOnlyFlag = FALSE )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::MakeKeyInput(MaxStrLength, CancelValidFlag, SingleCharOnlyFlag, NumCharOnlyFlag, DoubleCharOnlyFlag); }
		//! キー入力ハンドルの削除
		inline int DeleteKeyInput(int InputHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DeleteKeyInput(InputHandle ); }
		//! 指定のキー入力ハンドルをアクティブにする( -1 を指定するとアクティブなキー入力ハンドルが無い状態になります )
		inline int SetActiveKeyInput(int InputHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetActiveKeyInput(InputHandle ); }
		//! 現在アクティブになっているキー入力ハンドルを取得する
		inline int GetActiveKeyInput(void )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetActiveKeyInput(); }
		//! キー入力ハンドルの入力が終了しているか取得する
		inline int CheckKeyInput(int InputHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::CheckKeyInput(InputHandle ); }
		//! 入力が完了したキー入力ハンドルを再度編集状態に戻す
		inline int ReStartKeyInput(int InputHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReStartKeyInput(InputHandle ); }
		//! キー入力ハンドル処理関数
		inline int ProcessActKeyInput(void )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ProcessActKeyInput(); }
		//! キー入力ハンドルの入力中情報の描画
		inline int DrawKeyInputString(int x, int y, int InputHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DrawKeyInputString(x, y, InputHandle); }

		//! キー入力ハンドルの指定の領域を選択状態にする( SelectStart と SelectEnd に -1 を指定すると選択状態が解除されます )
		inline int SetKeyInputSelectArea(int  SelectStart, int  SelectEnd, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputSelectArea(SelectStart, SelectEnd, InputHandle); }
		//! キー入力ハンドルの選択領域を取得する
		inline int GetKeyInputSelectArea(int *SelectStart, int *SelectEnd, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetKeyInputSelectArea(SelectStart, SelectEnd, InputHandle); }
		//! キー入力ハンドルの描画開始文字位置を設定する
		inline int SetKeyInputDrawStartPos(int DrawStartPos, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputDrawStartPos(DrawStartPos, InputHandle); }
		//! キー入力ハンドルの描画開始文字位置を取得する
		inline int GetKeyInputDrawStartPos(int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetKeyInputDrawStartPos(InputHandle); }
		//! キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
		inline int SetKeyInputCursorBrinkTime(int Time)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputCursorBrinkTime(Time); }
		//! キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
		inline int SetKeyInputCursorBrinkFlag(int Flag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputCursorBrinkFlag(Flag); }
		//! キー入力ハンドルに指定の文字列をセットする
		inline int SetKeyInputString(const TCHAR *String, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputString(String, InputHandle); }
		//! キー入力ハンドルに指定の数値を文字に置き換えてセットする
		inline int SetKeyInputNumber(int Number, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputNumber(Number, InputHandle); }
		//! キー入力ハンドルに指定の浮動小数点値を文字に置き換えてセットする
		inline int SetKeyInputNumberToFloat(float Number, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputNumberToFloat(Number, InputHandle); }
		//! キー入力ハンドルの入力中の文字列を取得する
		inline int GetKeyInputString(TCHAR *StrBuffer, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetKeyInputString(StrBuffer, InputHandle); }
		//! キー入力ハンドルの入力中の文字列を整数値として取得する
		inline int GetKeyInputNumber(int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetKeyInputNumber(InputHandle); }
		//! キー入力ハンドルの入力中の文字列を浮動小数点値として取得する
		inline float GetKeyInputNumberToFloat(int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetKeyInputNumberToFloat(InputHandle); }
		//! キー入力ハンドルの現在のカーソル位置を設定する
		inline int SetKeyInputCursorPosition(int Position, int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetKeyInputCursorPosition(Position, InputHandle); }
		//! キー入力ハンドルの現在のカーソル位置を取得する
		inline int GetKeyInputCursorPosition(int InputHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetKeyInputCursorPosition(InputHandle); }

#endif // DX_NON_KEYEX
	}


	//----------ファイル操作----------//

	//!ファイル操作
	namespace DxFile
	{
		// ファイルアクセス関数

		//! ファイルを開く
		inline int FileRead_open(const TCHAR *FilePath, int ASync = FALSE)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_open(FilePath, ASync); }
		//! ファイルのサイズを取得する
		inline LONGLONG FileRead_size(const TCHAR *FilePath)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_size(FilePath); }
		//! ファイルを閉じる
		inline int FileRead_close(int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_close(FileHandle); }
		//! ファイルポインタの読み込み位置を取得する
		inline LONGLONG FileRead_tell(int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_tell(FileHandle); }
		//! ファイルポインタの読み込み位置を変更する
		inline int FileRead_seek(int FileHandle, LONGLONG Offset, int Origin )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_seek(FileHandle , Offset , Origin); }
		//! ファイルからデータを読み込む
		inline int FileRead_read(void *Buffer, int ReadSize, int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_read(Buffer, ReadSize, FileHandle ); }
		//! ファイル読み込みが完了しているかどうかを取得する
		inline int FileRead_idle_chk(int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_idle_chk(FileHandle); }
		//! ファイルの読み込み位置が終端に達しているかどうかを取得する
		inline int FileRead_eof(int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_eof(FileHandle); }
		//! ファイルから文字列を読み出す
		inline int FileRead_gets(TCHAR *Buffer , int BufferSize , int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_gets(Buffer, BufferSize, FileHandle ); }
		//! ファイルから一文字読み出す
		inline TCHAR FileRead_getc(int FileHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_getc( FileHandle ); }
		//! ファイルから書式化されたデータを読み出す
		template<typename... Args> inline int FileRead_scanf(int FileHandle, const TCHAR *Format, Args&&... args)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_scanf(FileHandle, Format, std::forward<Args>(args)...); }

#if 0
		static_assert(false, "ここまで");
		//! ファイル情報ハンドルを作成する( 戻り値  -1:エラー  -1以外:ファイル情報ハンドル )
		inline DWORD_PTR FileRead_createInfo(		const TCHAR *ObjectPath )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_createInfo(		const TCHAR *ObjectPath ); }
		//! ファイル情報ハンドル中のファイルの数を取得する
		inline int FileRead_getInfoNum(		DWORD_PTR FileInfoHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_getInfoNum(		DWORD_PTR FileInfoHandle ); }
		//! ファイル情報ハンドル中のファイルの情報を取得する
		inline int FileRead_getInfo(			int Index , FILEINFO *Buffer , DWORD_PTR FileInfoHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_getInfo(			int Index , FILEINFO *Buffer , DWORD_PTR FileInfoHandle ); }
		//! ファイル情報ハンドルを削除する
		inline int FileRead_deleteInfo(		DWORD_PTR FileInfoHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_deleteInfo(		DWORD_PTR FileInfoHandle ); }

		//! 指定のファイル又はフォルダの情報を取得し、ファイル検索ハンドルも作成する( 戻り値: -1=エラー  -1以外=ファイル検索ハンドル )
		inline DWORD_PTR FileRead_findFirst(			const TCHAR *FilePath, FILEINFO *Buffer )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_findFirst(			const TCHAR *FilePath, FILEINFO *Buffer ); }
		//! 条件の合致する次のファイルの情報を取得する( 戻り値: -1=エラー  0=成功 )
		inline int FileRead_findNext(			DWORD_PTR FindHandle, FILEINFO *Buffer )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_findNext(			DWORD_PTR FindHandle, FILEINFO *Buffer ); }
		//! ファイル検索ハンドルを閉じる( 戻り値: -1=エラー  0=成功 )
		inline int FileRead_findClose(			DWORD_PTR FindHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_findClose(			DWORD_PTR FindHandle ); }

		//! 指定のファイルの内容を全てメモリに読み込み、その情報のアクセスに必要なハンドルを返す( 戻り値  -1:エラー  -1以外:ハンドル )、使い終わったらハンドルは FileRead_fullyLoad_delete で削除する必要があります
		inline int FileRead_fullyLoad(			const TCHAR *FilePath )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_fullyLoad(			const TCHAR *FilePath ); }
		//! FileRead_fullyLoad で読み込んだファイルのハンドルを削除する
		inline int FileRead_fullyLoad_delete(	int FLoadHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_fullyLoad_delete(	int FLoadHandle ); }
		//! FileRead_fullyLoad で読み込んだファイルの内容を格納したメモリアドレスを取得する
		inline const void* FileRead_fullyLoad_getImage( int FLoadHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_fullyLoad_getImage( int FLoadHandle ); }
		//! FileRead_fullyLoad で読み込んだファイルのサイズを取得する
		inline LONGLONG FileRead_fullyLoad_getSize(	int FLoadHandle )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::FileRead_fullyLoad_getSize(	int FLoadHandle ); }

		// 設定関係関数

		//! ＤＸライブラリでストリームデータアクセスに使用する関数がデフォルトのものか調べる( TRUE:デフォルトのもの  FALSE:デフォルトではない )
		inline int GetStreamFunctionDefault(	void )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetStreamFunctionDefault(	void ); }
		//! ＤＸライブラリでストリームデータアクセスに使用する関数を変更する
		inline int ChangeStreamFunction(		const STREAMDATASHREDTYPE2  *StreamThread  )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ChangeStreamFunction(		const STREAMDATASHREDTYPE2  *StreamThread  ); }
		//! ＤＸライブラリでストリームデータアクセスに使用する関数を変更する( wchar_t 使用版 )
		inline int ChangeStreamFunctionW(		const STREAMDATASHREDTYPE2W *StreamThreadW )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ChangeStreamFunctionW(		const STREAMDATASHREDTYPE2W *StreamThreadW ); }

		// 補助関係関数

		//! フルパスではないパス文字列をフルパスに変換する( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )( CurrentDir が NULL の場合は現在のカレントディレクトリを使用する )
		inline int ConvertFullPath( const TCHAR *Src, TCHAR *Dest, const TCHAR *CurrentDir = NULL )DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ConvertFullPath( const TCHAR *Src, TCHAR *Dest, const TCHAR *CurrentDir = NULL ); }
#endif
	}


	//----------サウンド関係----------//

	//!サウンド関係
	namespace Sound
	{
		//オーバーヘッドが少ないVer
		//ただしコピーができない
		class SoundEffect_Unique : public impl::Unique_Handle_Bace<SoundEffect_Unique>
		{
		public:
			//!サウンドハンドルを削除する
			inline void Delete(bool LogOutFlag = false) {
				DeleteSoundMem(GetHandle(), LogOutFlag);
				//handle = -1;
			}
			//!このオブジェクトを無効化し、今まで使っていたハンドルを返す
			//!自動削除も無効化される
			inline int release(){ return ReleaseRun(); }

			//----------static関数----------//

			static SoundEffect_Unique LoadSoundMem(const TCHAR *FileName, int BufferNum = 3, int UnionHandle = -1) {
				return DxLib::LoadSoundMem(FileName, BufferNum, UnionHandle);
			}

			static int PlaySoundMem(const SoundEffect_Unique& SoundHandle, int PlayType, int TopPositionFlag = TRUE) {
				return DxLib::PlaySoundMem(SoundHandle.GetHandle(), PlayType, TopPositionFlag);
			}

		private:
			typedef Unique_Handle_Bace<SoundEffect_Unique> Parent_T;
		public:
			SoundEffect_Unique()
				: Unique_Handle_Bace()
			{}

			//所有権の譲渡
			SoundEffect_Unique(SoundEffect_Unique&& other)
				: Unique_Handle_Bace(std::move(other))
			{}

			//所有権の譲渡
			SoundEffect_Unique& operator=(SoundEffect_Unique&& other)
			{
				Parent_T::operator=(std::move(other));
				return *this;
			}

		private:
			//間違えて他の種類のハンドルを持たないようにprivateにしておく
			SoundEffect_Unique(int param_handle)
				: Unique_Handle_Bace(param_handle)
			{}
		};
		class SoundEffect
		{
		public:
			void Delete(bool LogOutFlag = false) {
				p_handle->Delete(LogOutFlag);
			}
			//!このオブジェクトを無効化する
			//!最後の一個だった場合ハンドルは自動的に削除される
			void release_this() {
				p_handle.reset();
			}
			//!このオブジェクトおよび同じ派生元のオブジェクトを無効化し、今まで使っていたハンドルを返す
			//!自動削除も無効化される
			int release_all() {
				return p_handle->release();
			}

			//----------static関数----------//

			static SoundEffect LoadSoundMem(const TCHAR *FileName, int BufferNum = 3, int UnionHandle = -1) {
				return SoundEffect_Unique::LoadSoundMem(FileName, BufferNum, UnionHandle);
			}

			static int PlaySoundMem(const SoundEffect& SoundHandle, int PlayType, int TopPositionFlag = TRUE) {
				if (SoundHandle.p_handle == nullptr){ return -1; }
				return SoundEffect_Unique::PlaySoundMem(*SoundHandle.p_handle, PlayType, TopPositionFlag);
			}

			SoundEffect(SoundEffect_Unique&& handle)
				: p_handle(std::make_shared<SoundEffect_Unique>(std::move(handle)))
			{}
			SoundEffect(){}
		private:
			std::shared_ptr<SoundEffect_Unique> p_handle;
		};
	}
	namespace DxLibEx_Classes {
		using Sound::SoundEffect;
		using Sound::SoundEffect_Unique;
	}


}

//----------2Dグラフィック----------//
#include "dxlibex/Graph2D.h"

//関数の定義
#include "dxlibex/impl.hpp"
#endif//#if 0 (old version)


#endif
