//==========================================================
// インクルード
#include	"BaseJointableObj.h"
#include	"Allocator.h"

//==========================================================
// 静的領域
const float CBaseJointableObject::BLOCK_SIZE = 20;

CInput	*CBaseJointableObject::pRefInput1 = NULL;
CAllCharacterManager *CBaseJointableObject::pRefCharacter = NULL;
CTexture	*CBaseJointableObject::pTexRefBlock = NULL;
CGameState	*CBaseJointableObject::pRefGameState = NULL;

//==========================================================
// メソッド定義

CBaseJointableObject::CBaseJointableObject(){
	block = NULL;
	isConnected = false;
}

CBaseJointableObject::~CBaseJointableObject(){
	Destroy();
}

void CBaseJointableObject::Init(SVector3D _initPos, float _initAngle){
	colCoreSpace.SetColor(255, 0, 0, 64);
}

void CBaseJointableObject::Destroy(){
	DeleteArraySafely(block);
}

void CBaseJointableObject::Update(){
	// ポリゴンの更新
	for (int i = 0; i < blockNum; ++i){
		GetPBlkockObject(i)->Update();
	}
	colCoreSpace.Update();

	//ProcSeperateCore();
}

void CBaseJointableObject::Sync(){
	for (int i = 0; i < blockNum; ++i){
		GetPBlkockObject(i)->Sync();
	}
	colCoreSpace.Sync();

	SyncCore();
}

void CBaseJointableObject::Render(){
	for (int i = 0; i < blockNum; ++i){
		GetPBlkockObject(i)->Render();
	}
	colCoreSpace.Render();
}

void CBaseJointableObject::ProcSeperateCore(){
	if (!isConnected)
		return;

	CCharacter *char_ = GetPCharacter()->GetPCurCharacter();

	char_->SetIsConnected(false);
	char_->ProcCoreSeperate();
	isConnected = false;

}

void CBaseJointableObject::SyncCore(){
	if (!isConnected)
		return;

	CCharacter *char_ = GetPCharacter()->GetPCurCharacter();
	SVector3D corePos_ = colCoreSpace.GetWorldPositionCenter();

	char_->polyCore.SetPositionCenter(
		corePos_.x, corePos_.y, corePos_.z
		);
}