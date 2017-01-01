/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "dxlibex/basic_types.hpp"
#include "dxlibex/color.hpp"
#include "dxlibex/time.hpp"

template<typename... Args>
int DrawFormatString(dxle::pointi pos, dxle::dx_color_param color, const char* str, Args&&... args)
{
	return DxLib::DrawFormatString(pos.x, pos.y, color.get(), str, std::forward<Args>(args)...);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib::ChangeWindowMode(true);
	if (DxLib::DxLib_Init() == -1) {
		return -1;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SetFontSize(100);
	bool is_last_push_space = false;
	bool is_last_push_z = false;

	dxle::timer timer;
	while (DxLib::ProcessMessage() == 0)
	{

		if (DxLib::CheckHitKey(KEY_INPUT_SPACE)){
			if (is_last_push_space == false)
			{
				is_last_push_space = true;
				if (timer.is_stop()){
					timer.start();
				}
				else{
					timer.stop();
				}
			}
		}
		else{
			is_last_push_space = false;
		}
		if (DxLib::CheckHitKey(KEY_INPUT_Z)){
			if (is_last_push_z == false)
			{
				is_last_push_z = true;
				timer.reset();
			}
		}
		else{
			is_last_push_z = false;
		}
		DrawFormatString({ 140, 190 }, dxle::color_tag::white, "%3.3f", timer.get_pass_time().count() / 1000.0f);
		DxLib::printfDx("space: start/stop\n");
		DxLib::printfDx("z    : reset\n");

		DxLib::ScreenFlip();
		DxLib::ClearDrawScreen();
		DxLib::clsDx();
	}

	DxLib::DxLib_End();
	return 0;
}
