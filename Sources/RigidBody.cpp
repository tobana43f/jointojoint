//==========================================================
// インクルード
#include	"Math.h"
#include	"RigidBody.h"

//==========================================================
// 名前空間
using namespace NMath;

//==========================================================
// 静的領域

CRigidbodyPipeline *CRigidBody::rigidbodyPipeline;

//==========================================================
// メソッド定義

CRigidBody::CRigidBody(){
	next = NULL;
	//rigidbodyPipeline->AddRigidbodyPointer(this); // まだパイプラインが作成されてない。
}

CRigidBody::CRigidBody(CRigidBody *_src){
	*this = *_src;
	this->next = _src->next;
}

CRigidBody::~CRigidBody(){

}

void CRigidBody::Init(){
	// ポリゴンの初期化
	CPolygon3D::Init();

	next = NULL;

	// タイプ
	SetType(DYNAMIC);

	SetIsAffectGravity(true);

	// 係数などを設定(セッターを作ること)
	SetMass(10);
	SetFriction(0.52f);
	//SetFriction(0.0f);
	SetRestitution(0.52f);
	centroidRelPos = SVector3D(0, 0, 0);

	// 速度、加速度を初期化
	SetAcceleration(SVector3D(0, 0, 0));
	SetVelocity(SVector3D(0, 0, 0));
	SetMaxVelocity(SVector3D(9999, 9999, 9999));	// 適当な値で初期化
	SetAngularVelocityDegree(0);
	SetAngularAccelerationDegree(0);
	SetCircleAngleVelDeg(0);
	SetCircleAngleAccDeg(0);

	// ブロードフェーズ用のあたり判定を初期化
	colBroad.SetRefOwner(this);
	colBroad.SetRelativePositionCenter(SVector3D(0,0,0));
	colBroad.SetRelativeSize(0,0);
	colBroad.SetColor(255, 255, 255, 128);	// 白
	colBroad.SetType(CAABBCollider::WRAPPER);

	distPushBack = SVector3D(0, 0, 0);
	pushFriction = 1.0f;
	newVel = SVector3D(0, 0, 0);
	// 剛体パイプラインに登録
	ResisterToPipeline();
}

void CRigidBody::Destroy(){
	CPolygon3D::Destroy();
}

void CRigidBody::Update(){
	CPolygon3D::Update();
	colBroad.Update();
}

void CRigidBody::Sync(){
	// あたり判定の同期
	colBroad.Sync();

}

void CRigidBody::Render(){
	// ポリゴンの描画
	CPolygon3D::Render();
	colBroad.Render();
}

//----------------------------------------------------------
// ユーティリティ

void CRigidBody::InitAcceleration(){
	SetAcceleration(SVector3D(0,0,0));
	SetAngularAccelerationDegree(0.0f);
}

CRigidBody CRigidBody::GetNextData(){
	CRigidBody next_(this);	// 現在の剛体をコピー

	// 加速度を速度に代入
	//next_.AddVelocity(next_.GetAcceleration());
	//next_.AddAngularVelocityDegree(next_.GetAngularVelocityDegree());

	SVector3D v_ = next_.GetVelocity();
	next_.AddPositionCenter(v_.x, v_.y, v_.z);

	float avd_ = next_.GetAngularVelocityDegree();
	next_.AddAngleDegree(0, 0, avd_);

	return next_;
}

SVector3D CRigidBody::GetCentroidWorldPosition(){
	SVector3D centroidWorld_;
	centroidWorld_.x = GetPositionCenter().x + centroidRelPos.x;
	centroidWorld_.y = GetPositionCenter().y + centroidRelPos.y;
	centroidWorld_.z = GetPositionCenter().z + centroidRelPos.z;

	return centroidWorld_;
}

float CRigidBody::CalcRectInertiaZ(){
	return mass * (GetSize().x * GetSize().x) / 12.0f;
}

float CRigidBody::CalcTriInertiaZ(){
	// 重心からの頂点へのベクトル
	SVector3D vecVx0_ = CalcVertexPosition(CSquarePolygonData::VERTEX_0) - GetCentroidWorldPosition();
	SVector3D vecVx1_ = CalcVertexPosition(CSquarePolygonData::VERTEX_1) - GetCentroidWorldPosition();
	SVector3D vecVx2_ = CalcVertexPosition(CSquarePolygonData::VERTEX_2) - GetCentroidWorldPosition();

	// Z軸を無視
	vecVx0_.z = 0;
	vecVx1_.z = 0;
	vecVx2_.z = 0;

	// 距離を求める
	float lenVx0_ = CalcVector3DLength(&vecVx0_);
	float lenVx1_ = CalcVector3DLength(&vecVx1_);
	float lenVx2_ = CalcVector3DLength(&vecVx2_);

	// 内積
	float dotV1V2 = CalcDotVector3D(&vecVx1_, &vecVx2_);
	float dotV2V0 = CalcDotVector3D(&vecVx2_, &vecVx0_);
	float dotV0V1 = CalcDotVector3D(&vecVx0_, &vecVx1_);

	// 慣性モーメントを求める
	float i = (1.0f / 18.0f) * mass * (lenVx0_ + lenVx1_ + lenVx2_ - dotV1V2 - dotV2V0 - dotV0V1);

	// メンバに格納
	return i;
}
