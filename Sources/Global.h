//==========================================================
// ゲームで使用するグローバル変数を記述するヘッダ
// 簡単に宣言できるように、 _GLOBAL_CPP を全cppの内１つのcppで定義する。
// EXTERNマクロの後に宣言するだけで、グローバル変数として使える。
// 注意)
// 　基本的には使用しないでください。
// 　デバッグで常にその値を見ていたい変数などがある場合に使用する。
// 　基本は、シーン別のクラス内で完結すること。
#ifndef _INC_GLOBAL_H
#define _INC_GLOBAL_H

//==========================================================
// インクルード
#include	"PlatformSelector.h"

#ifdef		_PLATFORM_DIRECTX
#include <d3dx9.h>	
#endif

//==========================================================
// externマクロ
#ifdef _GLOBAL_CPP
#define EXTERN	
#else
#define EXTERN	extern
#endif

#ifdef _DIRECTX

#endif
#ifdef _WII

#endif
#endif