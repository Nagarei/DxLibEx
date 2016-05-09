#include <dxlibex/sound.hpp>
#include <dxlibex/basic_types/bel.hpp>
#include <Windows.h>
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
	//SetCreateSoundPitchRate(100);
	DXLE_CONSTEXPR auto str = _T("arikitari");
	const dxle::tstring ss = _T("arikitari");
	dxle::tinferior_string_ref s(str);
	dxle::tinferior_string_ref s2(ss);
#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
	using namespace dxle::sound_units_literals;
	DXLE_CONSTEXPR auto rate = 300_dB;
	DXLE_CONSTEXPR auto rate2 = 4000_myrioB;
	DXLE_CONSTEXPR auto rate3 = dxle::bel_cast<dxle::bel>(rate);
	DXLE_CONSTEXPR auto rate2_2 = rate2;
#endif
	DXLE_CONSTEXPR dxle::deci_bel rate4(100);
	DXLE_CONSTEXPR dxle::myrio_bel rate5{ 20 };
}
