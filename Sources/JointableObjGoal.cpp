//==========================================================
// インクルード
#include	"JointableObjGoal.h"

//==========================================================
// 静的領域
CTexture	*CJointableObjGoal::pRefTexGoal = NULL;

//==========================================================
// メソッド定義
CJointableObjGoal::CJointableObjGoal(){

}

CJointableObjGoal::~CJointableObjGoal(){

}

void CJointableObjGoal::Init(SVector3D _initPos, float _initAngle){
	Destroy();

	CBaseJointableObject::Init(_initPos);

	positionLeftTop = _initPos;
	angleDegree = _initAngle;

	blockNum = 1;
	block = new CBlockObject[blockNum];

	// それぞれのブロックを初期化
	InitBlockObj();
}

void CJointableObjGoal::Update(){
	CBaseJointableObject::Update();

	// 繋げられているときに、キャラクターを操作する。
	UpdateCharacter();
}

void CJointableObjGoal::UpdateCharacter(){
	if (!GetIsConnected()){
		GetPGameState()->isGameClear = false;
		return;
	}

	// ゴール判定を行う
	GetPGameState()->isGameClear = true;
}

void CJointableObjGoal::InitBlockObj(){
	CBlockObject *pObj1_ = &block[0];

	//----------------------------------------------------------
	// それぞれのブロックに絶対座標を設定する。
	pObj1_->Init();
	pObj1_->SetType(CRigidBody::KINEMATIC);
	pObj1_->SetSize(BLOCK_SIZE * 2, BLOCK_SIZE * 2);
	SVector3D	relPos1_(0, 0, 0);
	SVector3D	pos1_;
	pos1_ = relPos1_ + positionLeftTop;
	pObj1_->SetPositionLeftTop(pos1_.x, pos1_.y, pos1_.z);
	pObj1_->SetReferenceTexture(pRefTexGoal);

	colCoreSpace.SetRefOwner(pObj1_);
	colCoreSpace.SetRelativePositionCenter(SVector3D(-10, -10, 0));
	colCoreSpace.SetRadius(10);
}

