//==========================================================
// インクルード
#include	"RigidbodyPipeline.h"

#include	"RigidBody.h"
#include	"RigidbodyPairManager.h"

#include	"CollideDetecter.h"
#include	"System.h"
#include	"Allocator.h"
#include <math.h>

//==========================================================
// 名前空間
using namespace NSystem;
using namespace NCollideDetecter;

//==========================================================
// メソッド定義

CRigidbodyPipeline::CRigidbodyPipeline(){
	// リストの先頭にダミーデーターを確保
	rigidbodyListTop = new CRigidBody;
	rigidbodyListTop->next = NULL;

	rigidbodyNum = 0;

	pairManager = new CRigidbodyPairManager;

	deltaTime = 1.0f / DEFAULT_FRAME_RATE;
	gravity = SVector3D(0, 0, 0);

	pProxyArray = NULL;
}

CRigidbodyPipeline::~CRigidbodyPipeline(){
	DeleteSafely(rigidbodyListTop);
	//FreeAllRigidbodyList();	こっちで解放するのは危険
	FreeProxy();
	DeleteSafely(pairManager);
}

void CRigidbodyPipeline::Update(){
	// ペアの初期化
	pairManager->Init();

	// 要素を数える
	CountRigidbody();

	// 剛体を更新
	UpdateRigidbody();

	// 重力などの共通の処理
	ProcGravity();
	
	// 速度に代入
	ApllyVelocity();

	// 速度に影響する共通の処理
	ProcFriction();

	// 次フレームの剛体をすべて作成(速度を位置に加算したもの)
	AllocProxy();

	// ブロードフェーズ用のあたり判定を同期
	SyncProxy();
	
	// 作成した次フレームの剛体で衝突検知(ブロードフェーズ)
	// 衝突している可能性のあるペアを作る。
	CollideBroad();

	// 衝突検知(ナローフェーズ)
	pairManager->CollideDetect();

	// 衝突応答
	pairManager->CollideResponse();

	// 押し戻し
	ProcPushBack();

	// コピーした剛体を実際の剛体に適用する。
	ApllyProxy();

	// 不必要になったプロキシを削除
	FreeProxy();

}

void CRigidbodyPipeline::AddRigidbodyPointer(CRigidBody *_pRigidbody){
	CRigidBody *cur = rigidbodyListTop;

	while (cur->next != NULL){
		cur = cur->next;
	}
	cur->next = _pRigidbody;
	_pRigidbody->next = NULL;
}

CRigidBody* CRigidbodyPipeline::GetPRigidbodyListTop(){
	return rigidbodyListTop->next;
}

void CRigidbodyPipeline::FreeAllRigidbodyList(){
	CRigidBody *cur_ = GetPRigidbodyListTop();
	CRigidBody *next_ = NULL;

	// 全リストを開放
	while (cur_ != NULL){
		next_ = cur_->next;
		DeleteSafely(cur_);
		cur_ = next_;
	}
}

//----------------------------------------------------------

void CRigidbodyPipeline::CountRigidbody(){
	CRigidBody	*cur_ = GetPRigidbodyListTop();
	int			entityNum_ = 0;

	while (cur_ != NULL){
		entityNum_++;
		cur_ = cur_->next;
	}

	rigidbodyNum = entityNum_;
}

void CRigidbodyPipeline::UpdateRigidbody(){
	CRigidBody	*cur_ = GetPRigidbodyListTop();

	while (cur_ != NULL){
		cur_->Update();
		cur_ = cur_->next;
	}
}

void CRigidbodyPipeline::ProcGravity(){
	CRigidBody *cur_ = GetPRigidbodyListTop();
	SVector3D	deltaGravity_(gravity);// / (deltaTime));
	//SVector3D	deltaGravity_(0, -3, 0);
	
	while (cur_ != NULL){
		// 影響するかどうか
		if (!cur_->GetIsAffectGravity()){
			cur_ = cur_->next;
			continue;
		}

		if (cur_->GetType() == CRigidBody::DYNAMIC){
			cur_->AddAcceleration(deltaGravity_);
		}
		cur_ = cur_->next;
	}
}

void CRigidbodyPipeline::SyncProxy(){
	for (int i = 0; i < rigidbodyNum; ++i){
		pProxyArray[i].Sync();
	}
}

void CRigidbodyPipeline::ApllyVelocity(){
	CRigidBody *cur_ = GetPRigidbodyListTop();

	while (cur_ != NULL){
		// 加速度を速度に適用する
		cur_->AddVelocity(cur_->GetAcceleration());
		cur_->AddAngularVelocityDegree(cur_->GetAngularAccelerationDegree());

		// 加速度が最大を超えていたら補正する
		SVector3D curVel_ = cur_->GetVelocity();
		SVector3D maxVel_ = cur_->GetMaxVelocity();
		if (fabs(curVel_.x) > fabs(maxVel_.x)){
			int sign_ = (int)(curVel_.x / fabs(curVel_.x));
			cur_->SetVelocity(maxVel_.x * sign_, curVel_.y, curVel_.z);
		}
		if (fabs(curVel_.y) > (maxVel_.y)){
			int sign_ = (int)(curVel_.y / fabs(curVel_.y));
			cur_->SetVelocity(curVel_.x, maxVel_.y * sign_, curVel_.z);
		}

		// 適用し終わった加速度は初期化しておく。
		cur_->InitAcceleration();
		cur_ = cur_->next;
	}
}

void CRigidbodyPipeline::ProcFriction(){
	CRigidBody *cur_ = GetPRigidbodyListTop();

	while (cur_ != NULL){
		cur_->SetVelocity(cur_->GetVelocity() * 0.95f);
		cur_->SetAngularVelocityDegree(cur_->GetAngularVelocityDegree() * 0.6f);
		cur_->SetCircleAngleVelDeg(cur_->GetCircleAngleVelDeg() * 0.975f);
		cur_ = cur_->next;
	}
}

void CRigidbodyPipeline::AllocProxy(){
	// コピーする配列を確保
	pProxyArray = new CRigidBody[rigidbodyNum];
	int	idx_ = 0;

	// プロキシ配列を確保する為にリストの要素数を数える
	CRigidBody	*cur_ = GetPRigidbodyListTop();

	while (cur_ != NULL){
		pProxyArray[idx_] = cur_->GetNextData();
		cur_ = cur_->next;
		idx_++;
	}
}

void CRigidbodyPipeline::FreeProxy(){
	DeleteArraySafely(pProxyArray);
}

void CRigidbodyPipeline::CollideBroad(){
	// それぞれの剛体が1度筒だけ判定する

	for (int idxActive_ = 0; idxActive_ < rigidbodyNum; ++idxActive_){
		for (int idxPassive_ = idxActive_ + 1; idxPassive_ < rigidbodyNum; ++idxPassive_){

			// 剛体の種類で当たり判定を行うか判断する
			// 両方とも動的剛体でない場合は判定をとらない
			if (!(pProxyArray[idxActive_].GetType() == CRigidBody::DYNAMIC ||
				pProxyArray[idxPassive_].GetType() == CRigidBody::DYNAMIC)){
				continue;
			}

			// AABBでの当たり判定を行う
			bool isCollide_ = false;
			isCollide_ = CollideAABBAndAABB(pProxyArray[idxActive_].GetPCollideBroad(), pProxyArray[idxPassive_].GetPCollideBroad());

			// 衝突の可能性がある場合はペアとして登録
			if (isCollide_){
				pairManager->AddPair(&pProxyArray[idxActive_], &pProxyArray[idxPassive_]);
			}
		}
	}
}

void CRigidbodyPipeline::ProcPushBack(){
	CRigidBody *curEntity_ = GetPRigidbodyListTop();
	int	idx_ = 0;
	SVector3D	distPushBack_(0, 0, 0);
	SVector3D	newVel_(0, 0, 0);

	float lowLim_ = 0.0f;	// 押し戻し距離がこの値以下の場合は補正しない
	float lowLimVel_ = 0.0f;

	for (int i = 0; i < rigidbodyNum; ++i){
		if (pProxyArray[i].GetType() != CRigidBody::DYNAMIC)
			continue;

		distPushBack_ = pProxyArray[i].distPushBack;
		newVel_ = pProxyArray[i].newVel;

		// 下端判断
		if (fabs(distPushBack_.x) < lowLim_)
			distPushBack_.x = 0;
		if (fabs(distPushBack_.y) < lowLim_)
			distPushBack_.y = 0;

		if (fabs(newVel_.x) < lowLimVel_)
			newVel_.x = 0;
		if (fabs(newVel_.y) < lowLimVel_)
			newVel_.y = 0;


		pProxyArray[i].AddPositionCenter(distPushBack_.x, distPushBack_.y, distPushBack_.z);

		// 新しい速度をセット(摩擦)
		pProxyArray[i].SetVelocity(pProxyArray[i].GetVelocity() * pProxyArray[i].pushFriction);

		// 押し出し方向へ速度を追加
		pProxyArray[i].AddVelocity(newVel_);


		pProxyArray[i].distPushBack = SVector3D(0, 0, 0);
		pProxyArray[i].newVel = SVector3D(0, 0, 0);
		pProxyArray[i].pushFriction = 1.0f;
	}
}

void CRigidbodyPipeline::ApllyProxy(){
	CRigidBody *curEntity_ = GetPRigidbodyListTop();
	int	idx_ = 0;

	while (curEntity_ != NULL){
		// 内容をコピー
		*curEntity_ = pProxyArray[idx_];

		// 次へ
		curEntity_ = curEntity_->next;
		idx_++;
	}
}

