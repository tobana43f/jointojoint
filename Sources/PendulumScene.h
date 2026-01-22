//==========================================================
// 振り子サンプルを実行するシーン

#ifndef _INC_PENDULUM_SAMPLE_H
#define _INC_PENDULUM_SAMPLE_H

//==========================================================
// インクルード
#include	"BaseScene.h"

#include	"Texture.h"
#include	"PolygonText2D.h"

#include	"Polygon3D.h"
#include	"PolygonLine.h"

//==========================================================
// クラス定義
class CPendulumScene : public CBaseScene {
public:
	//==========================================================
	// 定数
	enum EScene {
		LOAD,
		INIT,
		MAIN,
	};

	float GetStiffness(){ return 0.8f; }
	float GetDamping(){ return 0.0f; }
	float GetMass(){ return 10.0f; }
	float GetNaturalLength(){ return 10.0f; }

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

	static const int NUM = 3;
	CPolygon3D		body[NUM];
	CPolygonLine	spr[NUM - 1];

	SVector3D	gravity;
	float radGrvty;

	float velRotZ[NUM];	
	float accRotZ[NUM];

	SVector3D	vel[NUM];
	SVector3D	acc[NUM];

	//==========================================================
	// 関数

	void Operate();

	void SyncSpring();	// バネ描画を同期

	SVector3D CalcSpringAcceleration(SVector3D _myPos, SVector3D _target);


public:
	CPendulumScene();
	~CPendulumScene();
};

#endif