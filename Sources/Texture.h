//==========================================================
// テクスチャの情報を格納するクラス
#ifndef _INC_TEXTURE_H
#define _INC_TEXTURE_H

#include	"PlatformSelector.h"
#include	"Animation.h"				// 内包する
#include	"TextureResourceInfo.h"		// テクスチャラベル型の参照
#include	"AnimationResourceInfo.h"	// アニメーションラベルの参照

//==========================================================
// 型の定義 (他の場所で使用しないでください。)
#ifdef		_PLATFORM_DIRECTX
#include	<d3dx9.h>	
// テクスチャに使用する型を再定義
typedef LPDIRECT3DTEXTURE9		STextureFile;

typedef struct {} TPLPalette, *TPLPalettePtr;

#endif

class CTexture{
public:
	void Init();

	// テクスチャ関係
	void	LoadTexture(NTextureResourceInfo::ETextureLabel _label);

	// アニメーション関係
	void	LoadAnimation(NAnimationResourceInfo::EAnimationLabel _label);


	// ゲッター
	TPLPalettePtr	GetTexturePalette(){ return pTexPal; }
	STextureFile	GetTexture(){ return tex; }
	float			GetDivSizeU(){ return divSizeU; }
	float			GetDivSizeV(){ return divSizeV; }
	int				GetDivX(){ return divX; }
	int				GetDivY(){ return divY; }
	int				GetSizeX(){ return sizeX; }
	int				GetSizeY(){ return sizeY; }
	float			GetDivSizeX(){ return divSizeX; }
	float			GetDivSizeY(){ return divSizeY; }
	float			getHDivSizeRatio(){ return (divSizeX / divSizeY); }	// 横の分割数の比率を取得(横:縦)(使う？）
	float			getVDivSizeRatio(){ return (divSizeY / divSizeX); }	// ↑の逆…(逆数になる)

	CAnimation*		GetPAnimation(){ return pAnimation; }
	int				GetAnmData(int _anmNo, int _anmCnt);// 指定のアニメーションのパターンから、テクスチャを取得

private:
	TPLPalettePtr		pTexPal;				// 実際にテクスチャを読み込む場所
	STextureFile		tex;					// テクスチャ
	float				divSizeU, divSizeV;		// 1辺の長さ(UV値)
	int					divX, divY;				// 1辺の分割数(画像の枚数)

	// サイズは必須の情報ではないが、あれば便利かも
	int					sizeX, sizeY;			// 画像全体の大きさ(最大1024*1024)
	float				divSizeX, divSizeY;		// 分割後の1テクスチャあたりの大きさ(Size / Div)

	CAnimation			*pAnimation;			// アニメーションをデータを保持する

	// 解放処理は、クラスの解放を持って行う。
	void	ReleaseTexture();
	void	ReleaseAnimation();

public:
	CTexture();
	~CTexture();

};

#endif