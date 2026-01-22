//==========================================================
// システムに関わる定数などを定義する。
#ifndef _INC_SYSTEM_H
#define _INC_SYSTEM_H

//==========================================================
// インクルード
#include	"PlatformSelector.h"
#ifdef		_PLATFORM_DIRECTX
#include	<Windows.h>
#include	<d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include	"Math.h"
#endif

//==========================================================
// 宣言
namespace NSystem{
#ifdef _PLATFORM_DIRECTX
	extern	HWND				gHWnd;
	extern	LPDIRECT3D9			gpD3D;
	extern	LPDIRECT3DDEVICE9	gpD3DDevice;

	extern const char*	WINDOW_NAME;	// Windowの識別子
	extern const char*	WINDOW_TITLE;	// Windowのタイトル

	extern SVector2D	windowPosLeftTop;
#endif

	// 画面サイズ
	extern const float	WINDOW_WIDTH;
	extern const float	WINDOW_HEIGHT;

	// デフォルトのフレームレート
	extern const float DEFAULT_FRAME_RATE;

	// デフォルトの背景色
	extern const unsigned char DEFAULT_BACKGROUND_COLOR_R;
	extern const unsigned char DEFAULT_BACKGROUND_COLOR_G;
	extern const unsigned char DEFAULT_BACKGROUND_COLOR_B;
}
#endif