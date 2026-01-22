//==========================================================
// メモリ操作にかかわるメソッドの定義
#ifndef _INC_ALLOCATOR_H
#define _INC_ALLOCATOR_H

#include	"PlatformSelector.h"

//==========================================================
// メモリリークを検出できるようにするマクロ
// メモ)
// 　DirectX専用
// 　new 演算子を使うソースファイルには必ずインクルードしてください。
#ifdef _PLATFORM_DIRECTX
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif


//==========================================================
// 関数定義

// 安全に解放する関数
#define DeleteSafely(_pTarget) {\
	if(_pTarget!=NULL){			\
		delete _pTarget;		\
		_pTarget = NULL;		\
	}							\
}

// 安全に配列を開放する関数
#define DeleteArraySafely(_pTarget) {	\
	if(_pTarget != NULL){				\
		delete[] _pTarget;				\
		_pTarget = NULL;				\
	}									\
}

namespace NAllocator {

}

#endif