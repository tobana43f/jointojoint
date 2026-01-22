#include	"PlatformSelector.h"
#include	"MathTypeDefinition.h"
#include	"CameraPerspective.h"	// クラスの定義
#include	"Math.h"	// 数学関係の演算を使用
#include	"System.h"	// DirectXのデバイスの仕様と、画面サイズの参照

#ifdef _PLATFORM_DIRECTX
#include	<d3dx9.h>
#endif

using namespace NSystem;
using namespace NMath;

SMatrix44 *CCameraPerspective::gpCurrentViewMatrix;

CCameraPerspective::CCameraPerspective(){
	// 適当な値で初期化する
	SetCameraPosition(0, 0, 0);
	SetFocusPosition(0, 0, 0);
	SetUpDirection(0, 1, 0);
	SetViewAngleDeg(45);
	SetScreenAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
	SetClipDistance(10, 100);
}

CCameraPerspective::~CCameraPerspective(){

}

// カメラを3D空間上に配置する。
// 射影変換をする準備をする。
// 注意) 3Dポリゴンの描画を開始する直前に1度、必ず呼び出す。
void CCameraPerspective::Deploy(){
	SetViewTransform();
	SetProjectionTransform();
	SetViewPort();
	
	gpCurrentViewMatrix = &mxView;	// 現在使用しているビュー変換行列を登録
}

void CCameraPerspective::SetViewTransform(){
#ifdef _PLATFORM_DIRECTX
	// DirectX用の型に変換
	D3DXVECTOR3	camPos_(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	D3DXVECTOR3	focusPos_(focusPosition.x, focusPosition.y, focusPosition.z);
	D3DXVECTOR3	upDir_(upDirection.x, upDirection.y, upDirection.z);
	D3DXMATRIX	mxView_;	// 行列を取得する

	D3DXMatrixLookAtLH(
		&mxView_,
		&camPos_, &focusPos_, &upDir_
		);
	gpD3DDevice->SetTransform(D3DTS_VIEW, &mxView_);

	// 作ったビュー変換行列を保持
	ConvertShareTypeMatrixToSMatrix44(&mxView, &mxView_);

#endif
}

void CCameraPerspective::SetProjectionTransform(){
#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mxPrj_;

	D3DXMatrixPerspectiveFovLH(
		&mxPrj_,
		ConvertDegToRad(viewAngleDeg),
		screenAspectRatio,
		nearClip,
		farClip);
	gpD3DDevice->SetTransform(D3DTS_PROJECTION, &mxPrj_);

	// 作った行列を保持
	ConvertShareTypeMatrixToSMatrix44(&mxProjection, &mxPrj_);
#endif
}

void CCameraPerspective::SetViewPort(){
#ifdef _PLATFORM_DIRECTX

#endif
}