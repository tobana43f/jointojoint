//==========================================================
// プラットフォームを選択するヘッダ
#ifndef _INC_CORSS_PLATFORM_SELECTOR
#define _INC_CROSS_PLATFORM_SELECTOR

#define _PLATFORM_DIRECTX	// この行をコメントアウトするとWii用に切り替わる。
#ifndef _PLATFORM_DIRECTX
#define _PLATFORM_WII
#endif

#endif