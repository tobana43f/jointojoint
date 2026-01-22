#include "PolygonScroll3D.h"
#include "SquarePolygonData.h"
#include	"PlatformSelector.h"
#include	"System.h"
#include	"CameraPerspective.h"
#include	"Math.h"

#ifdef _PLATFORM_DIRECTX
#include	"DXVertex.h"
#endif

using namespace NSystem;
using namespace NMath;

//-----------------
// コンストラクタ
CPolygonScroll3D::CPolygonScroll3D(){
	pCameraPos = NULL;		// カメラの座標ポインタ

	speed = 0;				// スクロール速度

	scroll.x = 0;			// スクロール分
	scroll.y = 0;

	CPolygon3D::SetPositionCenter(0, 0, 100);	// 背景の位置リセット
}

//---------------
// デストラクタ
CPolygonScroll3D::~CPolygonScroll3D(){

}

//-------
// 更新
void CPolygonScroll3D::Update(){
	UpdateBuffer();		// 基本処理
	UpdateAnimation();	// アニメーション

	CalcDistance();		// ポリゴンサイズを合わせる		
	Scroll();			// スクロール
}

//---------------------
// カメラの座標と同期
void CPolygonScroll3D::Sync(){
	// カメラと並行して移動
	SetPositionCenter(pCameraPos->x, pCameraPos->y);
}


//-------------------------------
// カメラとの距離を１度だけ計算
void CPolygonScroll3D::CalcDistance(){
	SVector3D vec = GetPositionCenter();
	distance = vec.z - pCameraPos->z;	// 距離を計算

	SetSize((distance * 2), (distance * 1.2f));	// 倍率でサイズを合わせる
}

//-----------------------
// カメラの移動分を計算
void CPolygonScroll3D::CalcMove(){
	// 差分を計算
	SVector3D now_;

	now_ = GetPositionCenter();	// 現在

	// 差分計算
	moved.x = now_.x - old.x;	// 横にどれだけ移動したか

	// 過去情報を更新
	SetOld();
}

//-------------
// スクロール
void CPolygonScroll3D::Scroll(){
	// 前フレームからの移動量を計算
	CalcMove();

	// スクロース計算
	scroll.x = (speed * moved.x) + scroll.x;
}

//-------
// 描画
void CPolygonScroll3D::Render(){
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
		0 + GetPCurData()->size.y / 2.0f,
		0 + GetPCurData()->size.y / 2.0f,
		0 - GetPCurData()->size.y / 2.0f,
		0 - GetPCurData()->size.y / 2.0f };
	float z_[CSquarePolygonData::VERTEX_MAX] = {
		0,
		0,
		0,
		0 };
	float tu_[CSquarePolygonData::VERTEX_MAX] = {
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x)) + scroll.x,
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x ^ 0x1)) + scroll.x,
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x ^ 0x1)) + scroll.x,
		textureDivSizeUV_.x * (texNo_.x + (texNoOffset_.x)) + scroll.x };
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
	SDXVertex3D	vx_[CSquarePolygonData::VERTEX_MAX];
	D3DXMATRIX	mxRotX_, mxRotY_, mxRotZ_, mxRot_;
	D3DXMATRIX	mxTrans_;
	D3DXMATRIX	mxWorld_;

	D3DXMatrixRotationX(&mxRotX_, D3DXToRadian(GetAngleDegree().x));	// X軸回転行列を作成
	D3DXMatrixRotationY(&mxRotY_, D3DXToRadian(GetAngleDegree().y));	// Y軸回転行列を作成
	D3DXMatrixRotationZ(&mxRotZ_, D3DXToRadian(GetAngleDegree().z));	// Z軸回転行列を作成
	mxRot_ = mxRotX_ * mxRotY_* mxRotZ_;						// 各軸の回転行列を合成

	D3DXMatrixTranslation(										// 平行移動行列の作成
		&mxTrans_,
		GetPositionCenter().x, GetPositionCenter().y, GetPositionCenter().z
		);

	mxWorld_ = mxRot_ * mxTrans_;								// ワールド変換行列を作成

	NSystem::gpD3DDevice->SetTransform(D3DTS_WORLD, &mxWorld_);	// ワールド変換行列を適用

	// DirectX専用の型に代入
	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		vx_[i].x = x_[i];
		vx_[i].y = y_[i];
		vx_[i].z = z_[i];

		CSquarePolygonData::EVertexID	idxVx_ = (CSquarePolygonData::EVertexID)i;
		vx_[i].diffuse = D3DCOLOR_RGBA(GetColor(idxVx_).r, GetColor(idxVx_).g, GetColor(idxVx_).b, GetColor(idxVx_).a);	// 色の型をDirectX用に変換する;

		vx_[i].tu = tu_[i];
		vx_[i].tv = tv_[i];
	}
	// 描画
	NSystem::gpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx_, sizeof(SDXVertex3D));		// ポリゴンを描画
#endif
}
