#ifndef DX_LIB_EX_STRUCTS_H_
#define DX_LIB_EX_STRUCTS_H_
#include "DxLib.h"

namespace DxLibEx{
namespace DxStructs//!DxLibのstructのusing宣言を集めたnamespaceです
{
	
	using DxLib::IMEINPUTCLAUSEDATA;//!ＩＭＥ入力文字列の描画に必要な情報の内の文節情報
	using DxLib::LPIMEINPUTCLAUSEDATA;//!ＩＭＥ入力文字列の描画に必要な情報の内の文節情報

	using DxLib::IMEINPUTDATA;//!ＩＭＥ入力文字列の描画に必要な情報
	using DxLib::LPIMEINPUTDATA;//!ＩＭＥ入力文字列の描画に必要な情報

	using DxLib::DISPLAYMODEDATA;//!画面モード情報データ型
	using DxLib::LPDISPLAYMODEDATA;//!画面モード情報データ型

	using DxLib::DATEDATA;//!タイムデータ型
	using DxLib::LPDATEDATA;//!タイムデータ型

	using DxLib::FILEINFO;//!ファイル情報構造体
	using DxLib::LPFILEINFO;//!ファイル情報構造体

	using DxLib::FILEINFOW;//!ファイル情報構造体（ wchar_t 版 ）
	using DxLib::LPFILEINFOW;//!ファイル情報構造体（ wchar_t 版 ）

	using DxLib::MATRIX;//!行列構造体
	using DxLib::LPMATRIX;//!行列構造体

	using DxLib::MATRIX_D;//!行列構造体
	using DxLib::LPMATRIX_D;//!行列構造体

	using DxLib::VECTOR;//!ベクトルデータ型
	using DxLib::LPVECTOR;//!ベクトルデータ型
	using DxLib::FLOAT3;//!ベクトルデータ型
	using DxLib::LPFLOAT3;//!ベクトルデータ型

	using DxLib::VECTOR_D;//!ベクトルデータ型
	using DxLib::LPVECTOR_D;//!ベクトルデータ型
	using DxLib::DOUBLE3;//!ベクトルデータ型
	using DxLib::LPDOUBLE3;//!ベクトルデータ型

	using DxLib::FLOAT2;//!FLOAT2個データ型

	using DxLib::COLOR_F;//!float 型のカラー値
	using DxLib::LPCOLOR_F;//!float 型のカラー値

	using DxLib::COLOR_U8;//!unsigned char 型のカラー値

	using DxLib::FLOAT4;//!FLOAT4個データ型
	using DxLib::LPFLOAT4;//!FLOAT4個データ型

	using DxLib::DOUBLE4;//!DOUBLE4個データ型
	using DxLib::LPDOUBLE4;//!DOUBLE4個データ型

	using DxLib::INT4;//!INT4個データ型

#ifndef DX_NOTUSE_DRAWFUNCTION

	using DxLib::VERTEX2D;//!２Ｄ描画に使用する頂点データ型(DrawPrimitive2D用)
	using DxLib::LPVERTEX2D;//!２Ｄ描画に使用する頂点データ型(DrawPrimitive2D用)

	using DxLib::VERTEX2DSHADER;//!２Ｄ描画に使用する頂点データ型(公開用)
	using DxLib::LPVERTEX2DSHADER;//!２Ｄ描画に使用する頂点データ型(公開用)

	using DxLib::VERTEX_3D;//!３Ｄ描画に使用する頂点データ型( 旧バージョンのもの )
	using DxLib::LPVERTEX_3D;//!３Ｄ描画に使用する頂点データ型( 旧バージョンのもの )

	using DxLib::VERTEX3D;//!３Ｄ描画に使用する頂点データ型
	using DxLib::LPVERTEX3D;//!３Ｄ描画に使用する頂点データ型

	//!３Ｄ描画に使用する頂点データ型( DrawPrimitive3DToShader用 )
	//!注意…メンバ変数に追加があるかもしれませんので、宣言時の初期化( VERTEX3DSHADER Vertex = { 0.0f, 0.0f, ... というようなもの )はしない方が良いです
	using DxLib::VERTEX3DSHADER;
	//!３Ｄ描画に使用する頂点データ型( DrawPrimitive3DToShader用 )
	//!注意…メンバ変数に追加があるかもしれませんので、宣言時の初期化( VERTEX3DSHADER Vertex = { 0.0f, 0.0f, ... というようなもの )はしない方が良いです
	using DxLib::LPVERTEX3DSHADER;

	using DxLib::LIGHTPARAM;//!ライトパラメータ

	using DxLib::MATERIALPARAM;//!マテリアルパラメータ

#endif//#ifndef DX_NOTUSE_DRAWFUNCTION

	using DxLib::HITRESULT_LINE;//!ラインヒットチェック結果格納用構造体
	using DxLib::HITRESULT_LINE_D;//!ラインヒットチェック結果格納用構造体

	using DxLib::SEGMENT_SEGMENT_RESULT;//!関数 Segment_Segment_Analyse の結果を受け取る為の構造体
	using DxLib::SEGMENT_SEGMENT_RESULT_D;//!関数 Segment_Segment_Analyse の結果を受け取る為の構造体

	using DxLib::SEGMENT_POINT_RESULT;//!関数 Segment_Point_Analyse の結果を受け取る為の構造体
	using DxLib::SEGMENT_POINT_RESULT_D;//!関数 Segment_Point_Analyse の結果を受け取る為の構造体

	using DxLib::SEGMENT_TRIANGLE_RESULT;//!関数 Segment_Triangle_Analyse の結果を受け取る為の構造体
	using DxLib::SEGMENT_TRIANGLE_RESULT_D;//!関数 Segment_Triangle_Analyse の結果を受け取る為の構造体

	using DxLib::TRIANGLE_POINT_RESULT;//!関数 Triangle_Point_Analyse の結果を受け取る為の構造体
	using DxLib::TRIANGLE_POINT_RESULT_D;//!関数 Triangle_Point_Analyse の結果を受け取る為の構造体

	using DxLib::PLANE_POINT_RESULT;//!関数 Plane_Point_Analyse の結果を受け取る為の構造体
	using DxLib::PLANE_POINT_RESULT_D;//!関数 Plane_Point_Analyse の結果を受け取る為の構造体

#ifndef DX_NOTUSE_DRAWFUNCTION

	using DxLib::MV1_COLL_RESULT_POLY;//!コリジョン結果代入用ポリゴン

	using DxLib::MV1_COLL_RESULT_POLY_DIM;//!コリジョン結果代入用ポリゴン配列

	using DxLib::MV1_REF_VERTEX;//!参照用頂点構造体

	using DxLib::MV1_REF_POLYGON;//!参照用ポリゴン構造体

	using DxLib::MV1_REF_POLYGONLIST;//!参照用ポリゴンデータ構造体

#endif //#ifndef DX_NOTUSE_DRAWFUNCTION

	

	using DxLib::SOUND3D_REVERB_PARAM;//!３Ｄサウンドリバーブエフェクトパラメータ構造体

	using DxLib::STREAMDATASHREDTYPE2;//!ストリームデータ制御用関数ポインタ構造体タイプ２
	using DxLib::STREAMDATASHREDTYPE2W;//!ストリームデータ制御用関数ポインタ構造体タイプ２の wchar_t 使用版

	using DxLib::STREAMDATASHRED;//!ストリームデータ制御用関数ポインタ構造体
	using DxLib::LPSTREAMDATASHRED;//!ストリームデータ制御用関数ポインタ構造体

	using DxLib::STREAMDATA;//!ストリームデータ制御用データ構造体



	using DxLib::STREAMDATA;//!パレット情報構造体

	using DxLib::COLORDATA;//!カラー構造情報構造体

	using DxLib::BASEIMAGE;//!基本イメージデータ構造体
	using DxLib::GRAPHIMAGE;//!基本イメージデータ構造体
	using DxLib::LPGRAPHIMAGE;//!基本イメージデータ構造体

	using DxLib::LINEDATA;//!ラインデータ型
	using DxLib::LPLINEDATA;//!ラインデータ型

	using DxLib::POINTDATA;//!座標データ型
	using DxLib::LPPOINTDATA;//!座標データ型

#ifndef DX_NOTUSE_DRAWFUNCTION

	using DxLib::IMAGEFORMATDESC;//!イメージフォーマットデータ

#endif //#ifndef DX_NOTUSE_DRAWFUNCTION

	using DxLib::DINPUT_JOYSTATE;//!DirectInput のジョイパッド入力情報

	using DxLib::XINPUT_STATE;//!XInput のジョイパッド入力情報

	using DxLib::TOUCHINPUTPOINT;//!タッチパネルの１箇所分のタッチの情報

	using DxLib::TOUCHINPUTDATA;//!タッチパネルのタッチの情報



	using DxLib::IPDATA;//!WinSockets使用時のアドレス指定用構造体
	using DxLib::LPIPDATA;//!WinSockets使用時のアドレス指定用構造体

	using DxLib::IPDATA_IPv6;

}
using namespace DxStructs;
}//namespace end

#endif
