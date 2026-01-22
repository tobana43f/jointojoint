 //==========================================================
// インクルード
#include	"PendulumScene.h"
#include	"GameTestScene.h"

#include	"System.h"
#include	"Allocator.h"		// newオーバーライド
#include	"ControllerDefinition.h"
#include	"Math.h"

//==========================================================
// 名前空間

using namespace NSystem;
using namespace NControllerDefinition;
using namespace NMath;

//==========================================================
// メソッド定義

CPendulumScene::CPendulumScene(){
	pCameraOrtho = new CCameraOrthographic;
	pCameraPers = new CCameraPerspective;

	scene = LOAD;
}

CPendulumScene::~CPendulumScene(){
	DeleteSafely(pCameraOrtho);
	DeleteSafely(pCameraPers);
}

void CPendulumScene::Init(){
	//==========================================================
	// カメラの初期化
	pCameraPers->SetCameraPosition(0, 0, -300);
	pCameraPers->SetFocusPosition(0, 0, 0);
	pCameraPers->SetScreenAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
	pCameraPers->SetUpDirection(0, 1, 0);
	pCameraPers->SetClipDistance(1, 10000);
	pCameraPers->SetViewAngleDeg(60);

	//==========================================================
	// 初期化

	//----------------------------------------------------------
	// デバッグ用フォント
	txtTitle.SetPositionLeftTop(0, 0);
	txtTitle.SetSize(18, 24);
	txtTitle.SetReferenceTexture(&texFont);

	txtDbg.SetPositionLeftTop(10, 30);
	txtDbg.SetSize(12, 16);
	txtDbg.SetReferenceTexture(&texFont);

	//----------------------------------------------------------
	// その他の初期化

	for (int i = 0; i < NUM; ++i){
		body[i].Init();
		body[i].SetSize(5, 5);
		body[i].SetPositionCenter(0.0f + i * 10.0f, 0, 0);
		body[i].SetColor(i * (255 / NUM), 0, 0, 255);

		velRotZ[i] = 0;
		accRotZ[i] = 0;

		vel[i] = SVector3D(0, 0, 0);
		acc[i] = SVector3D(0, 0, 0);

	}
	SyncSpring();

	gravity = SVector3D(0, -1.0f, 0);
	radGrvty = atan2f(-1.0f, 0.0f);

	timeStep = 1.0f / 60.0f;
}

void CPendulumScene::Destroy(){

}

CBaseScene* CPendulumScene::Run(){
	switch (scene){
		case LOAD:
			Load();
			scene = INIT;
		case INIT:
			Init();
			scene = MAIN;
		case MAIN:
			Update();
			Render();
			RenderDebug();
			break;
	}

	// 画面遷移
	if (GetPInput(CONTROLLER_1)->GetJump(TRIGGER))
		SetIsSceneEnd(true);

	return ChangeGameScene();
}

void CPendulumScene::Load(){
	texFont.LoadTexture(NTextureResourceInfo::FONT_DOT_AND_SHADOW);
	texTest.LoadTexture(NTextureResourceInfo::TEST);
	texTest.LoadAnimation(NAnimationResourceInfo::TEST);
}

void CPendulumScene::Update(){
	for (int i = 0; i < NUM; ++i)
		body[i].Update();

	Operate();

	{	// ProcDecVel()
		for (int i = 0; i < NUM; i++){
			vel[i] = vel[i] * 0.8f;
			velRotZ[i] *= 0.95f;
		}
	}

	{	// ProcTransGravity()
		for (int i = 1; i < NUM; ++i){
			acc[i] = acc[i] + gravity;
		}
	}


	{	// ProcSpring()
		for (int i = 0; i < NUM; ++i){
			SVector3D	sprAcc(0, 0, 0);
			SVector3D	pos = body[i].GetPositionCenter();

			if (i == 0){
				//sprAcc = sprAcc + CalcSpringAcceleration(pos, body[i + 1].GetPositionCenter());
			}
			else if (i == NUM - 1){
				sprAcc = sprAcc + CalcSpringAcceleration(pos, body[i - 1].GetPositionCenter());
			}
			else{
				// 後ろのバネとの影響
				sprAcc = sprAcc + CalcSpringAcceleration(pos, body[i - 1].GetPositionCenter());

				// 前のバネとの影響
				sprAcc = sprAcc + CalcSpringAcceleration(pos, body[i + 1].GetPositionCenter());
			}

			// 両方の影響を合成させたものを適用する。
			acc[i] = acc[i] + sprAcc;
		}
	}

	{	// ProcRotGravity()
		for (int i = 1; i < NUM; ++i){
			SVector3D org = body[i-1].GetPositionCenter();
			SVector3D pos = body[i].GetPositionCenter();
			SVector3D vecPos = pos - org;
			float r = CalcVector3DLength(&vecPos);

			SVector3D cpPxG = CalcCrossVector3D(&gravity, &vecPos);	// Z成分のみ必要
			float omega = cpPxG.z / r;

			accRotZ[i] += omega * timeStep;
		}
	}

	{	// PrcRotPring
		for (int i = 1; i < NUM; ++i){
			SVector3D org = body[i - 1].GetPositionCenter();
			SVector3D pos = body[i].GetPositionCenter();
			SVector3D vecPos = pos - org;
			float r = CalcVector3DLength(&vecPos);

			SVector3D accDir = acc[i];

			SVector3D cpPxA = CalcCrossVector3D(&accDir, &vecPos);	// Z成分のみ必要
			float omega = cpPxA.z / r;

			accRotZ[i] += omega * timeStep;
		}
	}


	{	// ApllyAccRot()
		for (int i = 0; i < NUM; ++i){
			velRotZ[i] += accRotZ[i];
			accRotZ[i] = 0;
		}
	}

	{	// ApllyVelRot()
		for (int i = 1; i < NUM; ++i){
			SMatrix44 mxRotZ = CalcMatrixRotateZ(velRotZ[i]);
			SVector3D org = body[i - 1].GetPositionCenter();
			SVector3D pos = body[i].GetPositionCenter();
			SVector3D vecPos = pos - org;

			SVector3D newVecPos = CalcVector3DTransform(&mxRotZ, &vecPos);

			SVector3D newPos = newVecPos + org;

			// 新しい座標へのベクトルを求める
			SVector3D newVec = newPos - pos;

			body[i].SetPositionCenter(newPos.x, newPos.y, 0);
			//acc[i] = acc[i] + newVec;
		}
	}

	{	// ApllyAcc()
		for (int i = 0; i < NUM; ++i){
			vel[i] = vel[i] + acc[i];
			acc[i] = SVector3D(0, 0, 0);
		}
	}

	{	// ApllyVel()
		for (int i = 0; i < NUM; ++i){
			body[i].AddPositionCenter(vel[i].x, vel[i].y, vel[i].z);
		}
	}



	SyncSpring();
}

void CPendulumScene::Render(){
	BeginRender3D();

	for (int i = 0; i < NUM - 1; ++i)
		spr[i].Render();

	for (int i = 0; i < NUM; ++i)
		body[i].Render();

	BeginRender2D();

}

void CPendulumScene::RenderDebug(){
	BeginRender2D();

	// タイトルを描画
	txtTitle.Render("PENDULUM TEST SCENE");

	//デバッグ用描画
	SVector2D	basePos_(10, 30);
	txtDbg.SetPositionLeftTop(basePos_.x, basePos_.y);

	for (int i = 0; i < NUM; ++i){
		txtDbg.Render("velRot : %6.4f , accRot : %6.4f", velRotZ[i], accRotZ[i]);
		txtDbg.MoveToNewLine();
		txtDbg.Render("vel.x : %.4f vel.y : %.4f, acc : %6.4f", vel[i].x, vel[i].y,acc[i]);
		txtDbg.MoveToNewLine();
		txtDbg.MoveToNewLine();
	}

	
}


CBaseScene* CPendulumScene::ChangeGameScene(){
	if (!GetIsSceneEnd())
		return this;

	// 終了
	delete this;

	CGameState	gs_;
	gs_.Init(CGameState::TEST, 0);
	return new CGameTestScene(gs_);
}

//----------------------------------------------------------
// ユーティリティ

void CPendulumScene::Operate(){
	float moveSpeed_ = 1.5f;

	// [0] (通常時のプレイヤーの操作)
	if (GetPInput(CONTROLLER_1)->GetHorizontalPositive(HOLD))
		acc[0].x += moveSpeed_;
	else if (GetPInput(CONTROLLER_1)->GetHorizontalNegative(HOLD))
		acc[0].x -= moveSpeed_;
	if (GetPInput(CONTROLLER_1)->GetVerticalPositive(HOLD))
		acc[0].y += moveSpeed_;
	else if (GetPInput(CONTROLLER_1)->GetVerticalNegative(HOLD))
		acc[0].y -= moveSpeed_;

	// 回転
	float rotSpeed = GetPI() / 100.0f;
	float aRot = 0;
	if (GetPInput(CONTROLLER_2)->GetHorizontalPositive(HOLD))
		aRot -= rotSpeed;
	else if (GetPInput(CONTROLLER_2)->GetHorizontalNegative(HOLD))
		aRot += rotSpeed;

	for (int i = 1; i < NUM; ++i)
		accRotZ[i] += aRot;
}

void CPendulumScene::SyncSpring(){
	for (int i = 0; i < NUM - 1; ++i){
		// 始点を合わせる
		spr[i].SetPositionStart(
			body[i].GetPositionCenter().x,
			body[i].GetPositionCenter().y,
			body[i].GetPositionCenter().z
			);
		spr[i].SetPositionEnd(
			body[i + 1].GetPositionCenter().x,
			body[i + 1].GetPositionCenter().y,
			body[i + 1].GetPositionCenter().z
			);
	}
}

SVector3D CPendulumScene::CalcSpringAcceleration(SVector3D _myPos, SVector3D _target){
	SVector3D myPos = _myPos;
	SVector3D tagPos = _target;
	SVector3D objLen = (tagPos - myPos);	// オブジェクト間の長さ
	SVector3D sprAcc(0, 0, 0);

	// 自然超より長い場合のみ影響が出る
	if (CalcVector3DLength(&objLen) > GetNaturalLength()){
		float rad = atan2f(objLen.y, objLen.x);	// ターゲットまでの角度を取得
		SVector3D natuVec(0, 0, 0);
		natuVec.x = GetNaturalLength() * cosf(rad);
		natuVec.y = GetNaturalLength() * sinf(rad);

		SVector3D len = objLen -natuVec;	// 自然長からの変化量(自然超以下は無視する)

		SVector3D sprF = len * GetStiffness();
		SVector3D sprA = sprF / GetMass();

		// 求めた力を加算
		sprAcc = sprA;
	}
	return sprAcc;
}