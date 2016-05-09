/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_SOUND_HPP_
#define DXLE_INC_SOUND_HPP_

#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <chrono>
#include "dxlibex/Helper.h"
#include "dxlibex/config/defines.h"
#include "dxlibex/thread.hpp"
#include "dxlibex/type_traits.hpp"
#include "dxlibex/basic_types.hpp"
#include "dxlibex/exception.hpp"
#include "dxlibex/utility/inferior_string_ref.hpp"
#include <filesystem>//in gcc(branch 5 or later), -lstdc++fs is required.
#ifdef small
#undef small//from rpcndr.h
#endif
//----------サウンド----------//

namespace dxle
{

	//! サウンド
	namespace sound
	{
		namespace ch = std::chrono;
		enum class reverb : int {
			default_airspace = DX_REVERB_PRESET_DEFAULT,// デフォルト
			generic          = DX_REVERB_PRESET_GENERIC,// 一般的な空間
			padded_cell      = DX_REVERB_PRESET_PADDEDCELL,// 精神病患者室(？)
			room             = DX_REVERB_PRESET_ROOM,// 部屋
			bathroom         = DX_REVERB_PRESET_BATHROOM,// バスルーム
			living_room      = DX_REVERB_PRESET_LIVINGROOM,// リビングルーム
			stone_room       = DX_REVERB_PRESET_STONEROOM,// 石の部屋
			auditorium       = DX_REVERB_PRESET_AUDITORIUM,// 講堂
			consert_hall     = DX_REVERB_PRESET_CONCERTHALL,// コンサートホール
			cave             = DX_REVERB_PRESET_CAVE,// 洞穴
			arena            = DX_REVERB_PRESET_ARENA,// 舞台
			hangar           = DX_REVERB_PRESET_HANGAR,// 格納庫
			carpeted_hallway = DX_REVERB_PRESET_CARPETEDHALLWAY,// カーペットが敷かれた玄関
			hallway          = DX_REVERB_PRESET_HALLWAY,// 玄関
			stone_corridor   = DX_REVERB_PRESET_STONECORRIDOR,// 石の廊下
			alley            = DX_REVERB_PRESET_ALLEY,// 裏通り
			forest           = DX_REVERB_PRESET_FOREST,// 森
			city             = DX_REVERB_PRESET_CITY,// 都市
			mountains        = DX_REVERB_PRESET_MOUNTAINS,// 山
			quarry           = DX_REVERB_PRESET_QUARRY,// 採石場
			plain            = DX_REVERB_PRESET_PLAIN,// 平原
			parking_lot      = DX_REVERB_PRESET_PARKINGLOT,// 駐車場
			sewer_pipe       = DX_REVERB_PRESET_SEWERPIPE,// 下水管
			under_water      = DX_REVERB_PRESET_UNDERWATER,// 水面下
			small_room       = DX_REVERB_PRESET_SMALLROOM,// 小部屋
			medium_room      = DX_REVERB_PRESET_MEDIUMROOM,// 中部屋
			large_room       = DX_REVERB_PRESET_LARGEROOM,// 大部屋
			megium_hall      = DX_REVERB_PRESET_MEDIUMHALL,// 中ホール
			large_hall       = DX_REVERB_PRESET_LARGEHALL,// 大ホール
			plate            = DX_REVERB_PRESET_PLATE// 板
		};
		enum class playtype : uint8_t {
			normal = DX_PLAYTYPE_NORMAL,
			back   = DX_PLAYTYPE_BACK,
			loop   = DX_PLAYTYPE_LOOP
		};
		enum class sound_data_type : uint8_t {
			small,     //DxLib::LoadSoundMem
			big,       //DxLib::LoadBGM
			auto_detect//call detail::detect_sound_data_type()
		};
		namespace detail {
			sound_data_type detect_sound_data_type(sound_data_type datatype, const TCHAR *FileName) {
				if (sound_data_type::auto_detect != datatype) return datatype;
				namespace fs = std::experimental::filesystem;//まだ標準には入っていない。boost::filesystemでも良かったけどmingw-clang以外では利用できそうだったので。
				try {
					fs::path path(FileName);
					const auto size = fs::file_size(path);
					DXLE_CONSTEXPR_OR_CONST std::uintmax_t borderline = 900000;//byte
					return (size < borderline) ? sound_data_type::small : sound_data_type::big;
				}
				catch(const std::exception&){
					return sound_data_type::big;
				}
			}
		}
		class sound final : public impl::Unique_HandledObject_Bace < sound >
		{
		public:
			//constant

			DXLE_STATIC_CONSTEXPR uint8_t SOUNDBUFFER_MAX_CHANNEL_NUM = 8;//DxSound.hに定義あり
			//ctor-like function
			//sound_data_type指定なしはsound_data_type::small扱い。

			void open(tinferior_string_ref FileName, int BufferNum, int UnionHandle, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW { 
				if (this->is_vaid()) this->delete_this();
				this->set_handle(DxLib::LoadSoundMem(FileName.c_str(), BufferNum, UnionHandle)); 
			}
			void open(tinferior_string_ref FileName, int BufferNum, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW { 
				if (this->is_vaid()) this->delete_this();
				this->set_handle(DxLib::LoadSoundMem(FileName.c_str(), BufferNum));
			}
			void open(tinferior_string_ref FileName, sound_data_type type, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW {
				if (this->is_vaid()) this->delete_this();
				this->set_handle(
					(sound_data_type::small == detail::detect_sound_data_type(type, FileName.c_str()))
					? DxLib::LoadSoundMem(FileName.c_str())
					: DxLib::LoadBGM(FileName.c_str())
				);
			}
			void open(tinferior_string_ref FileName, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW {
				if (this->is_vaid()) this->delete_this();
				this->set_handle(DxLib::LoadSoundMem(FileName.c_str()));
			}
			void open(tinferior_string_ref FileName, int BufferNum = 3, int UnionHandle = -1) {
				if (this->is_vaid()) this->delete_this();
				const auto re = DxLib::LoadSoundMem(FileName.c_str(), BufferNum, UnionHandle);
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 != re), "fail DxLib::LoadSoundMem().");
				this->set_handle(re);
			}
			void open(tinferior_string_ref FileName, sound_data_type type) {
				if (this->is_vaid()) this->delete_this();
				const auto re = (sound_data_type::small == (type = detail::detect_sound_data_type(type, FileName.c_str())))
					? DxLib::LoadSoundMem(FileName.c_str())
					: DxLib::LoadBGM(FileName.c_str());
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 != re), (sound_data_type::small == type) ? "fail DxLib::LoadSoundMem()." : "fail DxLib::LoadBGM().");
				this->set_handle(re);
			}
			//ctor
			sound()DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace() {}
			sound(const sound& other) = delete;
			sound(sound&& other)DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(std::move(other)){}

			sound& operator=(const sound& other) = delete;
			sound& operator=(sound&& other) DXLE_NOEXCEPT_OR_NOTHROW{ Unique_HandledObject_Bace<sound>::operator=(std::move(other)); return *this; }

			//!\~japanese サウンドを削除する
			//!\~english  Delete this sound data
			inline void delete_this(bool LogOutFlag = false) { DxLib::DeleteSoundMem(GetHandle(), LogOutFlag); }

			//! 同じサウンドデータを使用するサウンドハンドルを作成する( DX_SOUNDDATATYPE_MEMNOPRESS タイプのサウンドハンドルのみ可能 )
			sound DuplicateSoundMem(int BufferNum = 3)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::DuplicateSoundMem(GetHandle(), BufferNum); }


			//生成
			static sound			LoadSoundMem2(const TCHAR *FileName1, const TCHAR *FileName2);											// 前奏部とループ部に分かれたサウンドファイルを読み込みサウンドハンドルを作成する
			//static sound			LoadBGM(const TCHAR *FileName);// 主にＢＧＭを読み込みサウンドハンドルを作成するのに適した関数//不要。load_soundにsound_data_type::bigを渡すべし

			static sound load_sound(tinferior_string_ref FileName, int BufferNum, int UnionHandle, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW { return{ DxLib::LoadSoundMem(FileName.c_str(), BufferNum, UnionHandle) }; }
			static sound load_sound(tinferior_string_ref FileName, int BufferNum, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW { return{ DxLib::LoadSoundMem(FileName.c_str(), BufferNum) }; }
			static sound load_sound(tinferior_string_ref FileName, sound_data_type type, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW {
				return (sound_data_type::small == detail::detect_sound_data_type(type, FileName.c_str()))
					? DxLib::LoadSoundMem(FileName.c_str())
					: DxLib::LoadBGM(FileName.c_str());
			}
			static sound load_sound(tinferior_string_ref FileName, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW { return{ DxLib::LoadSoundMem(FileName.c_str()) }; }
			static sound load_sound(tinferior_string_ref FileName, int BufferNum = 3, int UnionHandle = -1) {
				const auto re = DxLib::LoadSoundMem(FileName.c_str(), BufferNum, UnionHandle);
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 != re), "fail DxLib::LoadSoundMem().");
				return re;
			}
			static sound load_sound(tinferior_string_ref FileName, sound_data_type type) {
				const auto re = (sound_data_type::small == (type = detail::detect_sound_data_type(type, FileName.c_str())))
					? DxLib::LoadSoundMem(FileName.c_str())
					: DxLib::LoadBGM(FileName.c_str());
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 != re), (sound_data_type::small == type) ? "fail DxLib::LoadSoundMem()." : "fail DxLib::LoadBGM().");
				return re;
			}

			static sound			LoadSoundMemByMemImageBase(const void *FileImage, int FileImageSize, int BufferNum, int UnionHandle = -1);			// メモリ上に展開されたサウンドファイルイメージからサウンドハンドルを作成する
			static sound			LoadSoundMemByMemImage(const void *FileImage, int FileImageSize, int UnionHandle = -1);			// メモリ上に展開されたサウンドファイルイメージからサウンドハンドルを作成する( バッファ数指定無し )
			static sound			LoadSoundMemByMemImage2(const void *WaveImage, int WaveImageSize, const WAVEFORMATEX *WaveFormat, int WaveHeaderSize);	// メモリ上に展開されたＰＣＭデータからサウンドハンドルを作成する
			//extern	int			LoadSoundMem2ByMemImage(const void *FileImage1, int FileImageSize1, const void *FileImage2, int FileImageSize2);	// 前奏部とループ部に分かれた二つのメモリ上に展開されたサウンドファイルイメージからサウンドハンドルを作成する
			static sound			LoadSoundMemFromSoftSound(int SoftSoundHandle, int BufferNum = 3);													// ソフトウエアサウンドハンドルが持つサウンドデータからサウンドハンドルを作成する

			//削除

			//!\~japanese サウンドを削除する
			//!\~english  Delete this sound data
			int	DeleteSoundMem(int LogOutFlag = false) DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::DeleteSoundMem(GetHandle(), LogOutFlag); }

			bool is_open() { return this->is_vaid(); }
			//操作

			//!\~japanese サウンドを再生する
			//!\~english  Play sound.
			int play(playtype PlayType, bool TopPositionFlag, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW {
				return DxLib::PlaySoundMem(GetHandle(), static_cast<int>(PlayType), TopPositionFlag);
			}
			//!\~japanese サウンドを再生する
			//!\~english  Play sound.
			int play(playtype PlayType, std::nothrow_t) DXLE_NOEXCEPT_OR_NOTHROW {
				return DxLib::PlaySoundMem(GetHandle(), static_cast<int>(PlayType), true);
			}
			//!\~japanese サウンドを再生する
			//!\~english  Play sound.
			void play(playtype PlayType, bool TopPositionFlag) {
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->play(PlayType, TopPositionFlag, std::nothrow)), "fail DxLib::PlaySoundMem().");
			}
			//!\~japanese サウンドを再生する
			//!\~english  Play sound.
			void play(playtype PlayType) {
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->play(PlayType, std::nothrow)), "fail DxLib::PlaySoundMem().");
			}
			//!\~japanese サウンドの再生を停止する
			//!\~english  Stop playing sound.
			int stop(std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				return DxLib::StopSoundMem(this->GetHandle());
			}
			//!\~japanese サウンドの再生を停止する
			//!\~english  Stop playing sound.
			void stop() {
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->stop(std::nothrow)), "fail DxLib::StopSoundMem().");
			}
			//!\~japanese サウンドが再生中かどうかを取得する
			//!\~english  check whether this sound is playing.
			int is_playing(std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				return DxLib::CheckSoundMem(this->GetHandle());
			}
			//!\~japanese サウンドが再生中かどうかを取得する
			//!\~english  check whether this sound is playing.
			bool is_playing() {
				const auto re = this->is_playing(std::nothrow);
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == re), "fail DxLib::CheckSoundMem().");
				return 0 != re;
			}
			//!\~japanese サウンドのパンを設定する
			//!\~english  Set the pan of this sound.
			template<typename T, typename Period>int set_pan(bel_c<T, Period> PanPal, std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				const auto pan = bel_cast<myrio_bel>(PanPal).get();
				assert(0 <= pan && pan <= 10000);
				return DxLib::SetPanSoundMem(bel_cast<myrio_bel>(PanPal).get(), this->GetHandle());
			}
			//!\~japanese サウンドのパンを設定する
			//!\~english  Set the pan of this sound.
			template<typename T, typename Period>void set_pan(bel_c<T, Period> PanPal) {
				DXLE_INVAID_ARGUMENT_THROW_WITH_MESSAGE_IF((PanPal < 0_myrioB || 10000_myrioB < PanPal), "");
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->set_pan(PanPal, std::nothrow)), "fail DxLib::SetPanSoundMem().");
			}
			//!\~japanese サウンドのパンを設定する( -255 ～ 255 )
			//!\~english  Set the pan of this sound.( -255 to 255 )
			int change_pan(int16_t PanPal, std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				assert(-255 <= PanPal && PanPal <= 255);
				return DxLib::ChangePanSoundMem(PanPal, this->GetHandle());
			}
			//!\~japanese サウンドのパンを設定する( -255 ～ 255 )
			//!\~english  Set the pan of this sound.( -255 to 255 )
			void change_pan(int16_t PanPal) {
				DXLE_INVAID_ARGUMENT_THROW_WITH_MESSAGE_IF((PanPal < -255 || 255 < PanPal), "");
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->change_pan(PanPal, std::nothrow)), "fail DxLib::ChangePanSoundMem().");
			}
			//!\~japanese サウンドのパンを取得する
			//!\~english  Get the pan of this sound.
			myrio_bel get_pan(std::nothrow_t) {
				return myrio_bel(DxLib::GetPanSoundMem(this->GetHandle()));
			}
			//!\~japanese サウンドのパンを取得する
			//!\~english  Get the pan of this sound.
			myrio_bel get_pan() {
				const auto re = this->get_pan(std::nothrow);
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1_myrioB == re), "fail DxLib::GetPanSoundMem().");
				return re;
			}
			//!\~japanese サウンドのボリュームを設定する
			//!\~english  Set sound volume.
			template<typename T, typename Period>int set_volume(bel_c<T, Period> VolumePal, std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				const auto volume = bel_cast<myrio_bel>(VolumePal).get();
				assert(0 <= volume && volume <= 10000);
				return DxLib::SetVolumeSoundMem(volume, this->GetHandle());
			}
			//!\~japanese サウンドのボリュームを設定する
			//!\~english  Set sound volume.
			template<typename T, typename Period>void set_volume(bel_c<T, Period> VolumePal) {
				DXLE_INVAID_ARGUMENT_THROW_WITH_MESSAGE_IF((VolumePal < 0_myrioB || 10000_myrioB < VolumePal), "");
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->set_volume(VolumePal, std::nothrow)), "fail DxLib::SetVolumeSoundMem().");
			}
			//!\~japanese サウンドのボリュームを設定する( 0 ～ 255 )
			//!\~english  Set sound volume( 0 to 255 ).
			int change_volume(uint8_t VolumePal, std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				return DxLib::ChangeVolumeSoundMem(VolumePal, this->GetHandle());
			}
			void change_volume(uint8_t VolumePal) {
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->change_volume(VolumePal, std::nothrow)), "fail DxLib::ChangeVolumeSoundMem().");
			}
			//!\~japanese サウンドのボリュームを取得する
			//!\~english  Get sound volume.
			myrio_bel get_volume(std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				return myrio_bel(DxLib::GetVolumeSoundMem(this->GetHandle()));
			}
			//!\~japanese サウンドのボリュームを取得する
			//!\~english  Get sound volume.
			myrio_bel get_volume() {
				const auto re = this->get_volume(std::nothrow);
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1_myrioB == re), "fail DxLib::GetVolumeSoundMem().");
				return re;
			}
			//!\~japanese サウンドの指定のチャンネルのボリュームを設定する
			//!\~english  Set the volume of specified chanel of sound.
			template<typename T, typename Period>int set_volume(uint8_t Channel, bel_c<T, Period> VolumePal, std::nothrow_t)DXLE_NOEXCEPT_OR_NOTHROW {
				assert(Channel < SOUNDBUFFER_MAX_CHANNEL_NUM);
				const auto volume = bel_cast<myrio_bel>(VolumePal).get();
				assert(0 <= volume && volume <= 10000);
				return DxLib::SetChannelVolumeSoundMem(Channel, volume, this->GetHandle());
			}
			//!\~japanese サウンドの指定のチャンネルのボリュームを設定する
			//!\~english  Set the volume of specified chanel of sound.
			template<typename T, typename Period>int set_volume(uint8_t Channel, bel_c<T, Period> VolumePal) {
				DXLE_INVAID_ARGUMENT_THROW_WITH_MESSAGE_IF((SOUNDBUFFER_MAX_CHANNEL_NUM <= Channel || VolumePal < 0_myrioB || 10000_myrioB < VolumePal), "");
				DXLE_SOUND_ERROR_THROW_WITH_MESSAGE_IF((-1 == this->set_volume(Channel, VolumePal, std::nothrow)), "fail DxLib::SetChannelVolumeSoundMem().");
			}
			int change_volume(int Channel, uint8_t VolumePal);						// サウンドハンドルの指定のチャンネルのボリュームを設定する( 0 ～ 255 )
			int get_volume(int Channel);						// サウンドハンドルの指定のチャンネルのボリュームを取得する
			int SetFrequency(int FrequencyPal);						// サウンドハンドルの再生周波数を設定する
			int GetFrequency();						// サウンドハンドルの再生周波数を取得する
			int ResetFrequency();						// サウンドハンドルの再生周波数を読み込み直後の状態に戻す

			int set_next_play_pan(int PanPal);						// サウンドハンドルの次の再生にのみ使用するパンを設定する( 100分の1デシベル単位 0 ～ 10000 )
			int change_next_play_pan(int16_t PanPal);						// サウンドハンドルの次の再生にのみ使用するパンを設定する( -255 ～ 255 )
			int set_next_play_volume(int VolumePal);						// サウンドハンドルの次の再生にのみ使用するボリュームを設定する( 100分の1デシベル単位 0 ～ 10000 )
			int cange_next_play_volume(uint8_t VolumePal);						// サウンドハンドルの次の再生にのみ使用するボリュームを設定する( 0 ～ 255 )
			int set_next_play_volume(int Channel, int VolumePal);						// サウンドハンドルの次の再生にのみ使用するチャンネルのボリュームを設定する( 100分の1デシベル単位 0 ～ 10000 )
			int cange_next_play_volume(int Channel, uint8_t VolumePal);						// サウンドハンドルの次の再生にのみ使用するチャンネルのボリュームを設定する( 0 ～ 255 )
			int set_next_play_frequency(int FrequencyPal);						// サウンドハンドルの次の再生にのみ使用する再生周波数を設定する

			typedef int sample_n_t;//! for sample num
			int set_current_position(sample_n_t SamplePosition);						// サウンドハンドルの再生位置をサンプル単位で設定する(再生が止まっている時のみ有効)
			sample_n_t get_current_position();						// サウンドハンドルの再生位置をサンプル単位で取得する
			int set_sound_current_position(int Byte);						// サウンドハンドルの再生位置をバイト単位で設定する(再生が止まっている時のみ有効)
			int get_sound_currentPosition();						// サウンドハンドルの再生位置をバイト単位で取得する
			int set_sound_current_time(ch::milliseconds Time);						// サウンドハンドルの再生位置をミリ秒単位で設定する(圧縮形式の場合は正しく設定されない場合がある)
			ch::milliseconds get_sound_current_time();						// サウンドハンドルの再生位置をミリ秒単位で取得する(圧縮形式の場合は正しい値が返ってこない場合がある)
			sample_n_t get_sound_total_sample();						// サウンドハンドルの音の総時間をサンプル単位で取得する
			ch::milliseconds get_sound_total_time();						// サウンドハンドルの音の総時間をミリ秒単位で取得する

			int set_loop_time_pos(ch::milliseconds LoopTime);						// サウンドハンドルにループ位置を設定する(ミリ秒単位)
			int set_loop_sample_pos(sample_n_t LoopSamplePosition);						// サウンドハンドルにループ位置を設定する(サンプル単位)

			int set_loop_start_time_pos(ch::milliseconds LoopStartTime);						// サウンドハンドルにループ開始位置を設定する(ミリ秒単位)
			int set_loop_start_sample_pos(sample_n_t LoopStartSamplePosition);						// サウンドハンドルにループ開始位置を設定する(サンプル単位)

			int register_delete_play_finish(bool DeleteFlag = true) { return DxLib::SetPlayFinishDeleteSoundMem(DeleteFlag, GetHandle()); }						// サウンドハンドルの再生が終了したら自動的にハンドルを削除するかどうかを設定する

			int set_3d_peverb_param(const SOUND3D_REVERB_PARAM& Param) { return DxLib::Set3DReverbParamSoundMem(&Param, GetHandle()); }						// サウンドハンドルの３Ｄサウンド用のリバーブパラメータを設定する
			int set_3d_preset_reverb_param(reverb PresetNo);						// サウンドハンドルの３Ｄサウンド用のリバーブパラメータをプリセットを使用して設定する
			SOUND3D_REVERB_PARAM get_3d_reverb_param();						// サウンドハンドルに設定されている３Ｄサウンド用のリバーブパラメータを取得する

			int set_3d_position(VECTOR Position);						// サウンドハンドルの３Ｄサウンド用の再生位置を設定する
			int set_3d_position(const point3df& Position);						// サウンドハンドルの３Ｄサウンド用の再生位置を設定する
			int set_3d_radius(float Radius);						// サウンドハンドルの３Ｄサウンド用の音が聞こえる距離を設定する
			int set_3d_velocity(VECTOR Velocity);						// サウンドハンドルの３Ｄサウンド用の移動速度を設定する
			int set_3d_velocity(const point3df& Velocity);						// サウンドハンドルの３Ｄサウンド用の移動速度を設定する
			int set_next_play_3d_position(VECTOR Position);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の再生位置を設定する
			int set_next_play_3d_position(const point3df& Position);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の再生位置を設定する
			int set_next_play_3d_radius(float Radius);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の音が聞こえる距離を設定する
			int set_next_play_3d_velocity(VECTOR Velocity);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の移動速度を設定する
			int set_next_play_3d_velocity(const point3df& Velocity);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の移動速度を設定する

		private:
			sound(int param_handle) DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(param_handle){}
		};

		// 設定関係関数
		extern	int			SetCreateSoundDataType(int SoundDataType);																		// 作成するサウンドハンドルの再生タイプを設定する( DX_SOUNDDATATYPE_MEMNOPRESS 等 )
		extern	int			GetCreateSoundDataType(void);																					// 作成するサウンドハンドルの再生タイプを取得する( DX_SOUNDDATATYPE_MEMNOPRESS 等 )
		extern	int			SetCreateSoundPitchRate(float Cents);																			// 作成するサウンドハンドルのピッチ( 音の長さを変えずに音程を変更する )レートを設定する( 単位はセント( 100.0fで半音、1200.0fで１オクターヴ )、プラスの値で音程が高く、マイナスの値で音程が低くなります )
		extern	float		GetCreateSoundPitchRate(void);																					// 作成するサウンドハンドルのピッチ( 音の長さを変えずに音程を変更する )レートを取得する( 単位はセント( 100.0fで半音、1200.0fで１オクターヴ )、プラスの値で音程が高く、マイナスの値で音程が低くなります )
		extern	int			SetCreateSoundTimeStretchRate(float Rate);																				// 作成するサウンドハンドルのタイムストレッチ( 音程を変えずに音の長さを変更する )レートを設定する( 単位は倍率、2.0f で音の長さが２倍に、0.5f で音の長さが半分になります )
		extern	float		GetCreateSoundTimeStretchRate(void);																					// 作成するサウンドハンドルのタイムストレッチ( 音程を変えずに音の長さを変更する )レートを取得する( 単位は倍率、2.0f で音の長さが２倍に、0.5f で音の長さが半分になります )
		extern	int			SetDisableReadSoundFunctionMask(int Mask);																				// 使用しないサウンドデータ読み込み処理のマスクを設定する( DX_READSOUNDFUNCTION_PCM 等 )
		extern	int			GetDisableReadSoundFunctionMask(void);																					// 使用しないサウンドデータ読み込み処理のマスクを取得する( DX_READSOUNDFUNCTION_PCM 等 )
		extern	int			SetEnableSoundCaptureFlag(int Flag);																				// サウンドキャプチャを前提とした動作をするかどうかを設定する
		extern	int			SetUseOldVolumeCalcFlag(int Flag);																				// ChangeVolumeSoundMem, ChangeNextPlayVolumeSoundMem, ChangeMovieVolumeToGraph の音量計算式を Ver3.10c以前のものを使用するかどうかを設定する( TRUE:Ver3.10c以前の計算式を使用  FALSE:3.10d以降の計算式を使用( デフォルト ) )

		extern	int			SetCreate3DSoundFlag(int Flag);																			// 次に作成するサウンドハンドルを３Ｄサウンド用にするかどうかを設定する( TRUE:３Ｄサウンド用にする  FALSE:３Ｄサウンド用にしない( デフォルト ) )
		extern	int			Set3DSoundOneMetre(float Distance);																	// ３Ｄ空間の１メートルに相当する距離を設定する、DxLib_Init を呼び出す前でのみ呼び出し可能( デフォルト:1.0f )
		extern	int			Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR Position, VECTOR FrontPosition);												// ３Ｄサウンドのリスナーの位置とリスナーの前方位置を設定する( リスナーの上方向はＹ軸固定 )
		extern	int			Set3DSoundListenerPosAndFrontPosAndUpVec(VECTOR Position, VECTOR FrontPosition, VECTOR UpVector);							// ３Ｄサウンドのリスナーの位置とリスナーの前方位置とリスナーの上方向を設定する
		extern	int			Set3DSoundListenerVelocity(VECTOR Velocity);																	// ３Ｄサウンドのリスナーの移動速度を設定する
		extern	int			Set3DSoundListenerConeAngle(float InnerAngle, float OuterAngle);												// ３Ｄサウンドのリスナーの可聴角度範囲を設定する
		extern	int			Set3DSoundListenerConeVolume(float InnerAngleVolume, float OuterAngleVolume);									// ３Ｄサウンドのリスナーの可聴角度範囲の音量倍率を設定する
																																			//extern	int			SetUseOldVolumeCalcFlag(int Flag);																				// ChangeVolumeSoundMem, ChangeNextPlayVolumeSoundMem, ChangeMovieVolumeToGraph の音量計算式を Ver3.10c以前のものを使用するかどうかを設定する( TRUE:Ver3.10c以前の計算式を使用  FALSE:3.10d以降の計算式を使用( デフォルト ) )


		//生成

		// 前奏部とループ部に分かれたサウンドファイルを読み込みサウンドハンドルを作成する
		sound	LoadSoundMem2(const TCHAR *FileName1, const TCHAR *FileName2)DXLE_NOEXCEPT_OR_NOTHROW{ return sound::LoadSoundMem2(FileName1, FileName2); }
		// 主にＢＧＭを読み込みサウンドハンドルを作成するのに適した関数
		sound	LoadBGM(const TCHAR *FileName)DXLE_NOEXCEPT_OR_NOTHROW{ return sound::load_sound(FileName, sound_data_type::big); }
		// サウンドファイルからサウンドハンドルを作成する
		sound	LoadSoundMemBase(const TCHAR *FileName, int BufferNum, int UnionHandle = -1){ return sound::load_sound(FileName, BufferNum, UnionHandle); }
		// サウンドファイルからサウンドハンドルを作成する(LoadSoundMemBase の別名関数)
		sound	LoadSoundMem(const TCHAR *FileName, int BufferNum = 3, int UnionHandle = -1){ return sound::load_sound(FileName, BufferNum, UnionHandle); }
		// 同じサウンドデータを使用するサウンドハンドルを作成する( DX_SOUNDDATATYPE_MEMNOPRESS タイプのサウンドハンドルのみ可能 )
		//sound	DuplicateSoundMem(sound SrcSoundHandle, int BufferNum = 3){ return sound::DuplicateSoundMem(SrcSoundHandle, BufferNum); }
		// メモリ上に展開されたサウンドファイルイメージからサウンドハンドルを作成する
		sound	LoadSoundMemByMemImageBase(const void *FileImage, int FileImageSize, int BufferNum, int UnionHandle = -1){ return sound::LoadSoundMemByMemImageBase(FileImage, FileImageSize, BufferNum, UnionHandle); }
		sound	LoadSoundMemByMemImage(const void *FileImage, int FileImageSize, int UnionHandle = -1);			// メモリ上に展開されたサウンドファイルイメージからサウンドハンドルを作成する( バッファ数指定無し )
		sound	LoadSoundMemByMemImage2(const void *WaveImage, int WaveImageSize, const WAVEFORMATEX *WaveFormat, int WaveHeaderSize);	// メモリ上に展開されたＰＣＭデータからサウンドハンドルを作成する
		sound	LoadSoundMem2ByMemImage(const void *FileImage1, int FileImageSize1, const void *FileImage2, int FileImageSize2);	// 前奏部とループ部に分かれた二つのメモリ上に展開されたサウンドファイルイメージからサウンドハンドルを作成する
		sound	LoadSoundMemFromSoftSound(int SoftSoundHandle, int BufferNum = 3);													// ソフトウエアサウンドハンドルが持つサウンドデータからサウンドハンドルを作成する

		//削除
		extern	int			DeleteSoundMem(int SoundHandle, int LogOutFlag = FALSE);												// サウンドハンドルを削除する

		//操作
		extern	int			PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag = TRUE);								// サウンドハンドルを再生する
		extern	int			StopSoundMem(int SoundHandle);						// サウンドハンドルの再生を停止する
		extern	int			CheckSoundMem(int SoundHandle);						// サウンドハンドルが再生中かどうかを取得する
		extern	int			SetPanSoundMem(int PanPal, int SoundHandle);						// サウンドハンドルのパンを設定する( 100分の1デシベル単位 0 ～ 10000 )
		extern	int			ChangePanSoundMem(int PanPal, int SoundHandle);						// サウンドハンドルのパンを設定する( -255 ～ 255 )
		extern	int			GetPanSoundMem(int SoundHandle);						// サウンドハンドルのパンを取得する
		extern	int			SetVolumeSoundMem(int VolumePal, int SoundHandle);						// サウンドハンドルのボリュームを設定する( 100分の1デシベル単位 0 ～ 10000 )
		extern	int			ChangeVolumeSoundMem(int VolumePal, int SoundHandle);						// サウンドハンドルのボリュームを設定する( 0 ～ 255 )
		extern	int			GetVolumeSoundMem(int SoundHandle);						// サウンドハンドルのボリュームを取得する
		extern	int			SetChannelVolumeSoundMem(int Channel, int VolumePal, int SoundHandle);						// サウンドハンドルの指定のチャンネルのボリュームを設定する( 100分の1デシベル単位 0 ～ 10000 )
		extern	int			ChangeChannelVolumeSoundMem(int Channel, int VolumePal, int SoundHandle);						// サウンドハンドルの指定のチャンネルのボリュームを設定する( 0 ～ 255 )
		extern	int			GetChannelVolumeSoundMem(int Channel, int SoundHandle);						// サウンドハンドルの指定のチャンネルのボリュームを取得する
		extern	int			SetFrequencySoundMem(int FrequencyPal, int SoundHandle);						// サウンドハンドルの再生周波数を設定する
		extern	int			GetFrequencySoundMem(int SoundHandle);						// サウンドハンドルの再生周波数を取得する
		extern	int			ResetFrequencySoundMem(int SoundHandle);						// サウンドハンドルの再生周波数を読み込み直後の状態に戻す

		extern	int			SetNextPlayPanSoundMem(int PanPal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用するパンを設定する( 100分の1デシベル単位 0 ～ 10000 )
		extern	int			ChangeNextPlayPanSoundMem(int PanPal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用するパンを設定する( -255 ～ 255 )
		extern	int			SetNextPlayVolumeSoundMem(int VolumePal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用するボリュームを設定する( 100分の1デシベル単位 0 ～ 10000 )
		extern	int			ChangeNextPlayVolumeSoundMem(int VolumePal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用するボリュームを設定する( 0 ～ 255 )
		extern	int			SetNextPlayChannelVolumeSoundMem(int Channel, int VolumePal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用するチャンネルのボリュームを設定する( 100分の1デシベル単位 0 ～ 10000 )
		extern	int			ChangeNextPlayChannelVolumeSoundMem(int Channel, int VolumePal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用するチャンネルのボリュームを設定する( 0 ～ 255 )
		extern	int			SetNextPlayFrequencySoundMem(int FrequencyPal, int SoundHandle);						// サウンドハンドルの次の再生にのみ使用する再生周波数を設定する

		extern	int			SetCurrentPositionSoundMem(int SamplePosition, int SoundHandle);						// サウンドハンドルの再生位置をサンプル単位で設定する(再生が止まっている時のみ有効)
		extern	int			GetCurrentPositionSoundMem(int SoundHandle);						// サウンドハンドルの再生位置をサンプル単位で取得する
		extern	int			SetSoundCurrentPosition(int Byte, int SoundHandle);						// サウンドハンドルの再生位置をバイト単位で設定する(再生が止まっている時のみ有効)
		extern	int			GetSoundCurrentPosition(int SoundHandle);						// サウンドハンドルの再生位置をバイト単位で取得する
		extern	int			SetSoundCurrentTime(int Time, int SoundHandle);						// サウンドハンドルの再生位置をミリ秒単位で設定する(圧縮形式の場合は正しく設定されない場合がある)
		extern	int			GetSoundCurrentTime(int SoundHandle);						// サウンドハンドルの再生位置をミリ秒単位で取得する(圧縮形式の場合は正しい値が返ってこない場合がある)
		extern	int			GetSoundTotalSample(int SoundHandle);						// サウンドハンドルの音の総時間をサンプル単位で取得する
		extern	int			GetSoundTotalTime(int SoundHandle);						// サウンドハンドルの音の総時間をミリ秒単位で取得する

		extern	int			SetLoopPosSoundMem(int LoopTime, int SoundHandle);						// SetLoopTimePosSoundMem の別名関数
		extern	int			SetLoopTimePosSoundMem(int LoopTime, int SoundHandle);						// サウンドハンドルにループ位置を設定する(ミリ秒単位)
		extern	int			SetLoopSamplePosSoundMem(int LoopSamplePosition, int SoundHandle);						// サウンドハンドルにループ位置を設定する(サンプル単位)

		extern	int			SetLoopStartTimePosSoundMem(int LoopStartTime, int SoundHandle);						// サウンドハンドルにループ開始位置を設定する(ミリ秒単位)
		extern	int			SetLoopStartSamplePosSoundMem(int LoopStartSamplePosition, int SoundHandle);						// サウンドハンドルにループ開始位置を設定する(サンプル単位)

		extern	int			SetPlayFinishDeleteSoundMem(int DeleteFlag, int SoundHandle);						// サウンドハンドルの再生が終了したら自動的にハンドルを削除するかどうかを設定する



		//3D
#		if 0
		extern	int			Set3DReverbParamSoundMem(            const SOUND3D_REVERB_PARAM *Param,              int SoundHandle ) ;						// サウンドハンドルの３Ｄサウンド用のリバーブパラメータを設定する
		extern	int			Set3DPresetReverbParamSoundMem(      int PresetNo /* DX_REVERB_PRESET_DEFAULT 等 */, int SoundHandle );						// サウンドハンドルの３Ｄサウンド用のリバーブパラメータをプリセットを使用して設定する
		extern	int			Set3DReverbParamSoundMemAll(const SOUND3D_REVERB_PARAM *Param, int PlaySoundOnly = FALSE);							// 全ての３Ｄサウンドのサウンドハンドルにリバーブパラメータを設定する( PlaySoundOnly TRUE:再生中のサウンドにのみ設定する  FALSE:再生していないサウンドにも設定する )
		extern	int			Set3DPresetReverbParamSoundMemAll(int PresetNo /* DX_REVERB_PRESET_DEFAULT 等 */, int PlaySoundOnly = FALSE);			// 全ての３Ｄサウンドのサウンドハンドルにリバーブパラメータをプリセットを使用して設定する( PlaySoundOnly TRUE:再生中のサウンドにのみ設定する  FALSE:再生していないサウンドにも設定する )
		extern	int			Get3DReverbParamSoundMem(SOUND3D_REVERB_PARAM *ParamBuffer, int SoundHandle);						// サウンドハンドルに設定されている３Ｄサウンド用のリバーブパラメータを取得する
		extern	int			Get3DPresetReverbParamSoundMem(SOUND3D_REVERB_PARAM *ParamBuffer, int PresetNo /* DX_REVERB_PRESET_DEFAULT 等 */);		// プリセットの３Ｄサウンド用のリバーブパラメータを取得する

		extern	int			Set3DPositionSoundMem(VECTOR Position, int SoundHandle);						// サウンドハンドルの３Ｄサウンド用の再生位置を設定する
		extern	int			Set3DRadiusSoundMem(float Radius, int SoundHandle);						// サウンドハンドルの３Ｄサウンド用の音が聞こえる距離を設定する
		extern	int			Set3DVelocitySoundMem(VECTOR Velocity, int SoundHandle);						// サウンドハンドルの３Ｄサウンド用の移動速度を設定する

		extern	int			SetNextPlay3DPositionSoundMem(VECTOR Position, int SoundHandle);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の再生位置を設定する
		extern	int			SetNextPlay3DRadiusSoundMem(float Radius, int SoundHandle);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の音が聞こえる距離を設定する
		extern	int			SetNextPlay3DVelocitySoundMem(VECTOR Velocity, int SoundHandle);						// サウンドハンドルの次の再生のみに使用する３Ｄサウンド用の移動速度を設定する

		// 設定関係関数
		extern	int			SetCreate3DSoundFlag(int Flag);																			// 次に作成するサウンドハンドルを３Ｄサウンド用にするかどうかを設定する( TRUE:３Ｄサウンド用にする  FALSE:３Ｄサウンド用にしない( デフォルト ) )
		extern	int			Set3DSoundOneMetre(float Distance);																	// ３Ｄ空間の１メートルに相当する距離を設定する、DxLib_Init を呼び出す前でのみ呼び出し可能( デフォルト:1.0f )
		extern	int			Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR Position, VECTOR FrontPosition);												// ３Ｄサウンドのリスナーの位置とリスナーの前方位置を設定する( リスナーの上方向はＹ軸固定 )
		extern	int			Set3DSoundListenerPosAndFrontPosAndUpVec(VECTOR Position, VECTOR FrontPosition, VECTOR UpVector);							// ３Ｄサウンドのリスナーの位置とリスナーの前方位置とリスナーの上方向を設定する
		extern	int			Set3DSoundListenerVelocity(VECTOR Velocity);																	// ３Ｄサウンドのリスナーの移動速度を設定する
		extern	int			Set3DSoundListenerConeAngle(float InnerAngle, float OuterAngle);												// ３Ｄサウンドのリスナーの可聴角度範囲を設定する
		extern	int			Set3DSoundListenerConeVolume(float InnerAngleVolume, float OuterAngleVolume);									// ３Ｄサウンドのリスナーの可聴角度範囲の音量倍率を設定する

#		endif
	}
	using namespace sound;
}
//#ifndef DXLE_INC_SOUND_HPP_
#endif
