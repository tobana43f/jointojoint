//==========================================================
// ゲームで使用するカメラの操作を担当する。
#ifndef _INC_GAME_CAMERA_MANAGER_H
#define _INC_GAME_CAMERA_MANAGER_H

//==========================================================
// インクルード

#include	"CameraPerspective.h"
#include	"AllCharacterManager.h"

//==========================================================
// クラス定義
class CGameCameraManager {
public:
	//==========================================================
	// 関数

	void Init();
	void Update();
	
	// セッター
	void SetPCamera(CCameraPerspective *_pCamera){ pCamera = _pCamera; }
	
	void SetPAllCharacterManager(CAllCharacterManager *_pCharaManager){ pAllCharacterManager = _pCharaManager; }

private:
	//==========================================================
	// 変数

	CCameraPerspective		*pCamera;	// 操作するカメラ

	CAllCharacterManager	*pAllCharacterManager;	// キャラクターを参照する

	SVector3D				velocity;	// カメラの移動速度

	float					initDist;		// 初期の距離
	float					curDist;		// カメラの距離

	//==========================================================
	// 関数

	// カメラの制御
	void ControlCamera();

	// カメラの動き方
	void MoveNormal();	// キャラクターの座標を見る

	void MoveSpring();	// ばねの動きを使った追従

	void MoveJointObj();

	// カメラの距離を更新
	void UpdateDistance();

public:
	CGameCameraManager();
	~CGameCameraManager();
};

#endif