//==========================================================
// インクルード
#include	"GameTestScene.h"	// 定義
#include	"SpringSample.h"	// 遷移先

#include	"System.h"
#include	"Allocator.h"		// newオーバーライド
#include	"ControllerDefinition.h"
#include	"Math.h"
#include	"GameState.h"

//==========================================================
// 名前空間

using namespace NSystem;
using namespace NControllerDefinition;
using namespace NMath;

//==========================================================
// メソッド定義

CSpringSampleScene::CSpringSampleScene(){
	pCameraOrtho = new CCameraOrthographic;
	pCameraPers = new CCameraPerspective;

	scene = LOAD;
}

CSpringSampleScene::~CSpringSampleScene(){
	DeleteSafely(pCameraOrtho);
	DeleteSafely(pCameraPers);
}

void CSpringSampleScene::Init(){
	//==========================================================
	// カメラの初期化
	pCameraPers->SetCameraPosition(0, 0, -500);
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

	// 剛体として初期化
	for (int i = 0; i < NUM; ++i){
		body[i].Init();
		body[i].SetSize(5, 5);
		body[i].SetPositionCenter(-30.0f + i * 30.0f, 0, 0);
		
		body[i].SetColor(i * (255 / NUM) , 0, 0, 255);

		vel[i] = SVector3D(0, 0, 0);
		acc[i] = SVector3D(0, 0, 0);
	}

	// バネ描画用
	for (int i = 0; i < NUM - 1; ++i){
		spr[i].Init();
		spr[i].SetLineWidth(1);
		SyncSpring();
	}

	// タイムステップ
	timeStep = 1.0f / 60.0f;

	radGrvty = atan2(-1.0f, 0.0f);
}

void CSpringSampleScene::Destroy(){

}

CBaseScene* CSpringSampleScene::Run(){
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

void CSpringSampleScene::Load(){
	texFont.LoadTexture(NTextureResourceInfo::FONT_DOT_AND_SHADOW);
	texTest.LoadTexture(NTextureResourceInfo::TEST);
	texTest.LoadAnimation(NAnimationResourceInfo::TEST);
}

void CSpringSampleScene::Update(){
	for (int i = 0; i < 3; ++i)
		body[i].Update();

	ProcDecVel();
	OperateTerminate();

	ProcGravity();
	ProcSpring();
	ProcCircle();

	ApllyAcc();
	ApllyVel();

	SyncSpring();
}

void CSpringSampleScene::Render(){
	BeginRender3D();

	// バネ描画
	for (int i = 0; i < NUM - 1; ++i)
		spr[i].Render();

	// 剛体描画
	for (int i = 0; i < NUM; ++i)
		body[i].Render();


	BeginRender2D();
	//polyLine.Render();

}

void CSpringSampleScene::RenderDebug(){
	BeginRender2D();

	// タイトルを描画
	txtTitle.Render("SPRING TEST SCENE");

	//デバッグ用描画
	SVector2D	basePos_(10, 30);
	txtDbg.SetPositionLeftTop(basePos_.x, basePos_.y);

}


CBaseScene* CSpringSampleScene::ChangeGameScene(){
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


void CSpringSampleScene::SyncSpring(){
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

// メモ)
// 　わかりやすいように、両端は重力の影響を受けない。
void CSpringSampleScene::ProcGravity(){
	//float gravity = 9.8f * timeStep / 2;
	float gravity = 3.0f;

	for (int i = 1; i < NUM; ++i){
		acc[i].y -= gravity;
	}
}

void CSpringSampleScene::ProcDecVel(){
	for (int i = 0; i < NUM; ++i){
		vel[i] = vel[i] * 0.8f;
	}
}

void CSpringSampleScene::ApllyAcc(){
	for (int i = 0; i < NUM; ++i){
		vel[i] = vel[i] + acc[i];
		acc[i] = SVector3D(0, 0, 0);
	}
}

void CSpringSampleScene::ApllyVel(){
	for (int i = 0; i < NUM; ++i){
		body[i].AddPositionCenter(vel[i].x, vel[i].y, vel[i].z);
	}
}


void CSpringSampleScene::OperateTerminate(){
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

	//[NUM -1] (ジョイント時のプレイヤーの操作)
	if (GetPInput(CONTROLLER_2)->GetHorizontalPositive(HOLD))
		acc[NUM - 1].x += moveSpeed_;
	else if (GetPInput(CONTROLLER_2)->GetHorizontalNegative(HOLD))
		acc[NUM - 1].x -= moveSpeed_;
	if (GetPInput(CONTROLLER_2)->GetVerticalPositive(HOLD))
		acc[NUM - 1].y += moveSpeed_;
	else if (GetPInput(CONTROLLER_2)->GetVerticalNegative(HOLD))
		acc[NUM - 1].y -= moveSpeed_;
}

void CSpringSampleScene::ProcSpring(){
	// 両端のオブジェクトを除くオブジェクトにバネの影響をつける
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

void CSpringSampleScene::ProcCircle(){
	// [0]を原点として回転する
	SVector3D	org = body[0].GetPositionCenter();

	for (int i = 1; i < NUM; ++i){
		// 現在の座標
		SVector3D pos = body[i].GetPositionCenter();

		// 座標を原点からのベクトルに変換
		SVector3D vecDist = pos - org;

		// 現在の速度をベクトルで取得
		SVector3D vVec = vel[i];

		// 速度に変換
		float v = CalcVector3DLength(&vVec);

		// 半径を求める
		float r = CalcVector3DLength(&vecDist);

		// 角度の変化を求める
		float w = v / r;

		// 現在の角度を求める
		float curRad = atan2(vecDist.y, vecDist.x);
	}
}

SVector3D CSpringSampleScene::CalcSpringAcceleration(SVector3D _myPos, SVector3D _target){
	SVector3D myPos = _myPos;
	SVector3D tagPos = _target;
	SVector3D objLen = (tagPos - myPos);	// オブジェクト間の長さ
	SVector3D sprAcc(0, 0, 0);

	// 自然超より長い場合のみ影響が出る
	if (CalcVector3DLength(&objLen) > GetNaturalLength()){
		float rad = atan2(objLen.y, objLen.x);	// ターゲットまでの角度を取得
		SVector3D natuVec(0, 0, 0);
		natuVec.x = GetNaturalLength() * cos(rad);
		natuVec.y = GetNaturalLength() * sin(rad);

		SVector3D len = objLen;;// -natuVec;	// 自然長からの変化量(自然超以下は無視する)

		SVector3D sprF = len * GetStiffness();
		SVector3D sprA = sprF / GetMass();

		// 求めた力を加算
		sprAcc = sprA;
	}
	return sprAcc;
}