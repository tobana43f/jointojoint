//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS
#include	"RigidbodyScene.h"
#include	"Allocator.h"
#include	"System.h"
#include	"ControllerDefinition.h"
#include	"Math.h"

#include	"SpringSample.h"

#include	<stdio.h>
//==========================================================
// 名前空間

using namespace NSystem;
using namespace NControllerDefinition;
using namespace NMath;

//==========================================================
// メソッド定義

CRigidbodyScene::CRigidbodyScene()
{
	// 使用するカメラを確保する
	pCameraOrtho = new CCameraOrthographic;
	pCameraPers = new CCameraPerspective;

	Load();
	Init();
}

CRigidbodyScene::~CRigidbodyScene(){
	// カメラ解放
	DeleteSafely(pCameraOrtho);
	DeleteSafely(pCameraPers);
}

void CRigidbodyScene::Init(){
	//----------------------------------------------------------
	// 必須の初期化

	// カメラ初期化
	pCameraPers->SetCameraPosition(0, 0, -300);
	pCameraPers->SetUpDirection(0, 1, 0);
	pCameraPers->SetFocusPosition(0, 0, 0);
	pCameraPers->SetScreenAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
	pCameraPers->SetClipDistance(1, 10000);

	// タイトル用ポリゴン初期化
	polyTextTitle.Init();
	polyTextTitle.SetReferenceTexture(&texFont);
	polyTextTitle.SetSize(15, 20);
	polyTextTitle.SetPositionLeftTop(0, 0);

	// デバッグ表示用ポリゴンを初期化
	polyTextDebug.Init();
	polyTextDebug.SetReferenceTexture(&texFont);
	polyTextDebug.SetSize(12, 16);

	//----------------------------------------------------------
	// オブジェクトの初期化
	deltaTime = 1.0f / DEFAULT_FRAME_RATE;
	gravity = SVector3D(0, -9.8f, 0);

	objRB.Init();
	objRB.SetSize(50, 50);
	objRB.SetPositionCenter(0, 0, 0);
	objRB.SetMass(10);
	
	cursor.Init();
	cursor.SetSize(50, 50);
	cursor.SetPositionCenter(40, 0, 0);
	cursor.SetColor(0, 128, 0,128);
	cursor.SetMass(10);

	// ラインテスト用
	lineTest.Init();
	lineTest.SetLineWidth(10);
	lineTest.SetPositionStart(0, 0, 0);
	lineTest.SetPositionEnd(0, 100, 0);
	lineTest.SetReferenceTexture(&texAnimationTest);

	////////////////////////////////////////////////////////////
	// デバッグ用描画

	// ミンコフスキーの差
	{
		SColor col_[4] = {
			{ 255, 0, 0, 255 },
			{ 255, 255, 0, 255 },
			{ 0, 255, 0, 255 },
			{ 0, 0, 255, 255 }
		};
		for (int i = 0; i < 16; ++i){
			mincofPos[i].Init();
			mincofPos[i].SetSize(2, 2);
			mincofPos[i].SetColor(
				col_[i / 4].r - ((i % 4) * 30),
				col_[i / 4].g - ((i % 4) * 30),
				col_[i / 4].b - ((i % 4) * 30),
				col_[i / 4].a
				);
		}
	}

	// 貫通深度を求める際のエリア
	{
		for (int i = 0; i < MINCOF_AREA_MAX; ++i){
			mincofArea[i].Init();
			mincofArea[i].SetLineWidth(0.5f);
			mincofArea[i].SetColor(255 - i * 40, 0, 255 - i * 20);
		}
	}

	// 基準となるベクトル描画
	{
		dir.Init();
		dir.SetLineWidth(2);
		dir.SetColor(128, 128, 255);
	}

	// 求めたベクトル
	{
		colvec.Init();
		colvec.SetLineWidth(3);
		colvec.SetColor(255, 255, 0);
	}

	// 求めた衝突座標
	{
		colPos.Init();
		colPos.SetSize(2, 2);
		colPos.SetColor(0, 0, 0);
	}

	// 衝突点を探索中の最短距離
	{
		SColor	col[9] = {
			{ 255, 0, 0, 255 },
			{ 255, 255, 0, 255 },
			{ 0, 255, 0, 255 },
			{ 0, 0, 255, 255 },
			{ 128, 0, 0, 255 },
			{ 128, 128, 0, 255 },
			{ 0, 128, 0, 255 },
			{ 0, 0, 128, 255 },
			{ 255,255,255, 255 },
		};
		for (int i = 0; i < LINE_MAX; ++i){
			colPosLine[i].Init();
			colPosLine[i].SetLineWidth(0.5f);

			int idxCol = i / 4;
			if (idxCol >= 8)
				idxCol = 8;

			colPosLine[i].SetColor(
				col[idxCol].r,
				col[idxCol].g,
				col[idxCol].b,
				col[idxCol].a
				);
		}
	}
	////////////////////////////////////////////////////////////
}

void CRigidbodyScene::Destroy(){

}

CBaseScene* CRigidbodyScene::Run(){
	Update();
	Render();
	RenderDebug();

	return ChangeGameScene();
}

CBaseScene* CRigidbodyScene::ChangeGameScene(){
	if (!GetIsSceneEnd())
		return this;

	// 変更
	//return this;
	delete this;
	return new CSpringSampleScene;
}

void CRigidbodyScene::Load(){
	texAnimationTest.LoadTexture(NTextureResourceInfo::TEST);
	texAnimationTest.LoadAnimation(NAnimationResourceInfo::TEST);
	texFont.LoadTexture(NTextureResourceInfo::FONT_DOT_AND_SHADOW);
}

//----------------------------------------------------------
// メイン

void CRigidbodyScene::Update(){
	//----------------------------------------------------------
	// 抵抗
	{
		SVector3D objAcc_(0, 0, 0);
		SVector3D curAcc_(0, 0, 0);
		objRB.SetAcceleration(objAcc_);
		cursor.SetAcceleration(curAcc_);

		SVector3D	objV_ = objRB.GetVelocity() * 0.8f;
		SVector3D	curV_ = cursor.GetVelocity() * 0.8f;

		objRB.SetVelocity(objV_);
		cursor.SetVelocity(curV_);
	}


	//----------------------------------------------------------
	// オブジェクトを操作する。
	{
		// カーソルを移動
		float msObj = 0.5f;
		float msCur = 0.5f;

		float asObj = -0.52f;
		float asCur = 0.01f;

		if (GetPInput(CONTROLLER_1)->GetHorizontalPositive(HOLD))
			cursor.AddAcceleration(SVector3D(msCur, 0, 0));
		else if (GetPInput(CONTROLLER_1)->GetHorizontalNegative(HOLD))
			cursor.AddAcceleration(SVector3D(-msCur, 0, 0));
		if (GetPInput(CONTROLLER_1)->GetVerticalPositive(HOLD))
			cursor.AddAcceleration(SVector3D(0, 5, 0));
		else if (GetPInput(CONTROLLER_1)->GetVerticalNegative(HOLD))
			cursor.AddAcceleration(SVector3D(0, -msCur, 0));

		if (GetPInput(CONTROLLER_1)->GetSeperate(HOLD)){
			cursor.AddAngularVelocityDegree(asCur);
		}

		// オブジェクト移動
		if (GetPInput(CONTROLLER_2)->GetHorizontalPositive(HOLD))
			objRB.AddAcceleration(SVector3D(msObj, 0, 0));
		else if (GetPInput(CONTROLLER_2)->GetHorizontalNegative(HOLD))
			objRB.AddAcceleration(SVector3D(-msObj, 0, 0));
		if (GetPInput(CONTROLLER_2)->GetVerticalPositive(HOLD))
			objRB.AddAcceleration(SVector3D(0, msObj, 0));
		else if (GetPInput(CONTROLLER_2)->GetVerticalNegative(HOLD))
			objRB.AddAcceleration(SVector3D(0, -msObj, 0));

		if (GetPInput(CONTROLLER_1)->GetSeperate(HOLD)){
			objRB.AddAngularVelocityDegree(asObj);
		}
		objRB.SetAngleDegree(0, 0, 45);
	}

	//----------------------------------------------------------
	// 重力
	{
		SVector3D gAccel_ = gravity / 10.0f;// *deltaTime;// *deltaTime;
		cursor.AddAcceleration(gAccel_);
	}

	//----------------------------------------------------------
	// 剛体を更新
	{
		objRB.AddVelocity(objRB.GetAcceleration());
		cursor.AddVelocity(cursor.GetAcceleration());

		objRB.Update();
		cursor.Update();
	}


	//----------------------------------------------------------
	// あたり判定
	// 判定
	bool isCollide_ = CollideRigidbodyAndRigidBody(&objRB, &cursor);
	//bool isCollide_ = CollideRigidbodyAndRigidBody(&cursor, &objRB);
	if (isCollide_)
		objRB.SetColor(128, 0, 0,128);
	else
		objRB.SetColor(0, 0, 128,128);



	//----------------------------------------------------------
	// あたり判定応答
	CollideResponse(&objRB, &cursor);


	//----------------------------------------------------------
	// 同期
	Sync();

	//----------------------------------------------------------
	// 画面遷移
	if (GetPInput(CONTROLLER_1)->GetJump(TRIGGER)){
		SetIsSceneEnd(true);
	}
}

void CRigidbodyScene::Sync(){
	objRB.Sync();
	cursor.Sync();
}

void CRigidbodyScene::Render(){
	BeginRender3D();

	// 剛体の衝突判定検出用
	objRB.Render();
	cursor.Render();

	////////////////////////////////////////////////////////////
	// デバッグ用描画

	// ---- ---- ----
	dir.Render();	// 基準ベクトル

	// ---- ---- ----
	colvec.Render();	// 貫通深度ベクトル

	// ---- ---- ----
	for (int i = 0; i < 16; ++i){
		mincofPos[i].Render();
	}

	// ---- ---- ----
	for (int i = 0; i < MINCOF_AREA_MAX; ++i){
		mincofArea[i].Render();	// 衝突範囲
	}

	// ---- ---- ----
	colPos.Render();	// 衝突座標

	// ---- ---- ----
	// 衝突点と最短めり込み衝突距離の探索の描画
	for (int i = 0; i < LINE_MAX; ++i){
		//colPosLine[i].Render();
	}

	// ---- ---- ----
	int idxVx_ = 1;
	for (int i = idxVx_ * 4; i < (idxVx_ + 1) * 4; ++i){
		//colPosLine[i].Render();
	}
	////////////////////////////////////////////////////////////
	
	// ラインテスト
	//lineTest.Render();

	BeginRender2D();
	RenderDebug();
}

//----------------------------------------------------------
// ユーティリティ


void CRigidbodyScene::RenderDebug(){
	BeginRender2D();
	//----------------------------------------------------------
	// タイトル名の描画
	polyTextTitle.Render("RIGID BODY SAMPLE SCENE");

	//----------------------------------------------------------
	// デバッグ用文字列の設定
	SVector2D	basePos_(10, 25);
	polyTextDebug.SetPositionLeftTop(basePos_.x, basePos_.y);

	//----------------------------------------------------------
	// 座標を描画する際のフォーマット
	char posFormat_[] = "%s : X = %6.1f, Y = %6.1f, Z = %6.1f";
	
	//----------------------------------------------------------
	// 文字列描画

	{
		SVector3D p_ = objRB.GetPositionCenter();
		polyTextDebug.Render(posFormat_, "obj" ,p_.x, p_.y, p_.z);
		polyTextDebug.MoveToNewLine();
	};

	{
		SVector3D p_ = cursor.GetPositionCenter();
		polyTextDebug.Render(posFormat_, "cursor", p_.x, p_.y, p_.z);
		polyTextDebug.MoveToNewLine();
	};



	// ラインの頂点座標
	SVector3D	vxPos_[4];
	for (int i = 0; i < 4; ++i){
		vxPos_[i] = lineTest.CalcVertexPosition((CPolygonLine::EVertexID)i);

		char str_[30];
		sprintf(str_, "vx[%d]", i);
	}

}

// collide系に移動させること
bool CRigidbodyScene::CollideTriangleAndPoint(SVector3D *_pPosA, SVector3D *_pPosB, SVector3D *_pPosC, SVector3D *_pPosP){
	// 判定する三角形の各頂点座標を求める
	SVector3D	vxPos_[3];
	vxPos_[0] = *_pPosA;
	vxPos_[1] = *_pPosB;
	vxPos_[2] = *_pPosC;

	// 三角形の各辺へのベクトルを取得
	SVector3D	edgeVec_[3];
	edgeVec_[0] = vxPos_[1] - vxPos_[0];	// 0→1のベクトル
	edgeVec_[1] = vxPos_[2] - vxPos_[1];	// 1→2のベクトル
	edgeVec_[2] = vxPos_[0] - vxPos_[2];	// 2→0のベクトル

	// vxPos[0]から見た辺の向きベクトルを得る
	SVector3D	ce1_ = vxPos_[1] - vxPos_[0];
	SVector3D	ce2_ = vxPos_[2] - vxPos_[0];
	int			ccw = 1;

	// 判定する法線方向を求める
	if (CalcCrossVector3D(&ce1_, &ce2_).z < 0.0f)
		ccw = -1;

	// 点が三角形の内側にあるかを判定
	for (int i = 0; i < 3; ++i){
		SVector3D	cp_ = *_pPosP - vxPos_[i];
		if (CalcCrossVector3D(&edgeVec_[i], &cp_).z * ccw <= 0)
			return false;
	}

	// 点が三角形の中に存在する
	return true;
}

// GJK (?)(衝突している時でも、三角形が、直線になってしまう場合に判定できない(ぴったり０になる場合))
bool CRigidbodyScene::CollideRigidbodyAndRigidBody(CRigidBody *_pRigid1, CRigidBody *_pRigid2){
	//----------------------------------------------------------
	// _pRigid1をの中心を原点とするローカル座標に変換
	SVector3D	posRB1_ = _pRigid1->GetPositionCenter() - _pRigid1->GetPositionCenter();
	SVector3D	posRB2_ = _pRigid2->GetPositionCenter() - _pRigid1->GetPositionCenter();
	
	//----------------------------------------------------------
	// 各頂点へのベクトルを求める
	SVector3D	vecRB1_[4];
	SVector3D	vecRB2_[4];

	for (int i = 0; i < 4; ++i){
		vecRB1_[i] = _pRigid1->CalcVertexPosition(i) - _pRigid1->GetPositionCenter();
		vecRB2_[i] = _pRigid2->CalcVertexPosition(i) - _pRigid2->GetPositionCenter();
	}

	// ミンコフスキー差の頂点の座標を取得
	SVector3D	vxPosMincof_[16];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			vxPosMincof_[j + i * 4] = posRB2_ + vecRB2_[i] - vecRB1_[j];
		}
	}

	//----------------------------------------------------------
	// 再帰的に判定する単体(三角形)を求めていく
	// 衝突していない場合は、最接近点が求まると終了
	SVector3D	original(0, 0, 0);				// 原点(posRB1_)
	SVector3D	vecDir_ = posRB2_ - original;	// ある方向ベクトル

	bool		isCollide_ = false;				// 衝突フラグ
	SVector3D	posShortest_[3];				// 衝突を判定する三角形を作る頂点を格納する
	int			cntShortest_ = 0;				// 選択された頂点の数
	SVector3D	posExcluded_[16];				// 一度選択された頂点を格納していき、除外する。(最大でもミンコフスキーの差の頂点の数にしかならない)
	int			cntExcluded_ = 0;				// 除外された頂点の数

	// 配列初期化
	for (int i = 0; i < 3; ++i)
		posShortest_[i] = SVector3D(_pRigid2->GetPositionCenter());

	for (int i = 0; i < 16; ++i)
		posExcluded_[i] = SVector3D(_pRigid2->GetPositionCenter());

	//----------------------------------------------------------
	// ミンコフスキの差の頂点のうち、ベクトル方向に一番遠い点を選ぶ
	while (true){
		//----------------------------------------------------------
		// 原点に向かう線と、ベクトルの始点から最も遠い点を求める
		float vecFar_ = -1;										// 最も遠いい点までの距離を格納する
		for (int i = 0; i < 16; ++i){
			SVector3D vecDirSuv_ = CalcVector3DNormal(&vecDir_) * -99999;			// 線を伸ばして、無限に続く線とする。
			SVector3D crossPos_(0, 0, 0);
			CalcShortestDistXY(&vecDir_, &vecDirSuv_, &vxPosMincof_[i], &crossPos_);

			// 最長距離を更新
			SVector3D	crossVec_ = vecDir_ - crossPos_;
			float len_ = CalcVector3DLength(&crossVec_);		// ベクトル方向に最も遠い点と距離を求める。
			if (vecFar_ < len_){
				vecFar_ = len_;									// 最も遠いい点を更新
				posShortest_[cntShortest_] = vxPosMincof_[i];	// 遠い場合は、ミンコフスキーの差上の点を格納
			}
		}
		
		//----------------------------------------------------------
		// 再帰処理準備
		if (cntShortest_ == 0){
			// 求まった点から原点に向かうベクトルを使う
			vecDir_ = posShortest_[cntShortest_] - original;

			// 選んだ点を除外
			posExcluded_[cntExcluded_] = posShortest_[0];

			// 次の点へ
			cntShortest_++;
			cntExcluded_++;
		}
		else if (cntShortest_ == 1){
			// 2点目の頂点が求まらなかった場合は衝突していない
			if (posShortest_[1] == SVector3D(_pRigid2->GetPositionCenter()))
				break;

			// 除外したデータを再度選んだ場合
			//if (posShortest_[1] == posExcluded_[0])
			//	break;

			// 求まった2つの点を結んだ線分と、原点の最短距離を求め、その点から原点に向かうベクトルを使う
			SVector3D	pos3(0, 0, 0);
			CalcShortestDistXY(&posShortest_[0], &posShortest_[1], &original, &pos3);
			vecDir_ = pos3 - original;

			// 今選んだ点を除外
			posExcluded_[cntExcluded_] = posShortest_[1];

			cntShortest_++;
			cntExcluded_++;
		}
		else if (cntShortest_ == 2){
			// すでに選択されている頂点を選んだ場合
			bool isExcluded_ = false;	// 多重ループから抜け出すため
			for (int i = 0; i < 16; ++i){
				if (posShortest_[2] == posExcluded_[i]){  
					isExcluded_ = true;
					break;
				}
			}
			if (isExcluded_)
				break;
			
			// 現在選ばれた点を除外する
			posExcluded_[cntExcluded_] = posShortest_[2];
			cntExcluded_++;

			// ３点をつなぐ三角形内に原点があるかどうかを調べる。
			isCollide_ = CollideTriangleAndPoint(&posShortest_[0], &posShortest_[1], &posShortest_[2], &original);
			// 三角形内にある場合は衝突している
			if (isCollide_)
				break;

			//----------------------------------------------------------
			// 現在の三角形に衝突していない場合

			// 原点から一番遠い頂点を探す。
			float	farestDist_ = CalcVector3DLength(&posShortest_[0]);
			int		indexTarget_ = 0;
			for (int i = 1; i < 3; ++i){
				float dist_ = CalcVector3DLength(&posShortest_[i]);

				if (dist_ > farestDist_){
					farestDist_ = dist_;
					indexTarget_ = i;
				}
			}

			// １番遠い点を外し、配列を整列する
			// メモ)[2]が不要な頂点
			switch(indexTarget_){
				case 0:
					posShortest_[0] = posShortest_[1];
				case 1:
					posShortest_[1] = posShortest_[2];

			}
			posShortest_[2] = SVector3D(0, 0, 0);	// 不要なので初期化

			// 最接近点を探し、
			// 新たな基準ベクトルを作成。
			SVector3D	pos3(0, 0, 0);
			CalcShortestDistXY(&posShortest_[0], &posShortest_[1], &original, &pos3);
			vecDir_ = pos3 - original;
		}
		else{
			break;
		}
	}

	////////////////////////////////////////////////////////////
	// 描画初期化
	for (int i = 0; i < 16; ++i){
		SVector3D	p_(0, 0, 0);
		mincofPos[i].SetPositionCenter(p_.x, p_.y, p_.z);
	}

	dir.SetPositionStart(0, 0, 0);
	dir.SetPositionEnd(0, 0, 0);

	for (int i = 0; i < 3; ++i){
		SVector3D	init_(0, 0, 0);
		mincofArea[i].SetPositionStart(init_.x, init_.y, init_.z);
		mincofArea[i].SetPositionEnd(init_.x, init_.y, init_.z);
	}

	colvec.SetPositionStart(0, 0, 0);
	colvec.SetPositionEnd(0, 0, 0);
	colPos.SetPositionCenter(0, 0, 0);
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	// 三角形を描画
	SVector3D vecDirSuv_ = CalcVector3DNormal(&vecDir_) * -99999;
	dir.SetPositionStart(vecDir_.x, vecDir_.y, vecDir_.z);
	dir.SetPositionEnd(vecDirSuv_.x, vecDirSuv_.y, vecDirSuv_.z);

	// ミンコフスキーの差の頂点
	for (int i = 0; i < 16; ++i){
		SVector3D	p_ = vxPosMincof_[i] + _pRigid1->GetPositionCenter();
		mincofPos[i].SetPositionCenter(p_.x, p_.y, p_.z);
	}

	for (int i = 0; i < 3; ++i){
		SVector3D	s_ = posShortest_[i] + _pRigid1->GetPositionCenter();
		SVector3D	e_ = posShortest_[(i + 1) % 3] + _pRigid1->GetPositionCenter();
		mincofArea[i].SetPositionStart(s_.x, s_.y, s_.z);
		mincofArea[i].SetPositionEnd(e_.x, e_.y, e_.z);
	}
	////////////////////////////////////////////////////////////


	// 衝突情報を初期化
	collisionVector = SVector3D(0, 0, 0);
	collisionPosition = SVector3D(0, 0, 0);
	isRigidBodyCollided = false;

	// 衝突なし
	if (!isCollide_)
		return isCollide_;

	isRigidBodyCollided = true;


	//----------------------------------------------------------
	// 衝突があった場合、貫通深度、衝突法線を求める
	CalcCollideVector(posShortest_, _pRigid1, _pRigid2, &collisionVector);
	
	////////////////////////////////////////////////////////////
	// ベクトル描画
	colvec.SetPositionStart(
		_pRigid1->GetPositionCenter().x,
		_pRigid1->GetPositionCenter().y,
		_pRigid1->GetPositionCenter().z);
	colvec.SetPositionEnd(
		collisionVector.x + _pRigid1->GetPositionCenter().x,
		collisionVector.y + _pRigid1->GetPositionCenter().y,
		collisionVector.z + _pRigid1->GetPositionCenter().z
		);
	////////////////////////////////////////////////////////////

	//----------------------------------------------------------
	// 衝突座標を求める
	CalcCollidePosition(&collisionVector, _pRigid1, _pRigid2, &collisionPosition);

	colPos.SetPositionCenter(
		collisionPosition.x,
		collisionPosition.y,
		collisionPosition.z);

	return isCollide_;
}

// 貫通深度(OK)
void CRigidbodyScene::CalcCollideVector(SVector3D _triangle[3], CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColVec){
	////////////////////////////////////////////////////////////
	// デバッグ用描画初期化

	// ミンコフスキーの差の頂点を描画
	for (int i = 0; i < 16; ++i){
		mincofPos[i].SetPositionCenter(0, 0, 0);
	}

	// エリアを初期化
	for (int i = 0; i < MINCOF_AREA_MAX; ++i){
		mincofArea[i].SetPositionStart(0, 0, 0);
		mincofArea[i].SetPositionEnd(0, 0, 0);
	}

	// 基準のベクトル
	dir.SetPositionStart(0, 0, 0);
	dir.SetPositionEnd(0, 0, 0);

	// 衝突法線
	colvec.SetPositionStart(0, 0, 0);
	colvec.SetPositionEnd(0, 0, 0);

	////////////////////////////////////////////////////////////

	//==========================================================
	// _pRigid1をの中心を原点とするローカル座標に変換
	SVector3D	posRB1_ = _pRigid1->GetPositionCenter() - _pRigid1->GetPositionCenter();
	SVector3D	posRB2_ = _pRigid2->GetPositionCenter() - _pRigid1->GetPositionCenter();

	//----------------------------------------------------------
	// 各頂点へのベクトルを求める
	SVector3D	vecRB1_[4];
	SVector3D	vecRB2_[4];
	for (int i = 0; i < 4; ++i){
		vecRB1_[i] = _pRigid1->CalcVertexPosition(i) - _pRigid1->GetPositionCenter();
		vecRB2_[i] = _pRigid2->CalcVertexPosition(i) - _pRigid2->GetPositionCenter();
	}

	// ミンコフスキー差の頂点の座標を取得
	SVector3D	vxPosMincof_[16];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			vxPosMincof_[j + i * 4] = posRB2_ + vecRB2_[i] - vecRB1_[j];
		}
	}

	////////////////////////////////////////////////////////////
	// ミンコフスキーの差の頂点を描画
	for (int i = 0; i < 16; ++i){
		SVector3D	p_ = vxPosMincof_[i] + _pRigid1->GetPositionCenter();
		mincofPos[i].SetPositionCenter(p_.x, p_.y, p_.z);
	}
	//return;
	////////////////////////////////////////////////////////////

	//==========================================================
	// 引数の衝突中三角形を頂点に格納
	SVector3D	original(0, 0, 0);	// 原点の位置
	SVector3D	colVec_(0, 0, 0);	// 貫通深度を格納する

	int			MAX_VX_AREA = 100;	// 多角形を構成する頂点を格納する配列関係
	SVector3D	posVxArea_[100];
	int			cntVxArea_ = 0;		// 多角形を構成している頂点の数

	// 引数から受け取った頂点を格納
	for (int i = 0; i < 3; ++i){
		posVxArea_[i] = _triangle[i];
	}
	cntVxArea_ = 3;	// 3固定

	//==========================================================
	// ↓ここからは、多角形を構成していく
	// 　原点から、めり込んでいる最短距離を探す
	//==========================================================

	//==========================================================
	// 原点から、中心座標に向かうベクトルを取得
	SVector3D	baseDir_(0, 0, 0);	// 基準となるベクトルを設定する
	int			idxEdgeStart = 0;	// 頂点を挿入する辺の終点側の添え字

	while (true){
		//==========================================================
		// 原点から多角形を構成している辺の最短距離を求め、原点からその頂点へのベクトルを取得する。
		{
			float lenShortest_ = 999999999999999999.9f;	// 適当にでかい値で初期化
			for (int i = 0; i < cntVxArea_; ++i){
				SVector3D	s_ = posVxArea_[i];						// 辺の始点位置を取得
				SVector3D	e_ = posVxArea_[(i + 1) % cntVxArea_];	// 辺の終点位置を取得
				SVector3D	cp_;									// 辺と原点の最短距離の交差点を取得
				float		len_;									// 辺と原点の最短距離を取得

				len_ = CalcShortestDistXY(&s_, &e_, &original, &cp_);

				if (len_ < lenShortest_){
					lenShortest_ = len_;							// 最短距離として設定
					baseDir_ = cp_ - original;						// 原点から最短距離の点へ向かうベクトルを基準とする。

					// 正しい?
					idxEdgeStart = (i + 1) % cntVxArea_;				// 最短距離となった辺の始点側の添え字を取得
				}
			}
		}

		//////////////////////////////////////////////////////////////
		//// ベクトルを描画
		//{
		//	SVector3D	s_ = original;
		//	SVector3D	e_ = baseDir_ * 999999;
		//	dir.SetPositionStart(s_.x, s_.y, s_.z);
		//	dir.SetPositionEnd(e_.x, e_.y, e_.z);
		//	//return;
		//}
		//////////////////////////////////////////////////////////////

		//==========================================================
		// 基準のベクトル方向で一番[遠いい]ミンコフスキーの差の頂点を取得する。
		SVector3D	newPos_(0, 0, 0);									// 追加する頂点
		{
			float lenFarest = -1;										// 初期値は無効な値を入れておく
			for (int i = 0; i < 16; ++i){
				SVector3D	baseDirInf_ = baseDir_ * 999999999999.9f;	// 線を伸ばして無限の長さと仮定する。
				SVector3D	vxMincof_;									// 頂点の座標(原点からのベクトルに等しい)

				// ミンコフスキーの差の点と、ベクトルとの交差点を求める
				CalcShortestDistXY(&original, &baseDirInf_, &vxPosMincof_[i], &vxMincof_);
				// 交差点への距離をもとめる
				float len_ = CalcVector3DLength(&vxMincof_);

				// 最遠点判断
				if (len_ > lenFarest){
					lenFarest = len_;			// 最遠距離を更新
					newPos_ = vxPosMincof_[i];	// 追加する頂点として保存
				}
			}
		}

		//==========================================================
		// 取得したミンコフスキーの差の頂点が、すでに多角形を構成している場合(再度同じ点が選ばれた場合) 
		{
			bool isFindShortest = false;
			for (int i = 0; i < cntVxArea_; ++i){
				if (newPos_ == posVxArea_[i]){
					isFindShortest = true;	// 最短距離が求まったので、終了
				}
			}

			if (isFindShortest)
				break;
		}

		//==========================================================
		// 取得した最遠のミンコフスキーの差の頂点を、多角形に追加する
		{
			// 頂点を挿入する場所まで、配列を後ろにずらし、挿入する場所を開ける
			for (int i = MAX_VX_AREA - 1; i > idxEdgeStart; --i){
				posVxArea_[i] = posVxArea_[i - 1];
			}
			posVxArea_[idxEdgeStart] = newPos_;	// 新しい頂点を挿入	
			cntVxArea_++;						// 頂点数を加算
		}

	}

	//==========================================================
	// 最短距離(貫通深度が求まった場合)
	*_pOutColVec = baseDir_;

	////////////////////////////////////////////////////////////
	// エリア描画
	for (int i = 0; i < cntVxArea_; ++i){
		SVector3D	s_ = posVxArea_[i] + _pRigid1->GetPositionCenter();
		SVector3D	e_ = posVxArea_[(i + 1) % cntVxArea_] + _pRigid1->GetPositionCenter();
		mincofArea[i].SetPositionStart(s_.x, s_.y, s_.z);
		mincofArea[i].SetPositionEnd(e_.x, e_.y, e_.z);
	}
	////////////////////////////////////////////////////////////
}

// 衝突座標を求める(OK)
void CRigidbodyScene::CalcCollidePosition(SVector3D *_pColVec, CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColPos){
	////////////////////////////////////////////////////////////
	// デバッグ用初期化
	for (int i = 0; i < LINE_MAX; ++i){
		colPosLine[i].SetPositionStart(0, 0, 0);
		colPosLine[i].SetPositionEnd(0, 0, 0);
	}
	int idxLine_ = 0;

	////////////////////////////////////////////////////////////

	//----------------------------------------------------------
	// 剛体の位置を求める
	SVector3D	posRB1_ = _pRigid1->GetPositionCenter();
	SVector3D	posRB2_ = _pRigid2->GetPositionCenter();

	//----------------------------------------------------------
	// 剛体の中心座標から、各頂点へのベクトルを求める
	SVector3D	vxRB1_[4];
	SVector3D	vxRB2_[4];

	for (int i = 0; i < 4; ++i){
		vxRB1_[i] = _pRigid1->CalcVertexPosition(i) - _pRigid1->GetPositionCenter();
		vxRB2_[i] = _pRigid2->CalcVertexPosition(i) - _pRigid2->GetPositionCenter();
	}

	//----------------------------------------------------------
	// 衝突した剛体を、衝突法線方向に多めに移動
	SVector3D	moveDist_ = *_pColVec * -1.2f;			// めり込み分を、反対方向に少し多めに(少し多めにするのは、確実に衝突をしてない状態にするため)
	SVector3D	movedPosRB2_ = posRB2_ + (moveDist_);	// 移動後のRB2の中心座標
	SVector3D	movedPosRB1_ = posRB1_ - (moveDist_);
		
	//----------------------------------------------------------
	// 各頂点と、各辺の最短距離を求める
	SVector3D	colPos_(0, 0, 0);
	float		shortestDist_ = CalcShortestDistXY(&SVector3D(posRB1_ + vxRB1_[0]), &SVector3D(posRB1_ + vxRB1_[1]), &SVector3D(movedPosRB2_ + vxRB2_[0]), &colPos_);

	// _rb1 の辺と、rb2の頂点
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			SVector3D p_(movedPosRB2_ + vxRB2_[i]);			// 点
			SVector3D s_(posRB1_ + vxRB1_[j]);				// 辺の始点
			SVector3D e_(posRB1_ + vxRB1_[(j + 1) % 4]);	// 辺の終点
			SVector3D crossPos_;							// 交差点

			float len_ = CalcShortestDistXY(&s_, &e_, &p_, &crossPos_);

			// 最短距離を更新
			if (len_ < shortestDist_){
				shortestDist_ = len_;
				colPos_ = crossPos_;
			}

			////////////////////////////////////////////////////////////
			// 線を作成
			colPosLine[idxLine_].SetPositionStart(p_.x, p_.y, p_.z);
			colPosLine[idxLine_].SetPositionEnd(crossPos_.x, crossPos_.y, crossPos_.z);
			idxLine_++;
			////////////////////////////////////////////////////////////

		}
	}

	// _rb2 の辺と、rb1の頂点
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			SVector3D p_(movedPosRB1_ + vxRB1_[i]);					// 点
			SVector3D s_(posRB2_ + vxRB2_[j]);				// 辺の始点
			SVector3D e_(posRB2_ + vxRB2_[(j + 1) % 4]);	// 辺の終点
			SVector3D crossPos_;								// 交差点

			float len_ = CalcShortestDistXY(&s_, &e_, &p_, &crossPos_);

			// 最短距離を更新
			if (len_ < shortestDist_){
				shortestDist_ = len_;
				colPos_ = crossPos_;
			}

			////////////////////////////////////////////////////////////
			// 線を作成
			colPosLine[idxLine_].SetPositionStart(p_.x, p_.y, p_.z);
			colPosLine[idxLine_].SetPositionEnd(crossPos_.x, crossPos_.y, crossPos_.z);
			idxLine_++;
			////////////////////////////////////////////////////////////
		}
	}

	if (_pOutColPos != NULL)
		*_pOutColPos = colPos_;
}

void CRigidbodyScene::CollideResponse(CRigidBody *_pRigid1, CRigidBody *_pRigid2){
	if (!isRigidBodyCollided)
		return;


	SVector3D test_ = collisionVector * -1;
	_pRigid2->AddPositionCenter(test_.x, test_.y, test_.z);

	_pRigid2->SetVelocity(0,0,0);
}