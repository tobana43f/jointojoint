//==========================================================
// インクルード
#include	"PlatformSelector.h"
#ifdef _PLATFORM_DIRECTX
#include	"DXVertex.h"
#endif

#include	"System.h"
#include	"Polygon2D.h"

using namespace NSystem;

//==========================================================
// CPolygon2Dクラス
//----------------------------------------------------------
// コンストラクタ
CPolygon2D::CPolygon2D(){

}

//----------------------------------------------------------
// デストラクタ
CPolygon2D::~CPolygon2D(){

}

//----------------------------------------------------------
// 描画
void CPolygon2D::Render(){
	//==========================================================
	// 描画の準備
	SCoord2D	texNo_ = GetTextureNo();
	SCoord2D	texNoOffset_ = GetTextureNoOffset();
	SVector2D	textureDivSizeUV_ = GetTextureDivSizeUV();

	//==========================================================
	// 4頂点の情報を格納
	float x_[CSquarePolygonData::VERTEX_MAX] = {		// 座標は平行移動行列を適用して行う
		0 - GetPCurData()->size.x / 2.0f,
		0 + GetPCurData()->size.x / 2.0f,
		0 + GetPCurData()->size.x / 2.0f,
		0 - GetPCurData()->size.x / 2.0f };
	float y_[CSquarePolygonData::VERTEX_MAX] = {
		0 - GetPCurData()->size.y / 2.0f,
		0 - GetPCurData()->size.y / 2.0f,
		0 + GetPCurData()->size.y / 2.0f,
		0 + GetPCurData()->size.y / 2.0f };
	float tu_[CSquarePolygonData::VERTEX_MAX] = {
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x)),
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x ^ 0x1)),
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x ^ 0x1)),
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x)) };
	float tv_[CSquarePolygonData::VERTEX_MAX] = {
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y)),
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y)),
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y ^ 0x1)),
		textureDivSizeUV_.y * (texNo_.y + (texNoOffset_.y ^ 0x1)),
	};

	SetVertexFormat();		// 頂点フォーマットを指定
	SetRenderState();		// レンダーステートの設定
	SetTexture();			// テクスチャの設定

#ifdef _PLATFORM_DIRECTX
	SDXVertex2D vx_[CSquarePolygonData::VERTEX_MAX];
	D3DXMATRIX	mxRotZ_;
	D3DXMatrixRotationZ(&mxRotZ_, D3DXToRadian(GetAngleDegree().z));	// 回転行列を作成

	// DirectX専用の型に代入
	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		vx_[i].x = x_[i];
		vx_[i].y = y_[i];
		vx_[i].z = 0;
		vx_[i].rhw = 1;

		CSquarePolygonData::EVertexID	idxVx_ = (CSquarePolygonData::EVertexID)i;
		vx_[i].diffuse = D3DCOLOR_RGBA(GetColor(idxVx_).r, GetColor(idxVx_).g, GetColor(idxVx_).b, GetColor(idxVx_).a);	// 色の型をDirectX用に変換する;

		vx_[i].tu = tu_[i];
		vx_[i].tv = tv_[i];
	}

	// Z軸回転と平行移動
	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		D3DXVECTOR3 srcVec_(vx_[i].x, vx_[i].y, vx_[i].z);
		D3DXVECTOR3 destVec_;
		D3DXVec3TransformCoord(&destVec_, &srcVec_, &mxRotZ_);
		vx_[i].x = destVec_.x + GetPositionCenter().x;
		vx_[i].y = destVec_.y + GetPositionCenter().y;
	}

	// 描画
	gpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx_, sizeof(SDXVertex2D));		// ポリゴンを描画
#endif
}


//----------------------------------------------------------
// 頂点フォーマットの設定
void CPolygon2D::SetVertexFormat(){
#ifdef _PLATFORM_DIRECTX
	gpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
#endif
}

SVector3D	CPolygon2D::CalcVertexPosition(CSquarePolygonData::EVertexID _vertex){
	SVector3D	vxPos_ = CBaseSquarePolygon::CalcVertexPosition(_vertex);
	vxPos_.y *= -1;

	return vxPos_;
}