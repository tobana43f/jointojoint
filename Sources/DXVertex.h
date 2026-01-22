//==========================================================
// DirectXに適した頂点構造体を定義
#ifndef _DXVERTEX_H
#define _DXVERTEX_H

#include	"PlatformSelector.h"
#ifdef		_PLATFORM_DIRECTX
#include	<d3dx9.h>

//==========================================================
// SDXVertex2D
struct SDXVertex2D{
	float		x, y, z;
	float		rhw;
	D3DCOLOR	diffuse;
	float		tu, tv;
};


//==========================================================
// SDXVertex3D
struct SDXVertex3D{
	float		x, y, z;
	D3DCOLOR	diffuse;
	float		tu, tv;
};

#endif
#endif
