//#include <dxlibex/Sound.h>
#include <dxlibex/basic_types/bel.hpp>
#include <Windows.h>
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
	//SetCreateSoundPitchRate(100);
#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
	constexpr auto rate = 300_dB;
	constexpr auto rate2 = 4000_myrioB;
	constexpr auto rate3 = dxle::bel_cast<dxle::bel>(rate);
#endif
	constexpr dxle::deci_bel rate4(100);
}
