//==========================================================
// インクルード
#include	"AllTextureManager.h"
#include	"Allocator.h"

//==========================================================
// 名前空間
using namespace NTextureResourceInfo;

//==========================================================
// グローバル変数
static CTexture	gAllTexture[TEX_LABEL_MAX];

//==========================================================
// メソッド定義
void NAllTextureManager::LoadAllTexture(){
	for (int i = 0; i < TEX_LABEL_MAX; ++i){
		ETextureLabel label_ = (ETextureLabel)i;
		gAllTexture[label_].LoadTexture(label_);
	}
}

CTexture* NAllTextureManager::GetPTexture(NTextureResourceInfo::ETextureLabel _label){
	return &gAllTexture[_label];
}