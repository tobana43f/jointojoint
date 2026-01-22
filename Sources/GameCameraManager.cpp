//==========================================================
// インクルード
#include	"GameCameraManager.h"

//==========================================================
// メソッド定義

CGameCameraManager::CGameCameraManager(){
	pCamera = NULL;
	pAllCharacterManager = NULL;

	velocity = SVector3D(0, 0, 0);
	initDist = -150;
	curDist = initDist;
}

CGameCameraManager::~CGameCameraManager(){
	pCamera = NULL;
	pAllCharacterManager = NULL;
}

void CGameCameraManager::Init(){
	CCharacter c_ = *pAllCharacterManager->GetPCurCharacter();
	SVector3D pos_(c_.body.GetPositionCenter());

	pCamera->SetCameraPosition(pos_.x, pos_.y, curDist);
	pCamera->SetFocusPosition(pos_.x, pos_.y, pos_.z);
}

void CGameCameraManager::Update(){
	UpdateDistance();
	ControlCamera();
}

//----------------------------------------------------------
// カメラ制御

void CGameCameraManager::MoveNormal(){
	CCharacter c_ = *pAllCharacterManager->GetPCurCharacter();
	SVector3D pos_(c_.body.GetPositionCenter());

	pCamera->SetCameraPosition(pos_.x, pos_.y, pCamera->GetCameraPosition().z);
	pCamera->SetFocusPosition(pos_.x, pos_.y, pos_.z);
}

void CGameCameraManager::MoveSpring(){
	// ばね係数
	float stiffness_ = 0.3f;
	float damping_ = 0.3f;
	float mass_ = 1.0f;

	// 座標を取得
	SVector3D targetPos_ = pAllCharacterManager->GetPCurCharacter()->body.GetPositionCenter();
	SVector3D curPos_	 = pCamera->GetCameraPosition();

	// 仮
	targetPos_.y += 30;
	targetPos_.z = curDist;

	SVector3D springForce_;
	springForce_ = (targetPos_ - curPos_) * stiffness_;

	SVector3D acceleration_ = springForce_ / mass_;

	velocity = (velocity + acceleration_) * damping_;

	// 適用
	pCamera->AddCameraPosition(velocity.x, velocity.y, velocity.z);
	pCamera->AddFocusPosition(velocity.x, velocity.y, 0);
}

void CGameCameraManager::MoveJointObj(){
	// ばね係数
	float stiffness_ = 0.3f;
	float damping_ = 0.3f;
	float mass_ = 1.0f;

	// 座標を取得
	SVector3D targetPos_ = pAllCharacterManager->GetPCurCharacter()->polyCore.GetPositionCenter();
	SVector3D curPos_ = pCamera->GetCameraPosition();

	// 仮
	targetPos_.y -= 50;
	targetPos_.z = curDist;
	targetPos_.z = curDist * (0.8f * (pAllCharacterManager->GetActiveCharacterNum()));

	SVector3D springForce_;
	springForce_ = (targetPos_ - curPos_) * stiffness_;

	SVector3D acceleration_ = springForce_ / mass_;

	velocity = (velocity + acceleration_) * damping_;

	// 適用
	pCamera->AddCameraPosition(velocity.x, velocity.y, velocity.z);
	pCamera->AddFocusPosition(velocity.x, velocity.y, 0);
}

void CGameCameraManager::UpdateDistance(){
	int charaNum_ = pAllCharacterManager->GetActiveCharacterNum();

	curDist = initDist;// *(charaNum_ * 1.0f);

	SVector3D	cp_(pCamera->GetCameraPosition());
	cp_.z = curDist;

	//pCamera->SetCameraPosition(cp_.x, cp_.y, cp_.z);
}

void CGameCameraManager::ControlCamera(){
	// ゲーム状態の判断
	CCharacter cTail_ = *pAllCharacterManager->GetPActiveCharManager()->GetPCharacterListTail();
	bool isTailConnected = false;
	isTailConnected = cTail_.GetIsConnected();

	// 通常時
	if (isTailConnected){
		MoveJointObj();
	}
	else{
		MoveSpring();
	}

}
