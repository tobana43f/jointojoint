//==========================================================
// インクルード
#include	"JointableObjPendulam.h"
#include	"Math.h"
#include	"Physics.h"

//==========================================================
// 名前空間
using namespace NMath;
using namespace NPhysics;

//==========================================================
// メソッド定義
CJointableObjPendulam::CJointableObjPendulam(){

}

CJointableObjPendulam::~CJointableObjPendulam(){

}

void CJointableObjPendulam::Init(SVector3D _initPos, float _initAngle){
	Destroy();

	CBaseJointableObject::Init(_initPos);

	positionLeftTop = _initPos;
	angleDegree = _initAngle;

	blockNum = 1;
	block = new CBlockObject[blockNum];

	// それぞれのブロックを初期化
	InitBlockObj();
}

void CJointableObjPendulam::Update(){
	CBaseJointableObject::Update();

	// 繋げられているときに、キャラクターを操作する。
	UpdateCharacter();
}

void CJointableObjPendulam::UpdateCharacter(){
	if (!GetIsConnected())
		return;
	//----------------------------------------------------------
	// 操作

	{	// 先頭のコアとバネを引っ張り合わせる
		SVector3D	sprAcc(0, 0, 0);
		CCharacter	*pCur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail();
		float		naturalLen_ = CCharacter::GetCableNaturalLength();
		float		stiffness_ = CCharacter::GetSpringStiffness() * 0.8f;

		SVector3D	body_ = pCur_->body.GetPositionCenter();
		float		mBody_ = CCharacter::GetBodyMass();

		SVector3D	core_ = pCur_->polyCore.GetPositionCenter();
		float		mCore_ = CCharacter::GetCoreMass();

		// ボディへの影響
		sprAcc = sprAcc + CalcSpringAcceleration(body_, core_, mBody_, naturalLen_, stiffness_);
		pCur_->body.AddAcceleration(sprAcc);
		pCur_->body.AddAngularAccelerationDegree(-sprAcc.x * 2);
	}

	// 回転
	{
		float rotSpeed = GetPI() / 900.0f;
		float aRot = 0;
		//if (pRefInput1->GetHorizontalPositive(HOLD))
		if(pRefInput1->GetHoldStickRight())
			aRot -= rotSpeed;
		//else if (pRefInput1->GetHorizontalNegative(HOLD))
		else if(pRefInput1->GetHoldStickLeft())
			aRot += rotSpeed;

		//CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail();
		CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead();

		while (cur_ != NULL){
			cur_->body.AddCircleAngleAccDeg(aRot);
			//cur_ = cur_->prev;
			cur_ = cur_->next;
		}
	}

	{	// ProcRotGravity()
		//CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail();
		CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead();

		SVector3D gravity(0, -0.006f, 0);
		while (cur_ != NULL){
			SVector3D org;
			
			// 先頭の時はコア
			//if (cur_ == GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail())
			//	org = cur_->polyCore.GetPositionCenter();
			//else
			//	org = cur_->next->body.GetPositionCenter();

			org = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail()->polyCore.GetPositionCenter();


			SVector3D	pos = cur_->body.GetPositionCenter();
			SVector3D	vecPos = pos - org;
			float		r = CalcVector3DLength(&vecPos);
	
			SVector3D cpPxG = CalcCrossVector3D(&gravity, &vecPos);	// Z成分のみ必要
			float omega = cpPxG.z / (r);
			
			cur_->body.AddCircleAngleVelDeg(omega);

			//cur_ = cur_->prev;
			cur_ = cur_->next;
		}
	}


	{	// ApllyAccRot()
		//CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail();
		CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead();
		while (cur_ != NULL){
			cur_->body.AddCircleAngleVelDeg(cur_->body.GetCircleAngleAccDeg());
			cur_->body.SetCircleAngleAccDeg(0);
			//cur_ = cur_->prev;
			cur_ = cur_->next;
		}
	}
		
	{	// ApllyVelRot()
		//CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail();
		CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead();

		while (cur_ != NULL){
			SMatrix44 mxRotZ = CalcMatrixRotateZ(cur_->body.GetCircleAngleVelDeg());
			SVector3D org;

			//if (cur_ == GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail())
			//	org = cur_->polyCore.GetPositionCenter();
			//else
			//	org = cur_->next->body.GetPositionCenter();

			org = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail()->polyCore.GetPositionCenter();

			SVector3D pos = cur_->body.GetPositionCenter();
			SVector3D vecPos = pos - org;
			SVector3D newVecPos = CalcVector3DTransform(&mxRotZ, &vecPos);
			SVector3D newPos = newVecPos + org;

			// 新しい座標へのベクトルを求める
			SVector3D newVec = newPos - pos;

			cur_->body.SetPositionCenter(newPos.x, newPos.y, 0);

			//cur_ = cur_->prev;
			cur_ = cur_->next;
		}
	}

	//----------------------------------------------------------
	// 外す
	if (pRefInput1->GetSeperate(TRIGGER)){
		//CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail();
		CCharacter *cur_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead();
		while (cur_ != NULL){
			SVector3D org;
			float speed_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead()->body.GetCircleAngleVelDeg();
			//float speed_ = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail()->body.GetCircleAngleVelDeg();
			//if (cur_ == GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail())
			//	org = cur_->polyCore.GetPositionCenter();
			//else
			//	org = cur_->next->body.GetPositionCenter();

			org = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListTail()->polyCore.GetPositionCenter();

			//SVector3D	pos = cur_->body.GetPositionCenter();
			SVector3D	pos = GetPCharacter()->GetPActiveCharManager()->GetPCharacterListHead()->body.GetPositionCenter();
			SVector3D	vecPos = pos - org;
			float		r = CalcVector3DLength(&vecPos);

			//speed_ *= fabs(r);

			SVector3D vec_;
			float rad_ = atan2(vecPos.y, vecPos.x) - GetPI() / 2.0f;
			vec_.x = speed_ * cos(rad_);
			vec_.y = speed_ * sin(rad_);
			vec_.z = 0;

			vec_ = vec_ * (r * 2);

			// ｘ方向へ強く
			vec_.x *= 4.0f;
			vec_.y *= 1.6f;
			cur_->body.AddVelocity(vec_);

			//cur_ = cur_->prev;
			cur_ = cur_->next;
		}

		// 外す処理
		ProcSeperateCore();
	}

}

void CJointableObjPendulam::InitBlockObj(){
	CBlockObject *pObj1_ = &block[0];

	//----------------------------------------------------------
	// それぞれのブロックに絶対座標を設定する。
	pObj1_->Init();
	pObj1_->SetType(CRigidBody::KINEMATIC);
	pObj1_->SetSize(BLOCK_SIZE, BLOCK_SIZE);
	SVector3D	relPos1_(0, 0, 0);
	SVector3D	pos1_;
	pos1_ = relPos1_ + positionLeftTop;
	pObj1_->SetPositionLeftTop(pos1_.x, pos1_.y, pos1_.z);
	pObj1_->SetReferenceTexture(pTexRefBlock);

	colCoreSpace.SetRefOwner(pObj1_);
	colCoreSpace.SetRadius(10);
}

