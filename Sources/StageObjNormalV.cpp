#include "StageObjNormalV.h"


// □
// □


//-----------------
// コンストラクタ
CStageObjNormalV::CStageObjNormalV(){

}
//---------------
// デストラクタ
CStageObjNormalV::~CStageObjNormalV(){

}

//---------
// 初期化
void CStageObjNormalV::Init(SVector3D _initPos, float _initAngle){
	// 一度解放
	Destroy();

	// 初期化数値を設定
	positionLeftTop = _initPos;	// 基準となる座標
	angleDegree = _initAngle;	// ブロックの角度

	// ブロック数分確保
	blockNum = 1;	// ブロックの数
	block = new CBlockObject[blockNum];	// オブジェクトを確保

	// 初期化
	InitBlockObj();
}

//-------------------
// ブロックの初期化
void CStageObjNormalV::InitBlockObj(){
	// 各ブロックの変数
	CBlockObject *pObj1_ = &block[0];

	// それぞれのブロックの絶対座標を設定
	pObj1_->Init();
	pObj1_->SetType(CRigidBody::STATIC);		// オブジェクトの種類（動かない）
	pObj1_->SetSize(BLOCK_SIZE, BLOCK_SIZE * 2);// サイズ 1*2
	SVector3D relPos1_(0, 0, 0);
	SVector3D pos1_;
	pos1_ = relPos1_ + positionLeftTop;		// ブロックの位置計算
	pObj1_->SetPositionLeftTop(pos1_.x, pos1_.y, pos1_.z);	// 設定
	pObj1_->SetReferenceTexture(pTexRefBlock);
}

//-------
// 更新
void CStageObjNormalV::Update(){
	CBaseStageObject::Update();
}