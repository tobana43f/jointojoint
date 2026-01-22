#include	"PlatformSelector.h"
#include	"MathTypeDefinition.h"
#include	"CameraOrthographic.h"
#include	"Math.h"
#include	"System.h"

#ifdef _PLATFORM_DIRECTX
#include	<d3dx9.h>
#endif

using namespace NSystem;
using namespace NMath;

CCameraOrthographic::CCameraOrthographic(){

}

CCameraOrthographic::~CCameraOrthographic(){

}

void CCameraOrthographic::Deploy(){
	SetViewTransform();
	SetProjectionTransform();
}

void CCameraOrthographic::SetViewTransform(){
#ifdef _PLATFORM_DIRECTX
	// DirectX—p‚ÌŒ^‚É•ÏŠ·
	D3DXVECTOR3	camPos_(0, 0, -1);
	D3DXVECTOR3	focusPos_(0, 0, 0);
	D3DXVECTOR3	upDir_(0, 1, 0);
	D3DXMATRIX	mxView_;	// s—ñ‚ðŽæ“¾‚·‚é

	D3DXMatrixLookAtLH(
		&mxView_,
		&camPos_, &focusPos_, &upDir_
		);
	gpD3DDevice->SetTransform(D3DTS_VIEW, &mxView_);
#endif
}

void CCameraOrthographic::SetProjectionTransform(){
#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mxPrj_;	// s—ñ‚ðŽæ“¾‚·‚é

	D3DXMatrixOrthoOffCenterLH( // ³ŽË‰es—ñ‚Ìì¬
		&mxPrj_,
		0,WINDOW_WIDTH,
		WINDOW_HEIGHT,0,
		0.0f,1000.0f
		);

	gpD3DDevice->SetTransform(D3DTS_PROJECTION, &mxPrj_);
#endif
}