//==========================================================
// インクルード
#include	"StageObjTest.h"

//==========================================================
// メソッド定義
CStageObjTest::CStageObjTest(){

}

CStageObjTest::~CStageObjTest(){

}

void CStageObjTest::Init(SVector3D _initPos, float _initAngle){
	// 一度解放
	Destroy();

	// 基準の座標を設定
	positionLeftTop = _initPos;
	angleDegree = _initAngle;

	// ブロック数分確保
	blockNum = 2;
	block = new CBlockObject[blockNum];

	// それぞれのブロックを初期化
	InitBlockObj();
}

void CStageObjTest::Update(){
	CBaseStageObject::Update();

	// オブジェクトの操作
}


void CStageObjTest::InitBlockObj(){
	CBlockObject *pObj1_ = &block[0];
	CBlockObject *pObj2_ = &block[1];

	//----------------------------------------------------------
	// それぞれのブロックに絶対座標を設定する。
	pObj1_->Init();
	pObj1_->SetType(CRigidBody::KINEMATIC);
	pObj1_->SetSize(BLOCK_SIZE * 2, BLOCK_SIZE);
	SVector3D	relPos1_(0, 0, 0);
	SVector3D	pos1_;
	pos1_ = relPos1_ + positionLeftTop;
	pObj1_->SetPositionLeftTop(pos1_.x, pos1_.y, pos1_.z);
	pObj1_->SetReferenceTexture(pTexRefBlock);

	pObj2_->Init();
	pObj2_->SetType(CRigidBody::KINEMATIC);
	pObj2_->SetSize(BLOCK_SIZE * 2, BLOCK_SIZE);
	SVector3D	relPos2_(20, 20, 0);
	SVector3D	pos2_;
	pos2_ = relPos2_ + positionLeftTop;
	pObj2_->SetPositionLeftTop(pos2_.x, pos2_.y, pos2_.z);
	pObj2_->SetReferenceTexture(pTexRefBlock);
}