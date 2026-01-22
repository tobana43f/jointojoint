//==========================================================
// 剛体シミュレーションのを行うシーン
#ifndef _INC_RIGID_BODY_SCENE_H
#define _INC_RIGID_BODY_SCENE_H

//==========================================================
// インクルード
#include	"BaseScene.h"
#include	"PolygonText2D.h"
#include	"Texture.h"

#include	"RigidBody.h"


//==========================================================
// クラス定義
class CRigidbodyScene : public CBaseScene {
public:
	//==========================================================
	// 関数

	void Init();
	void Destroy();
	CBaseScene* Run();

private:
	//==========================================================
	// 変数

	CTexture	texAnimationTest;		// テスト用テクスチャ
	CTexture	texFont;				// 文字列描画用

	// デバッグ関係
	CPolygonText2D	polyTextTitle;
	CPolygonText2D	polyTextDebug;			// 値を見るための

	// オブジェクト
	CRigidBody		objRB;			// 
	CRigidBody		cursor;


	SVector3D		collisionVector;	// 衝突法線、貫通深度の情報
	SVector3D		collisionPosition;	// 衝突座標(ワールド座標(絶対座標で持つ))

	bool			isRigidBodyCollided;
	
	SVector3D		gravity;	// 重力加速度
	float			deltaTime;	// フレームの経過時間
	//----------------------------------------------------------
	// デバッグ用描画に必要な変数

	CPolygonLine		colvec;			// 衝突法線

	CPolygonLine		lineTest;		// Wii側でラインが描画されない原因を特定するために作っておく
	
	CPolygon3D			mincofPos[16];	// ミンコフスキーの差の頂点位置

	static const int	MINCOF_AREA_MAX = 100;
	CPolygonLine		mincofArea[MINCOF_AREA_MAX];	// 衝突法線を求める際に、求まる範囲

	CPolygonLine		dir;

	CPolygon3D			colPos;	

	static const int	LINE_MAX = 100;
	CPolygonLine		colPosLine[LINE_MAX];

	//==========================================================
	// 関数

	void Load();

	// メイン
	void Update();
	void Sync();
	void Render();

	CBaseScene* ChangeGameScene();

	//----------------------------------------------------------
	// ユーティリティ
	
	// デバッグ情報の描画
	void RenderDebug();

	// collideに移す
	bool CollideTriangleAndPoint(SVector3D *_pPosA, SVector3D *_pPosB, SVector3D *_pPosC, SVector3D *_pPosP);


	//----------------------------------------------------------
	// GJKアルゴリズムによる衝突検地
	bool CollideRigidbodyAndRigidBody(CRigidBody *_pRigid1, CRigidBody *_pRigid2);

	// 貫通深度、衝突法線を求める
	// GJKアルゴリズムで衝突している場合にのみ呼び出す。
	// メモ)
	// 　_triangle[3] : 衝突している3頂点の座標(ローカル座標)
	// 　_pRigid1, _pRigid2 ： 判定対象
	// 　_pOutColVec : 求めた衝突法線の格納先
	void CalcCollideVector(SVector3D _triangle[3], CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColVec);

	// テスト用
		

	// 衝突座標を求める
	void CalcCollidePosition(SVector3D *_pColVec, CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColPos);

	//----------------------------------------------------------
	// あたり判定応答

	// 求まった情報からレスポンスをする
	void CollideResponse(CRigidBody *_pRigid1, CRigidBody *_pRigid2);


public:
	CRigidbodyScene();
	~CRigidbodyScene();
};

#endif