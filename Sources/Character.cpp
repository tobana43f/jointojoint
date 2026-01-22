//==========================================================
// インクルード
#include	"Character.h"
#include	"System.h"
#include	"Math.h"

using namespace NSystem;
using namespace NMath;

//==========================================================
// グローバル変数
CTexture	*CCharacter::pTexRefBodyActive;
CTexture	*CCharacter::pTexRefCoreActive;
CTexture	*CCharacter::pTexRefCoreNegative;
CTexture	*CCharacter::pTexRefCable;

//==========================================================
// 定数ゲッター(インラインで定義出来ないものだけ)

int		CCharacter::GetCntCoreCollideInterval(){ return (int)(DEFAULT_FRAME_RATE * 0.5f); }
float	CCharacter::GetSpringStiffness(){ return 0.44f; }
float	CCharacter::GetCoreMass(){ return 1.0f; }
float	CCharacter::GetBodyMass(){ return 1.0f;}
float	CCharacter::GetCableNaturalLength(){ return 43.0f; }
float	CCharacter::GetCableWidth(){ return 2.5f; }

//==========================================================
// メソッド定義

CCharacter::CCharacter(){
	initPositionCenter = SVector3D(0, 0, 0);
}

CCharacter::~CCharacter(){
	
}

void CCharacter::Init(){
	SVector3D	initPos_(0, 0, 0);

	//----------------------------------------------------------
	// 汎用
	cnt = 0;
	cntCoreSeperate = cnt;

	//----------------------------------------------------------
	// ボディの初期化
	body.Init();
	body.SetPositionCenter(initPos_.x, initPos_.y, initPos_.z);
	body.SetMass(GetBodyMass());
	body.SetReferenceTexture(pTexRefBodyActive);

	//body.SetType(CRigidBody::KINEMATIC);
	//body.SetIsAffectGravity(false);
	body.SetMaxVelocity(SVector3D(2, 10, 0));


	// コアをつなげられる部分のあたり判定を設定
	colliderCoreSpace.SetRefOwner(&body);
	colliderCoreSpace.SetRelativePositionCenter(SVector3D(0, 0, 0));
	colliderCoreSpace.SetRadius(body.GetSize().x / 2.0f);
	colliderCoreSpace.SetColor(255, 0, 0,  128);

	//----------------------------------------------------------
	// コアの初期化

	// コア本体を初期化
	polyCore.Init();
	polyCore.SetSize(body.GetSize().x, body.GetSize().y);
	polyCore.SetPositionCenter(initPos_.x, initPos_.y, initPos_.z);
	polyCore.SetReferenceTexture(pTexRefCoreActive);
	acceleartionCore = SVector3D(0, 0, 0);
	velocityCore = SVector3D(0, 0, 0);

	// コアのあたり判定を設定
	colliderCore.SetRefOwner(&polyCore);
	colliderCore.SetRelativePositionCenter(SVector3D(0, 0, 0));
	colliderCore.SetRadius(polyCore.GetSize().x / 3.0f);
	colliderCore.SetColor(0, 255, 0, 128);

	//----------------------------------------------------------
	// ケーブルの初期化
	polyCable.Init();
	polyCable.SetLineWidth(GetCableWidth());
	polyCable.SetReferenceTexture(pTexRefCable);
	SyncCablePosition();

	//----------------------------------------------------------
	// 着地常態あたり判定
	colGround.SetRefOwner(&body);
	colGround.SetRelativeSize(-body.GetSize().x * 0.4f, -body.GetSize().y * 0.75f);
	initColGroundRelPos = SVector3D(0, -body.GetSize().y * 0.45f, 0);
	colGround.SetRelativePositionCenter(initColGroundRelPos);
	colGround.SetColor(0, 255,255, 128);

	//----------------------------------------------------------
	// リスト用のポインタを初期化
	prev = NULL;
	next = NULL;

	SetIsJumpReady(false);
	SetIsConnected(false);
}

void CCharacter::Update(){
	polyCore.Update();
	polyCable.Update();

	body.Update();
	// あたり判定の更新
	colliderCoreSpace.Update();
	colliderCore.Update();
	colGround.Update();
	{
		SVector3D	add_(0, 0, 0);
		int	rotZ_ = (int)fabs((float)((int)body.GetAngleDegree().z % 90));
		rotZ_ *= 2;
		float rotZRad_ = ConvertDegToRad((float)rotZ_);
		
		add_.y = sin(rotZRad_) * -3;

		colGround.SetRelativePositionCenter(initColGroundRelPos + add_);
	}

	// コアのあたり判定の無効時間を判定
	ProcCoreInterval();

	// 回転の補正
	ProcAdjustRotate();

	// カウンタ更新
	cnt++;
}

void CCharacter::Sync(){
	// ケーブルの位置をボディとコアに同期させる
	SyncCablePosition();

	// あたり判定更新
	body.Sync();
	colliderCoreSpace.Sync();
	colliderCore.Sync();
	colGround.Sync();
}

void CCharacter::Render(){
	polyCable.Render();
	polyCore.Render();
	body.Render();

	// あたり判定の描画
	colliderCoreSpace.Render();
	colliderCore.Render();
	colGround.Render();
}

void CCharacter::SetReferenceResource(CTexture *_pTexBody, CTexture *_pTexCore, CTexture	*_pTexCoreNega, CTexture *_pTexCable){
	pTexRefBodyActive = _pTexBody;
	pTexRefCoreActive = _pTexCore;
	pTexRefCoreNegative = _pTexCoreNega;
	pTexRefCable = _pTexCable;
}

void CCharacter::SyncCablePosition(){
	polyCable.SetPositionStart(				// スタートをボディに設定
		body.GetPositionCenter().x,
		body.GetPositionCenter().y,
		body.GetPositionCenter().z
		);
	polyCable.SetPositionEnd(				// エンドをコアに設定
		polyCore.GetPositionCenter().x,
		polyCore.GetPositionCenter().y,
		polyCore.GetPositionCenter().z
		);
}

void CCharacter::ProcCoreInterval(){
	if (cnt > cntCoreSeperate + GetCntCoreCollideInterval()){
		colliderCore.SetIsActive(true);
	}
	else {
		colliderCore.SetIsActive(false);
	}
}

// メモ)
// 　+: CCW
// 　-:　CW
void CCharacter::ProcAdjustRotate(){
	//----------------------------------------------------------
	// 静止状態の時のみ補正をする

	// 速度が遅い時のみ処理をする
	if (fabs(body.GetVelocity().x) > 0.5f)
		return;
	
	//----------------------------------------------------------
	// 0度になるように調節していく。(辺が地面につくように)

	// 現在の角度を90度以内の数値に補正
	float curRotZ_ = body.GetAngleDegree().z;
	int	  up_ = (int)(curRotZ_) % 90;	// 小数点以上の数字
	up_ = (int)fabs((float)up_);

	// すでに0度になっている場合
	if (up_ <= 1)
		return;

	float adjstMag_ = 1.5f;	// 補正量

	// 45度を超えている場合は、右周りに補正、
	// 45度以下の場合は、左回りに補正
	// ただし、符号によって反転する。(-の場合は反転)
	if (curRotZ_ < 0)
		adjstMag_ *= -1;

	if (up_ > 45) 
		adjstMag_ *= 1;
	else if (up_ <= 45)
		adjstMag_ *= -1;

	// 適用
	body.AddAngularAccelerationDegree(adjstMag_);
}