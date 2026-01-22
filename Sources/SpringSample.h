//==========================================================
// ばねサンプルを実行するシーン
//----------------------------------------------------------
// クラス化したバネ(キャラクター)クラスをテスト
#ifndef _INC_SPRING_SAMPLE_H
#define _INC_SPRING_SAMPLE_H

//==========================================================
// インクルード
#include	"BaseScene.h"

#include	"Texture.h"
#include	"PolygonText2D.h"

#include	"Polygon3D.h"
#include	"PolygonLine.h"

//==========================================================
// クラス定義
class CSpringSampleScene : public CBaseScene {
public:
	//==========================================================
	// 定数
	enum EScene {
		LOAD,
		INIT,
		MAIN,
	};

	float GetStiffness(){ return 1.0f; }
	float GetDamping(){ return 0.0f; }
	float GetMass(){ return 5.0f; }
	float GetNaturalLength(){ return 3.0f; }

	//==========================================================
	// 変数


	//==========================================================
	// 関数	

	void Init();
	void Destroy();
	CBaseScene* Run();

private:
	//==========================================================
	// 関数

	void Load();
	void Update();
	void Render();
	CBaseScene* ChangeGameScene();

	//----------------------------------------------------------
	// ユーティリティ
	void RenderDebug();

	//==========================================================
	// 変数

	// クラス内シーン
	EScene			scene;

	// リソース
	CTexture		texFont;
	CTexture		texTest;

	// デバッグ用に使用するテキスト
	CPolygonText2D	txtTitle;
	CPolygonText2D	txtDbg;

	// タイムステップ
	float timeStep;

	// 剛体
	static const int NUM = 3;
	CPolygon3D	body[NUM];
	SVector3D	vel[NUM];
	SVector3D	acc[NUM];

	float		radGrvty;	// 重力方向の角度

	CPolygonLine spr[NUM - 1];

	void OperateTerminate();	// 両端を操作する。

	void ProcGravity();
	void ProcSpring();	// バネの影響を計算
	void ProcDecVel();	// 減速

	void ApllyAcc();	// 加速度を速度に反映
	void ApllyVel();	// 速度を変位に反映

	void SyncSpring();	// バネ描画を同期
	void ProcCircle();

	// ユーティリティ
	SVector3D CalcSpringAcceleration(SVector3D _myPos, SVector3D _target);


public:
	CSpringSampleScene();
	~CSpringSampleScene();
};

#endif