#define		_CRT_SECURE_NO_WARNINGS
#include	"PlatformSelector.h"
#include	"PolygonText2D.h"	// 定義
#include	"System.h"

#include	<stdio.h>					// sprintf , vsprintfなどを使うため	
#include	<stdarg.h>					// 可変引数を利用した関数を作るため

#ifdef _PLATFORM_DIRECTX
#include	"DXVertex.h"
#endif

using namespace NSystem;

CPolygonText2D::CPolygonText2D(){
	Init();
}

CPolygonText2D::~CPolygonText2D(){
	Destroy();
}

void CPolygonText2D::Init(){
	SetPositionLeftTop(0, 0);
	SetSize(0, 0);
	SetColor(255, 255, 255, 255);
	SetReferenceTexture(NULL);
}

void CPolygonText2D::Destroy(){
	SetReferenceTexture(NULL);
}

// 注意)
// 　合計 255 文字(Byte)の文字列しか描画できない
// 　それ以上する場合は、配列の大きさを変更してください。
void CPolygonText2D::Render(const char *_format, ...){
	va_list arg_;
	char buf_[256] = { "\0" };			// 動的確保だと、フォーマットと置き換える前のサイズでとってしまうので、置き換えたときにバッファが足りなくなる。

	va_start(arg_, _format);
	vsprintf(buf_, _format, arg_);
	va_end(arg_);

	// 全文字列を描画
	for (int i = 0; buf_[i] != '\0'; ++i){
		RenderLetter(i, buf_[i]);
	}
}

// _a : 指定しなければ 0 を加算し、変化させない
void CPolygonText2D::AddColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
	for (int i = 0; i < VERTEX_MAX; ++i){
		color[i].r += _r;
		color[i].g += _g;
		color[i].b += _b;
		color[i].a += _a;
	}
}


// 指定した頂点に色を設定する
// 引数
// 　_a : 指定しなければ 0
void CPolygonText2D::AddColorToVertex(EVertexID _vxId, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
	if (_vxId >= VERTEX_MAX || _vxId < 0){
		// error
		return;
	}

	color[_vxId].r += _r;
	color[_vxId].g += _g;
	color[_vxId].b += _b;
	color[_vxId].a += _a;
}

// 全頂点に共通の色を設定する
// 引数
// 　_a : 指定しなければ 255
void CPolygonText2D::SetColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
	for (int i = 0; i < VERTEX_MAX; ++i){
		color[i].r = _r;
		color[i].g = _g;
		color[i].b = _b;
		color[i].a = _a;
	}
}

// 指定した頂点に色を設定する
// 引数
// 　_a : 指定しなければ 255
void CPolygonText2D::SetColorToVertex(EVertexID _vxId, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
	if (_vxId >= VERTEX_MAX || _vxId < 0){
		// error
		return;
	}

	color[_vxId].r = _r;
	color[_vxId].g = _g;
	color[_vxId].b = _b;
	color[_vxId].a = _a;
}

void CPolygonText2D::SetTexture(){
#ifdef _PLATFORM_DIRECTX
	// テクスチャを設定
	if (GetPTexture() != NULL)
		gpD3DDevice->SetTexture(0, GetPTexture()->GetTexture());									// テクスチャを設定
	else
		gpD3DDevice->SetTexture(0, NULL);
#endif
}

//----------------------------------------------------------
// 1文字を描画する関数(Polygon2DのRenderと同じ)
// 引数
// 　_strIndex	: 描画する文字が何文字目か。(文字列配列の添え字)
// 　_char		: 描画する文字。
void CPolygonText2D::RenderLetter(int _strIndex, char _char){
	//==========================================================
	// 描画の準備
	SCoord2D	texNo_ = GetTextureNo(_char);
	SCoord2D	texNoOffset_ = GetTextureNoOffset();
	SVector2D	textureDivSizeUV_ = GetTextureDivSizeUV();

	//==========================================================
	// 4頂点の情報を格納
	float x_[VERTEX_MAX] = {		// 座標は平行移動行列を適用して行う
		0,
		sizeLetter.x,
		sizeLetter.x,
		0,
	};
	float y_[VERTEX_MAX] = {
		0,
		0,
		sizeLetter.y,
		sizeLetter.y
	};
	float tu_[VERTEX_MAX] = {
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x)),
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x ^ 0x1)),
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x ^ 0x1)),
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x)) };
	float tv_[VERTEX_MAX] = {
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y)),
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y)),
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y ^ 0x1)),
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y ^ 0x1)),
	};

	SetVertexFormat();		// 頂点フォーマットを指定
	SetRenderState();		// レンダーステートの設定
	SetTexture();			// テクスチャの設定

#ifdef _PLATFORM_DIRECTX
	SDXVertex2D vx_[VERTEX_MAX];

	// DirectX専用の型に代入
	for (int i = 0; i < VERTEX_MAX; ++i){
		vx_[i].x = x_[i];
		vx_[i].y = y_[i];
		vx_[i].z = 0;
		vx_[i].rhw = 1;
		vx_[i].diffuse = D3DCOLOR_RGBA(color[i].r, color[i].g, color[i].b, color[i].a);	// 色の型をDirectX用に変換する;
		vx_[i].tu = tu_[i];
		vx_[i].tv = tv_[i];
	}

	// 平行移動
	for (int i = 0; i < VERTEX_MAX; ++i){
		vx_[i].x += positionLeftTop.x + (sizeLetter.x * _strIndex);
		vx_[i].y += positionLeftTop.y;
	}

	// 描画
	gpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx_, sizeof(SDXVertex2D));		// ポリゴンを描画
#endif
}

//----------------------------------------------------------
// 頂点フォーマットの設定
void CPolygonText2D::SetVertexFormat(){
#ifdef _PLATFORM_DIRECTX
	gpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
#endif
}

//----------------------------------------------------------
// レンダーステートの設定
void CPolygonText2D::SetRenderState(){
#ifdef _PLATFORM_DIRECTX
	gpD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	gpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	gpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	gpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	gpD3DDevice->LightEnable(0, FALSE);

	gpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	gpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	gpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
#endif
}


SCoord2D CPolygonText2D::GetTextureNo(char _char){
	SCoord2D	textureNo_;
	int			fontNo_ = _char - ' ';	// その文字のテクスチャ連番番号。

	textureNo_.x = 0;
	textureNo_.y = 0;

	// アニメーション付きテクスチャのテクスチャ番号を取得する
	if (GetPTexture() != NULL){
		textureNo_.x = fontNo_ % GetPTexture()->GetDivX();		// 横の数での剰余
		textureNo_.y = fontNo_ / GetPTexture()->GetDivX();		// 横の数の商
	}

	return textureNo_;
}

// 戻り値は 0か1 のみ
SCoord2D CPolygonText2D::GetTextureNoOffset(){
	SCoord2D	textureNoOffset_;
	textureNoOffset_.x = 0;
	textureNoOffset_.y = 0;

	return textureNoOffset_;
}

// テクスチャのポインタがセットされているかどうかを判断するだけ
// 0~1の範囲
SVector2D	CPolygonText2D::GetTextureDivSizeUV(){
	SVector2D	textureDivSizeUV_;	// UVサイズ
	textureDivSizeUV_.x = 1.0f;
	textureDivSizeUV_.y = 1.0f;

	// テクスチャがある場合、テクスチャの分割後のUVを取得
	if (GetPTexture() != NULL){
		textureDivSizeUV_.x = GetPTexture()->GetDivSizeU();
		textureDivSizeUV_.y = GetPTexture()->GetDivSizeV();
	}

	return textureDivSizeUV_;
}