//==========================================================
// 全テクスチャをメモリにロードする
#ifndef _INC_ALL_TEXTURE_MANAGER_H
#define _INC_ALL_TEXTURE_MANAGER_H

//==========================================================
// インクルード
#include	"Texture.h"	
#include	"TextureResourceInfo.h"

//==========================================================
// グローバル
namespace NAllTextureManager {
	void LoadAllTexture();

	CTexture* GetPTexture(NTextureResourceInfo::ETextureLabel _label);
}

#endif