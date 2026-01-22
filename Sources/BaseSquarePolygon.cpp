#include	"PlatformSelector.h"		
#include	"BaseSquarePolygon.h"
#include	"System.h"					// D3Dデバイスを使用する
#include	"Math.h"

using namespace NSystem;
using namespace NMath;

//==========================================================
// メソッド定義
CBaseSquarePolygon::CBaseSquarePolygon(){
	Init();
}

CBaseSquarePolygon::CBaseSquarePolygon(CSquarePolygonData _src){
	Init();
	SetPositionCenter(_src.positionCenter.x, _src.positionCenter.y, _src.positionCenter.z);
	SetAngleDegree(_src.angleDegree.x, _src.angleDegree.y, _src.angleDegree.z);
	SetSize(_src.size.x, _src.size.y);

	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		CSquarePolygonData::EVertexID idxVx_ = (CSquarePolygonData::EVertexID)i;
		SetColorToVertex(idxVx_, _src.color[idxVx_].r, _src.color[idxVx_].g, _src.color[idxVx_].b, _src.color[idxVx_].a);
	}

	SetReferenceTexture(_src.pTexture);
	GetPCurData()->anmCnt = _src.anmCnt;
	GetPCurData()->anmNo = _src.anmNo;
	GetPCurData()->isAnmEnd = _src.isAnmEnd;

	SetIsReverseX(_src.isReverseX);
	SetIsReverseY(_src.isReverseY);

	SetBlendType(_src.blendType);
}

CBaseSquarePolygon::~CBaseSquarePolygon(){
	Destroy();
}

void CBaseSquarePolygon::Init(){
	InitBuffer();
}

void CBaseSquarePolygon::Destroy(){
	SetReferenceTexture(NULL);
}

void CBaseSquarePolygon::Update(){
	UpdateBuffer();
	UpdateAnimation();
}

// _a : 指定しなければ 0 を加算し、変化させない
void CBaseSquarePolygon::AddColor(int _r, int _g, int _b, int _a){
	int r_, g_, b_, a_;

	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		r_ = GetPCurData()->color[i].r + _r;
		g_ = GetPCurData()->color[i].g + _g;
		b_ = GetPCurData()->color[i].b + _b;
		a_ = GetPCurData()->color[i].a + _a;

		if (r_ > 255)		r_ = 255;
		else if (r_ < 0)	r_ = 0;
		if (g_ > 255)		g_ = 255;
		else if (g_ < 0)	g_ = 0;
		if (b_ > 255)		b_ = 255;
		else if (b_ < 0)	b_ = 0;
		if (a_ > 255)		a_ = 255;
		else if (a_ < 0)	a_ = 0;

		GetPCurData()->color[i].r = r_;
		GetPCurData()->color[i].g = g_;
		GetPCurData()->color[i].b = b_;
		GetPCurData()->color[i].a = a_;
	}
}

// 指定した頂点に色を設定する
// 引数
// 　_a : 指定しなければ 0
void CBaseSquarePolygon::AddColorToVertex(CSquarePolygonData::EVertexID _vxId, int _r, int _g, int _b, int _a){
	int r_, g_, b_, a_;

	if (_vxId >= CSquarePolygonData::VERTEX_MAX || _vxId < 0){
		// error
		return;
	}

	r_ = GetPCurData()->color[_vxId].r + _r;
	g_ = GetPCurData()->color[_vxId].g + _g;
	b_ = GetPCurData()->color[_vxId].b + _b;
	a_ = GetPCurData()->color[_vxId].a + _a;

	if (r_ > 255)		r_ = 255;
	else if (r_ < 0)	r_ = 0;
	if (g_ > 255)		g_ = 255;
	else if (g_ < 0)	g_ = 0;
	if (b_ > 255)		b_ = 255;
	else if (b_ < 0)	b_ = 0;
	if (a_ > 255)		a_ = 255;
	else if (a_ < 0)	a_ = 0;

	GetPCurData()->color[_vxId].r = r_;
	GetPCurData()->color[_vxId].g = g_;
	GetPCurData()->color[_vxId].b = b_;
	GetPCurData()->color[_vxId].a = a_;
}

void CBaseSquarePolygon::SetPositionLeftTop(float _x, float _y, float _z){
	SVector3D pos_;
	pos_.x = _x + GetSize().x / 2.0f;
	pos_.y = _y - GetSize().y / 2.0f;
	pos_.z = _z;

	SetPositionCenter(pos_.x, pos_.y, pos_.z);
}


// 全頂点に共通の色を設定する
// 引数
// 　_a : 指定しなければ 255
void CBaseSquarePolygon::SetColor(int _r, int _g, int _b, int _a){
	if (_r > 255)		_r = 255;
	else if (_r < 0)	_r = 0;
	if (_g > 255)		_g = 255;
	else if (_g < 0)	_g = 0;
	if (_b > 255)		_b = 255;
	else if (_b < 0)	_b = 0;
	if (_a > 255)		_a = 255;
	else if (_a < 0)	_a = 0;

	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		GetPCurData()->color[i].r = _r;
		GetPCurData()->color[i].g = _g;
		GetPCurData()->color[i].b = _b;
		GetPCurData()->color[i].a = _a;
	}
}

// 指定した頂点に色を設定する
// 引数
// 　_a : 指定しなければ 255
void CBaseSquarePolygon::SetColorToVertex(CSquarePolygonData::EVertexID _vxId, int _r, int _g, int _b, int _a){
	if (_r > 255)		_r = 255;
	else if (_r < 0)	_r = 0;
	if (_g > 255)		_g = 255;
	else if (_g < 0)	_g = 0;
	if (_b > 255)		_b = 255;
	else if (_b < 0)	_b = 0;
	if (_a > 255)		_a = 255;
	else if (_a < 0)	_a = 0;

	if (_vxId >= CSquarePolygonData::VERTEX_MAX || _vxId < 0){
		// error
		return;
	}

	GetPCurData()->color[_vxId].r = _r;
	GetPCurData()->color[_vxId].g = _g;
	GetPCurData()->color[_vxId].b = _b;
	GetPCurData()->color[_vxId].a = _a;
}

// アニメーションの指定
// メモ)
// 　指定と同時に、アニメーションのカウンタもリセットされる。
// メモ2)
// 　基本的には、自由な値を入れるが
// 　アニメーション情報が設定されている場合は、エラー処理をする
void CBaseSquarePolygon::SetAnmNo(int _anmNo){
	GetPCurData()->anmCnt = 0;			// カウンタは無条件でリセット
	GetPCurData()->anmNo = _anmNo;

	// テクスチャ、テクスチャアニメーションがない場合はここで終わり
	if (GetPTexture() == NULL || GetPTexture()->GetPAnimation() == NULL)
		return;

	// 指定したアニメーションが、アニメーションの範囲ではない場合はエラー処理をする
	if (GetPCurData()->anmNo > GetPTexture()->GetPAnimation()->GetAnmNum() || GetPCurData()->anmNo < 0){
		// error 
		GetPCurData()->anmNo = 0;// 仮) 一応エラーが起こらないように
	}
}

SVector3D CBaseSquarePolygon::CalcVertexPosition(int _vertex){
	if (_vertex < CSquarePolygonData::VERTEX_0 || _vertex >= CSquarePolygonData::VERTEX_MAX){
		// error
		return SVector3D(0, 0, 0);		// 無効な値を返す
	}

	SMatrix44	mxRotX_, mxRotY_, mxRotZ_, mxRot_;
	mxRotX_ = CalcMatrixRotateX(ConvertDegToRad(GetAngleDegree().x));	// X軸回転行列を作成
	mxRotY_ = CalcMatrixRotateY(ConvertDegToRad(GetAngleDegree().y));	// Y軸回転行列を作成
	mxRotZ_ = CalcMatrixRotateZ(ConvertDegToRad(-GetAngleDegree().z));	// Z軸回転行列を作成
	mxRot_ = mxRotX_ * mxRotY_* mxRotZ_;								// 各軸の回転行列を合成

	float x_[CSquarePolygonData::VERTEX_MAX] = {		// 座標は平行移動行列を適用して行う
		0 - GetPCurData()->size.x / 2.0f,
		0 + GetPCurData()->size.x / 2.0f,
		0 + GetPCurData()->size.x / 2.0f,
		0 - GetPCurData()->size.x / 2.0f };
	float y_[CSquarePolygonData::VERTEX_MAX] = {
		0 + GetPCurData()->size.y / 2.0f,
		0 + GetPCurData()->size.y / 2.0f,
		0 - GetPCurData()->size.y / 2.0f,
		0 - GetPCurData()->size.y / 2.0f };
	float z_[CSquarePolygonData::VERTEX_MAX] = {
		0,
		0,
		0,
		0
	};

	// 求める頂点に、回転行列を適用する
	SVector3D	srcVec_(x_[_vertex], y_[_vertex], z_[_vertex]);		// 適用前の座標
	SVector3D	destVec_ = CalcVector3DTransform(&mxRot_, &srcVec_);// 適用後の座標
	SVector3D	vxPos_;	// 求めるワールド座標
	vxPos_.x = destVec_.x + GetPositionCenter().x;
	vxPos_.y = destVec_.y + GetPositionCenter().y;
	vxPos_.z = destVec_.z + GetPositionCenter().z;

	return vxPos_;
}

SCoord2D CBaseSquarePolygon::GetTextureNo(){
	SCoord2D	textureNo_;
	textureNo_.x = 0;
	textureNo_.y = 0;

	// アニメーション付きテクスチャのテクスチャ番号を取得する
	if (GetPTexture() != NULL && GetPTexture()->GetPAnimation() != NULL){
		textureNo_.x = GetPTexture()->GetAnmData(GetPCurData()->anmNo, GetPCurData()->anmCnt) % GetPTexture()->GetDivX();		// 横の数での剰余
		textureNo_.y = GetPTexture()->GetAnmData(GetPCurData()->anmNo, GetPCurData()->anmCnt) / GetPTexture()->GetDivX();		// 横の数の商
	}
	// アニメーションがない場合は指定したAnmNoを使用して指定できるようにする。
	else if (GetPTexture() != NULL){
		textureNo_.x = GetPCurData()->anmNo % GetPTexture()->GetDivX();
		textureNo_.y = GetPCurData()->anmNo / GetPTexture()->GetDivX();
	}

	return textureNo_;
}

// 戻り値は 0か1 のみ
SCoord2D CBaseSquarePolygon::GetTextureNoOffset(){
	SCoord2D	textureNoOffset_;

	// X
	if (GetPCurData()->isReverseX)
		textureNoOffset_.x = 1;
	else
		textureNoOffset_.x = 0;

	// Y
	if (GetPCurData()->isReverseY)
		textureNoOffset_.y = 1;
	else
		textureNoOffset_.y = 0;

	return textureNoOffset_;
}

// テクスチャのポインタがセットされているかどうかを判断するだけ
// 0~1の範囲
SVector2D	CBaseSquarePolygon::GetTextureDivSizeUV(){
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

void CBaseSquarePolygon::InitBuffer(){
	for (int i = 0; i < DATA_BUFFER_SIZE; ++i){
		bufData[i].Init();
	}
}

void CBaseSquarePolygon::UpdateBuffer(){
	// 情報を更新
	for (int i = DATA_BUFFER_SIZE - 1; i > 0; --i){
		bufData[i] = bufData[i - 1];
	}
}

void CBaseSquarePolygon::UpdateAnimation(){
	// テクスチャのアニメーションがない場合は更新しない
	if (GetPTexture() == NULL || GetPTexture()->GetPAnimation() == NULL)
		return;

	GetPCurData()->isAnmEnd = false;		// 更新状態にする
	GetPCurData()->anmCnt++;				// 次のアニメーションへ
	if (GetPTexture()->GetAnmData(GetPCurData()->anmNo, GetPCurData()->anmCnt) == CAnimation::ANM_LOOP){
		GetPCurData()->anmCnt = 0;			// 最初に戻す
	}
	else if (GetPTexture()->GetAnmData(GetPCurData()->anmNo, GetPCurData()->anmCnt) == CAnimation::ANM_END){
		GetPCurData()->anmCnt--;			// 一つ前(一番最後)の状態に戻す
		GetPCurData()->isAnmEnd = true;	// 終了状態にする。
	}
}

void CBaseSquarePolygon::SetTexture(){
#ifdef _PLATFORM_DIRECTX
	// テクスチャを設定
	if (GetPTexture() != NULL)
		gpD3DDevice->SetTexture(0, GetPTexture()->GetTexture());									// テクスチャを設定
	else
		gpD3DDevice->SetTexture(0, NULL);
#endif
}

void CBaseSquarePolygon::SetRenderState(){
	switch (GetPCurData()->blendType){
		case CSquarePolygonData::REPLACE:
			SetRenderStateReplace();
			break;

		case CSquarePolygonData::ADD:
			SetRenderStateAdd();
			break;

		case CSquarePolygonData::SUB:
			SetRenderStateSub();
			break;
	}
}

void CBaseSquarePolygon::SetRenderStateReplace(){
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

void CBaseSquarePolygon::SetRenderStateAdd(){
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

void CBaseSquarePolygon::SetRenderStateSub(){
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