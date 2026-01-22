//==========================================================
// インクルード
#include	"PlatformSelector.h"
#include	"PolygonLine.h"
#include	"System.h"
#include	"Math.h"
#include	"CameraPerspective.h"

#include	<math.h>

#ifdef _PLATFORM_DIRECTX
#include	"DXVertex.h"
#endif

//==========================================================
// 名前空間
using namespace NSystem;
using namespace NMath;

//==========================================================
// メソッド定義

CPolygonLine::CPolygonLine(){
	Init();
}

CPolygonLine::~CPolygonLine(){
	Destroy();
}

void CPolygonLine::Init(){
	// ポリゴン情報
	SetPositionStart(0, 0, 0);
	SetPositionEnd(0, 0, 0);
	SetLineWidth(1);
	SetColor(255, 255, 255);
	SetReferenceTexture(NULL);

	// アニメーション
	anmCnt = 0;
	anmNo = 0;
	isReverseX = false;
	isReverseY = false;
	isAnmEnd = false;
	
	// ブレンドタイプ
	SetBlendType(REPLACE);
}

void CPolygonLine::Destroy(){
	SetReferenceTexture(NULL);
}

void CPolygonLine::Update(){
	UpdateAnimation();
}

void CPolygonLine::Render(){
	//==========================================================
	// 描画の準備
	SCoord2D	texNo_ = GetTextureNo();
	SCoord2D	texNoOffset_ = GetTextureNoOffset();
	SVector2D	textureDivSizeUV_ = GetTextureDivSizeUV();

	//==========================================================
	// 4頂点の情報を格納
	float lineHalfWidth_ = lineWidth / 2.0f;
	float direction_ = atan2f(positionEnd.y - positionStart.y, positionEnd.x - positionStart.x);
	direction_ -= ConvertDegToRad(90);	// 傾きに直角にする

	float x_[VERTEX_MAX] = {		// 座標は平行移動行列を適用して行う
		positionStart.x - lineHalfWidth_ * cosf(direction_),
		positionStart.x + lineHalfWidth_ * cosf(direction_),
		positionEnd.x + lineHalfWidth_ * cosf(direction_),
		positionEnd.x - lineHalfWidth_ * cosf(direction_) };
	float y_[VERTEX_MAX] = {
		positionStart.y - lineHalfWidth_ * sinf(direction_),
		positionStart.y + lineHalfWidth_ * sinf(direction_),
		positionEnd.y + lineHalfWidth_ * sinf(direction_),
		positionEnd.y - lineHalfWidth_ * sinf(direction_) };
	float z_[VERTEX_MAX] = {
		0,
		0,
		0,
		0 };
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
	SDXVertex3D	vx_[VERTEX_MAX];
	D3DXMATRIX	mxWorld_;	// 

	D3DXMatrixIdentity(&mxWorld_);
	gpD3DDevice->SetTransform(D3DTS_WORLD, &mxWorld_);

	// DirectX専用の型に代入
	for (int i = 0; i < VERTEX_MAX; ++i){
		vx_[i].x = x_[i];
		vx_[i].y = y_[i];
		vx_[i].z = z_[i];
		vx_[i].diffuse = D3DCOLOR_RGBA(color[i].r, color[i].g, color[i].b, color[i].a);	// 色の型をDirectX用に変換する;
		vx_[i].tu = tu_[i];
		vx_[i].tv = tv_[i];
	}
	// 描画
	NSystem::gpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx_, sizeof(SDXVertex3D));		// ポリゴンを描画
#endif
}


// _a : 指定しなければ 0 を加算し、変化させない
void CPolygonLine::AddColor(int _r, int _g, int _b, int _a){
	int r_, g_, b_, a_;

	for (int i = 0; i < VERTEX_MAX; ++i){
		r_ = color[i].r + _r;
		g_ = color[i].g + _g;
		b_ = color[i].b + _b;
		a_ = color[i].a + _a;

		if (r_ > 255)		r_ = 255;
		else if (r_ < 0)	r_ = 0;
		if (g_ > 255)		g_ = 255;
		else if (g_ < 0)	g_ = 0;
		if (b_ > 255)		b_ = 255;
		else if (b_ < 0)	b_ = 0;
		if (a_ > 255)		a_ = 255;
		else if (a_ < 0)	a_ = 0;

		color[i].r = r_;
		color[i].g = g_;
		color[i].b = b_;
		color[i].a = a_;
	}
}


// 指定した頂点に色を設定する
// 引数
// 　_a : 指定しなければ 0
void CPolygonLine::AddColorToVertex(EVertexID _vxId, int _r, int _g, int _b, int _a){
	int r_, g_, b_, a_;

	if (_vxId >= VERTEX_MAX || _vxId < 0){
		// error
		return;
	}

	r_ = color[_vxId].r + _r;
	g_ = color[_vxId].g + _g;
	b_ = color[_vxId].b + _b;
	a_ = color[_vxId].a + _a;

	if (r_ > 255)		r_ = 255;
	else if (r_ < 0)	r_ = 0;
	if (g_ > 255)		g_ = 255;
	else if (g_ < 0)	g_ = 0;
	if (b_ > 255)		b_ = 255;
	else if (b_ < 0)	b_ = 0;
	if (a_ > 255)		a_ = 255;
	else if (a_ < 0)	a_ = 0;

	color[_vxId].r = r_;
	color[_vxId].g = g_;
	color[_vxId].b = b_;
	color[_vxId].a = a_;
}


// 全頂点に共通の色を設定する
// 引数
// 　_a : 指定しなければ 255
void CPolygonLine::SetColor(int _r, int _g, int _b, int _a){
	if (_r > 255)		_r = 255;
	else if (_r < 0)	_r = 0;
	if (_g > 255)		_g = 255;
	else if (_g < 0)	_g = 0;
	if (_b > 255)		_b = 255;
	else if (_b < 0)	_b = 0;
	if (_a > 255)		_a = 255;
	else if (_a < 0)	_a = 0;

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
void CPolygonLine::SetColorToVertex(EVertexID _vxId, int _r, int _g, int _b, int _a){
	if (_r > 255)		_r = 255;
	else if (_r < 0)	_r = 0;
	if (_g > 255)		_g = 255;
	else if (_g < 0)	_g = 0;
	if (_b > 255)		_b = 255;
	else if (_b < 0)	_b = 0;
	if (_a > 255)		_a = 255;
	else if (_a < 0)	_a = 0;

	if (_vxId >= VERTEX_MAX || _vxId < 0){
		// error
		return;
	}

	color[_vxId].r = _r;
	color[_vxId].g = _g;
	color[_vxId].b = _b;
	color[_vxId].a = _a;
}

// アニメーションの指定
// メモ)
// 　指定と同時に、アニメーションのカウンタもリセットされる。
// メモ2)
// 　基本的には、自由な値を入れるが
// 　アニメーション情報が設定されている場合は、エラー処理をする
void CPolygonLine::SetAnmNo(int _anmNo){
	anmCnt = 0;			// カウンタは無条件でリセット
	anmNo = _anmNo;

	// テクスチャ、テクスチャアニメーションがない場合はここで終わり
	if (GetPTexture() == NULL || GetPTexture()->GetPAnimation() == NULL)
		return;

	// 指定したアニメーションが、アニメーションの範囲ではない場合はエラー処理をする
	if (anmNo > GetPTexture()->GetPAnimation()->GetAnmNum() || anmNo < 0){
		// error 
		anmNo = 0;// 仮) 一応エラーが起こらないように
	}
}

SVector3D	CPolygonLine::CalcVertexPosition(int _vertex){
	if (_vertex < VERTEX_0 || _vertex >= VERTEX_MAX){
		//error
		return SVector3D(0, 0, 0);
	}

	//==========================================================
	// 4頂点の情報を格納
	float lineHalfWidth_ = lineWidth / 2.0f;
	float direction_ = atan2f(positionEnd.y - positionStart.y, positionEnd.x - positionStart.x);
	direction_ -= ConvertDegToRad(90);	// 傾きに直角にする

	float x_[VERTEX_MAX] = {		// 座標は平行移動行列を適用して行う
		positionStart.x - lineHalfWidth_ * cosf(direction_),
		positionStart.x + lineHalfWidth_ * cosf(direction_),
		positionEnd.x + lineHalfWidth_ * cosf(direction_),
		positionEnd.x - lineHalfWidth_ * cosf(direction_) };
	float y_[VERTEX_MAX] = {
		positionStart.y - lineHalfWidth_ * sinf(direction_),
		positionStart.y + lineHalfWidth_ * sinf(direction_),
		positionEnd.y + lineHalfWidth_ * sinf(direction_),
		positionEnd.y - lineHalfWidth_ * sinf(direction_) };
	float z_[VERTEX_MAX] = {
		0,
		0,
		0,
		0 };

	// 結果を格納
	SVector3D	vxPos_;
	vxPos_ = SVector3D(x_[_vertex], y_[_vertex], z_[_vertex]);

	return vxPos_;
}

SCoord2D CPolygonLine::GetTextureNo(){
	SCoord2D	textureNo_;
	textureNo_.x = 0;
	textureNo_.y = 0;

	// アニメーション付きテクスチャのテクスチャ番号を取得する
	if (GetPTexture() != NULL && GetPTexture()->GetPAnimation() != NULL){
		textureNo_.x = GetPTexture()->GetAnmData(anmNo, anmCnt) % GetPTexture()->GetDivX();		// 横の数での剰余
		textureNo_.y = GetPTexture()->GetAnmData(anmNo, anmCnt) / GetPTexture()->GetDivX();		// 横の数の商
	}
	// アニメーションがない場合は指定したAnmNoを使用して指定できるようにする。
	else if (GetPTexture() != NULL){
		textureNo_.x = anmNo % GetPTexture()->GetDivX();
		textureNo_.y = anmNo / GetPTexture()->GetDivX();
	}

	return textureNo_;
}

// 戻り値は 0か1 のみ
SCoord2D CPolygonLine::GetTextureNoOffset(){
	SCoord2D	textureNoOffset_;

	// X
	if (isReverseX)
		textureNoOffset_.x = 1;
	else
		textureNoOffset_.x = 0;

	// Y
	if (isReverseY)
		textureNoOffset_.y = 1;
	else
		textureNoOffset_.y = 0;

	return textureNoOffset_;
}

// テクスチャのポインタがセットされているかどうかを判断するだけ
// 0~1の範囲
SVector2D	CPolygonLine::GetTextureDivSizeUV(){
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

void CPolygonLine::UpdateAnimation(){
	// テクスチャのアニメーションがない場合は更新しない
	if (GetPTexture() == NULL || GetPTexture()->GetPAnimation() == NULL)
		return;

	isAnmEnd = false;		// 更新状態にする
	anmCnt++;				// 次のアニメーションへ
	if (GetPTexture()->GetAnmData(anmNo, anmCnt) == CAnimation::ANM_LOOP){
		anmCnt = 0;			// 最初に戻す
	}
	else if (GetPTexture()->GetAnmData(anmNo, anmCnt) == CAnimation::ANM_END){
		anmCnt--;			// 一つ前(一番最後)の状態に戻す
		isAnmEnd = true;	// 終了状態にする。
	}
}

void CPolygonLine::SetVertexFormat(){
#ifdef _PLATFORM_DIRECTX
	NSystem::gpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);				// 頂点フォーマットを指定
#endif
}

void CPolygonLine::SetTexture(){
#ifdef _PLATFORM_DIRECTX
	// テクスチャを設定
	if (GetPTexture() != NULL)
		gpD3DDevice->SetTexture(0, GetPTexture()->GetTexture());									// テクスチャを設定
	else
		gpD3DDevice->SetTexture(0, NULL);
#endif
}

void CPolygonLine::SetRenderState(){
	switch (blendType){
		case REPLACE:
			SetRenderStateReplace();
			break;

		case ADD:
			SetRenderStateAdd();
			break;

		case SUB:
			SetRenderStateSub();
			break;
	}
}

void CPolygonLine::SetRenderStateReplace(){
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

void CPolygonLine::SetRenderStateAdd(){
#ifdef _PLATFORM_DIRECTX
	gpD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	gpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	gpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	gpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	gpD3DDevice->LightEnable(0, FALSE);

	gpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	gpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	gpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
#endif
}

void CPolygonLine::SetRenderStateSub(){
#ifdef _PLATFORM_DIRECTX
	gpD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	gpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	gpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	gpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);

	gpD3DDevice->LightEnable(0, FALSE);

	gpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	gpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	gpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
#endif
}

