#include <dxlibex/sound.hpp>
#include <dxlibex/basic_types/bel.hpp>
#include <Windows.h>
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
	//SetCreateSoundPitchRate(100);
	constexpr auto str = _T("arikitari");
	const dxle::tstring ss = _T("arikitari");
	dxle::tinferior_string_ref s(str);
	dxle::tinferior_string_ref s2(ss);
#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
	using namespace dxle::sound_units_literals;
	constexpr auto rate = 300_dB;
	constexpr auto rate2 = 4000_myrioB;
	constexpr auto rate3 = dxle::bel_cast<dxle::bel>(rate);
	constexpr auto rate2_2 = rate2;
#endif
	constexpr dxle::deci_bel rate4(100);
	constexpr dxle::myrio_bel rate5{ 20 };
}
