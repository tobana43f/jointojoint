//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS

#include	"PlatformSelector.h"
#include	"System.h"
#include	"Allocator.h"	// 安全な解放
#include	"Texture.h"		// クラス定義

#include	<stdio.h>		// 書式付き文字列操作

using namespace NSystem;
using namespace NAllocator;
using namespace NTextureResourceInfo;
using namespace NAnimationResourceInfo;

// テクスチャリソースまでのパス
// テクスチャの拡張子
#ifdef _PLATFORM_DIRECTX
const char	*TEXTURE_RESOURCE_DIRECTORY = "_Resource/Texture/";	
const char	*TEXTURE_RESOURCE_EXTENSION = ".tga";	
#endif

CTexture::CTexture(){
	Init();
}

CTexture::~CTexture(){
	ReleaseAnimation();
	ReleaseTexture();
}

// 全ての値を初期化する
// 注意) テクスチャの解放処理は行わない
void CTexture::Init(){
	pTexPal = NULL;
	tex = NULL;		
	divSizeU = 1.0f;
	divSizeV = 1.0f;
	divX = 1;
	divY = 1;
	sizeX = 1;
	sizeY = 1;
	divSizeX = 1;
	divSizeY = 1;
	pAnimation = NULL;
}

// テクスチャの読み込み
void CTexture::LoadTexture(ETextureLabel _label){
	char filePath_[256];

	// ファイルパスの作成
	sprintf(filePath_, "%s%s%s", TEXTURE_RESOURCE_DIRECTORY, GetTextureResourceInfo(_label).GetFileName(), TEXTURE_RESOURCE_EXTENSION);
	
#ifdef _PLATFORM_DIRECTX
	if (FAILED(D3DXCreateTextureFromFile(gpD3DDevice, filePath_, &tex))){
		// error
		return;
	}
#endif
	
	divX = GetTextureResourceInfo(_label).GetDivX();					// 分割数を設定
	divY = GetTextureResourceInfo(_label).GetDivY();
	divSizeU = (float)(1.0f / divX);	// 一辺あたりの長さを設定
	divSizeV = (float)(1.0f / divY);

	sizeX = GetTextureResourceInfo(_label).GetSizeX();				// 画像のサイズを設定
	sizeY = GetTextureResourceInfo(_label).GetSizeY();
	divSizeX = (float)(sizeX / (float)divX);		// テクスチャ１つあたりのサイズを設定(分割後のサイズ)
	divSizeY = (float)(sizeY / (float)divY);
}

// テクスチャ開放
void CTexture::ReleaseTexture(){
#ifdef _PLATFORM_DIRECTX
	if (tex != NULL){
		IDirect3DTexture9_Release(tex);
		tex = NULL;
	}
#endif
	Init();
}

// アニメーションの読込
// アニメーションの実態を生成
// 現状はポインタの参照を解除しているだけ。
void CTexture::LoadAnimation(EAnimationLabel _label){
	ReleaseAnimation();	// 先に解放処理をして、メモリリークを避ける。
	pAnimation = new CAnimation;
	pAnimation->Load(_label);
}


// アニメーションの解放
void CTexture::ReleaseAnimation(){
	DeleteSafely(pAnimation);
}


// 現在のテクスチャを取得
int CTexture::GetAnmData(int _anmNo, int _anmCnt){
	if (pAnimation != NULL)
		return (pAnimation->GetAnmData(_anmNo,_anmCnt));
	return 0;
}
