//==========================================================
// 衝突を起こしているペアの情報を格納するクラス
// ナローフェーズのあたり判定を行う
#ifndef _RIGID_BODY_PAIR_H
#define _RIGID_BODY_PAIR_H

//==========================================================
// インクルード
#include	"RigidBody.h"

//==========================================================
// クラス定義
class CRigidbodyPair {
public:
	// 衝突の可能性がある剛体を追加
	void SetPair(CRigidBody *_pRigid1, CRigidBody *_pRigid2);

	// 衝突判定
	void CollideDetect();

	// 衝突応答
	void CollideResponse();

	// 衝突情報を取得する
	SVector3D GetCollisionVector(){ return collisionVector; }
	SVector3D GetCollisionPosition(){ return collisionPosition; }

	CRigidBody* GetPRigid1(){ return pRigid1; }
	CRigidBody* GetPRigid2(){ return pRigid2; }

	// リストの次ポインタ
	CRigidbodyPair	*next;

private:
	//==========================================================
	// 変数

	// 衝突している可能性がある剛体
	CRigidBody	*pRigid1;		
	CRigidBody	*pRigid2;

	// 衝突応答時に必要な情報
	SVector3D	collisionVector;
	SVector3D	collisionPosition;

	bool isCollide;
	
	//==========================================================
	// 関数

	// GJKアルゴリズムによる衝突検地
	bool CollideRigidbodyAndRigidBody(CRigidBody *_pRigid1, CRigidBody *_pRigid2);

	// 貫通深度、衝突法線を求める
	// GJKアルゴリズムで衝突している場合にのみ呼び出す。
	// メモ)
	// 　_triangle[3] : 衝突している3頂点の座標(ローカル座標)
	// 　_pRigid1, _pRigid2 ： 判定対象
	// 　_pOutColVec : 求めた衝突法線の格納先
	void CalcCollideVector(SVector3D _triangle[3], CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColVec);

	// 衝突座標を求める
	void CalcCollidePosition(SVector3D *_pColVec, CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColPos);

	// 剛体の種類ごとの判定応答
	// メモ)押し戻し距離を取得し、剛体に保持させる。
	void CollideResponseDynamicAndDynamic(CRigidBody *_dynamic1, CRigidBody *_dynamic2);
	void CollideResponseDynamicandKinematic(CRigidBody *_dynamic, CRigidBody *_kinematic);
	void CollideResponseDynamicAndStatic(CRigidBody *_dynamic, CRigidBody *_static);

public:
	CRigidbodyPair();
	~CRigidbodyPair();
};

#endif