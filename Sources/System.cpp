//==========================================================
// System.hでの宣言の実態を生成する

//==========================================================
// インクルード
#include	"System.h"
#include	"Math.h"

//==========================================================
// 宣言
namespace NSystem{
#ifdef _PLATFORM_DIRECTX
	HWND				gHWnd = NULL;
	LPDIRECT3D9			gpD3D = NULL;
	LPDIRECT3DDEVICE9	gpD3DDevice = NULL;

	const char*	WINDOW_NAME = "win32_DirectX";
	const char* WINDOW_TITLE = "Join To Joint";

	SVector2D	windowPosLeftTop;
#endif

	const float WINDOW_WIDTH = 640.0f;
	const float WINDOW_HEIGHT = 480.0f;

	const float DEFAULT_FRAME_RATE = 60.0f;

	const unsigned char DEFAULT_BACKGROUND_COLOR_R = 128;
	const unsigned char DEFAULT_BACKGROUND_COLOR_G = 128;
	const unsigned char DEFAULT_BACKGROUND_COLOR_B = 128;
}

