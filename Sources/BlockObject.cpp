//==========================================================
// インクルード
#include	"BlockObject.h"

//==========================================================
// メソッド定義

void CBlockObject::Init(){
	CRigidBody::Init();
	SVector3D	initPos_(0, 0, 0);
	SVector3D	size_(0, 0, 0);

	SetSize(size_.x, size_.y);
	SetPositionCenter(initPos_.x, initPos_.y, initPos_.z);
	SetFriction(0.9f);
	SetRestitution(0.52f);

	colBroad.SetRefOwner(this);
	colBroad.SetRelativePositionCenter(SVector3D(0, 0, 0));
	colBroad.SetRelativeSize(3, 3);
	colBroad.SetColor(255, 255, 0, 128);
}

void CBlockObject::Sync(){
	colBroad.Sync();
}

void CBlockObject::Render(){
	CPolygon3D::Render();
	colBroad.Render();
}

