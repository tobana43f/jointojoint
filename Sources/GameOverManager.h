//==========================================================
// ゲームオーバーエリアを管理する
#ifndef _INC_GAME_OVER_MANAGER_H
#define _INC_GAME_OVER_MANAGER_H

//==========================================================
// インクルード
#include	"AllCharacterManager.h"
#include	"AllStageObjectManager.h"		// ステージオブジェクトの場所を参照する
#include	"AllJointableObjectManager.h"	// 〃
#include	"AABBCollider.h"

//==========================================================
// クラス定義
class CGameOverManager {
public:
	//==========================================================
	//　関数
	void Init();
	void Destroy();
	void Update();
	void Render();

	static void SetGameState(CGameState *_pGameState){ pGameState = _pGameState; }
	static void SetRefInfo(CAllCharacterManager *_pRefChar, CAllStageObjectManager* _pRefStage, CAllJointableObjectManager *_pRefJointable);

private:
	//==========================================================
	// 変数

	// 変更する情報
	static CGameState *pGameState;

	// 参照する情報
	static CAllCharacterManager			*pCharacter;
	static CAllStageObjectManager		*pStageObj;
	static CAllJointableObjectManager	*pJointableObj;

	// ゲームオーバーにならない範囲
	CPolygon3D		polGameArea;
	CAABBCollider	colGameArea;

	float			limitBottom;	// 下端(これより下に行くとゲームオーバー)

	//==========================================================
	// 関数
	
	// ゲームエリアを、ステージオブジェクトなどの情報から設定する。
	void SetGameArea();

	// あたり判定
	void CollideArea();

public:
	CGameOverManager();
	~CGameOverManager();
};

#endif