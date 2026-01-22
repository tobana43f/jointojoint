//==========================================================
// 剛体に必要なパラメーターを持つクラス、
// ゲーム内のオブジェクトはすべて剛体として定義したほうが、
// シミュレーションんが楽そうなので、このクラスを継承すること。
//----------------------------------------------------------
// 2Dの長方形(四角形)での実装のみをする。

#ifndef _INC_RIGID_BODY_H
#define _INC_RIGID_BODY_H

//==========================================================
// インクルード
#include	"BaseScene.h"		// 定義
#include	"Polygon3D.h"		// 継承
#include	"AABBCollider.h"	// ブロードフェーズ用のあたり判定

#include	"PolygonLine.h"		// デバッグ用
#include	"RigidbodyPipeline.h"

//==========================================================
// クラス定義

class CRigidBody : public CPolygon3D {
public:
	//==========================================================
	// 定数
	
	// 剛体の種類
	enum EType {
		DYNAMIC,	// 通常の剛体()
		KINEMATIC,	// 他から影響を受けない剛体(ただし動かせる)
		STATIC,		// 動かない剛体
	};

	//==========================================================
	// 変数

	// リストとしても使えるようにする。
	CRigidBody* next;

	//==========================================================
	// 関数

	virtual void Init();
	virtual void Destroy();
	virtual void Update();
	virtual void Sync();
	virtual void Render();

	// 剛体を登録するパイプラインのポインタを保持する。
	static void ResisterPipeline(CRigidbodyPipeline *_pResister){ rigidbodyPipeline = _pResister; }

	// セッター
	void SetType(EType _type){ type = _type; }

	void SetIsAffectGravity(bool _state){ isAffectGravity = _state; }

	void SetVelocity(float _x, float _y, float _z){ velocity = SVector3D(_x, _y, _z); }
	void SetVelocity(SVector3D _velocity){ velocity = _velocity; }
	void SetMaxVelocity(SVector3D _max){ maxVelocity = _max; }
	void SetAcceleration(SVector3D _accel){ acceleration = _accel; }
	void SetAngularVelocityDegree(float _deg){ angularVelocityDegZ = _deg; }
	void SetAngularAccelerationDegree(float _deg){ angularAcceleration = _deg; }
	void SetCircleAngleVelDeg(float _deg){ circleAngularVel = _deg; }
	void SetCircleAngleAccDeg(float _deg){ circleAngularAcc = _deg; }

	void SetMass(float _mass){ mass = _mass; }
	void SetFriction(float _friction){ friction = _friction; }
	void SetRestitution(float _restitution){ restitution = _restitution; }
	void SetCentroidRelPosition(SVector3D _relPos){ centroidRelPos = _relPos; }

	// 加算
	void AddVelocity(float _x, float _y, float _z){ velocity = velocity + SVector3D(_x, _y, _z); }
	void AddVelocity(SVector3D _velocity){ velocity = velocity + _velocity; }
	void AddAcceleration(SVector3D _accel){ acceleration = acceleration + _accel; }
	void AddAngularVelocityDegree(float _deg){ angularVelocityDegZ += _deg; }
	void AddAngularAccelerationDegree(float _deg){ angularAcceleration += _deg; }

	void AddCircleAngleVelDeg(float _deg){ circleAngularVel += _deg; }
	void AddCircleAngleAccDeg(float _deg){ circleAngularAcc += _deg; }

	// ゲッター

	EType GetType(){ return type; }

	bool GetIsAffectGravity(){ return isAffectGravity; }

	SVector3D	GetVelocity(){ return velocity; }
	SVector3D	GetMaxVelocity(){ return maxVelocity; }
	SVector3D	GetAcceleration(){ return acceleration; }
	float		GetAngularVelocityDegree(){ return angularVelocityDegZ; }
	float		GetAngularAccelerationDegree(){ return angularAcceleration; }
	float		GetCircleAngleVelDeg(){ return circleAngularVel; }
	float		GetCircleAngleAccDeg(){ return circleAngularAcc; }

	CAABBCollider* GetPCollideBroad(){ return &colBroad; }

	// 加速度を初期化
	void InitAcceleration();

	// 次フレームのポリゴンの情報を取得(コピー)
	// 加速度などの変更の処理がすべて終わった後に呼び出す
	CRigidBody	GetNextData();

	// 係数を取得
	float GetMass(){ return mass; }
	float GetFriction(){ return friction; }
	float GetRestitution(){ return restitution; }
	float GetInertia(){ return inertiaZ;}

	// 重心のワールド座標を求める
	SVector3D	GetCentroidWorldPosition();

	// テスト
	SVector3D	distPushBack;		// 衝突時に自分が受ける押し戻し距離を格納
	float		pushFriction;		// 衝突時の摩擦力
	SVector3D	newVel;				// 衝突後の速度
	//SVector3D	collisionPosition;	// 衝突座標を保持しておく

protected:
	//==========================================================
	// 変数

	EType		type;				// 剛体のタイプ

	bool		isAffectGravity;	// 重力が影響するフラグ

	float		mass;				// 質量(0 < 質量)(-1 : 無限の重さ)
	float		friction;			// 摩擦係数(0:滑りにくい ～　1：滑りやすい)
	float		restitution;		// 反発係数(0:跳ね返らない ～ 1:よく弾む)
	SVector3D	centroidRelPos;		// 重心 (ポリゴンの座標を基準にした相対座標)
	float		inertiaZ;			// 慣性モーメント(平面で行うためZ軸方向だけでいい)
	
	SVector3D	acceleration;			// 加速度
	SVector3D	velocity;				// 物体の速度
	SVector3D	maxVelocity;			// 最大速度

	float		angularVelocityDegZ;	// 角速度
	float		angularAcceleration;	// 角加速度

	float		circleAngularVel;		// 円運動の角速度
	float		circleAngularAcc;		// 円運動の角加速度

	CAABBCollider	colBroad;			// ブロードフェーズで使用するAABBコライダー

	//----------------------------------------------------------
	// 計算

	// 長方形の慣性モーメントを求める
	float CalcRectInertiaZ();

	// 三角形の慣性モーメントを求める
	float CalcTriInertiaZ();

private:
	//==========================================================
	// 変数

	// 剛体を登録するパイプラインへのポインタ
	static CRigidbodyPipeline *rigidbodyPipeline;

	//==========================================================
	// 関数

	// パイプラインに登録
	void ResisterToPipeline(){ rigidbodyPipeline->AddRigidbodyPointer(this); }

public:
	CRigidBody();
	CRigidBody(CRigidBody *_src);
	virtual ~CRigidBody();
};

#endif