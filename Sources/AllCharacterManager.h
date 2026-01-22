//==========================================================
// ステージ上に存在する全てのキャラクターを管理する。
//----------------------------------------------------------
// アクティブ状態のキャラクター、ネガティブ状態のキャラクターの実態はこのクラスが所持している。
// アクティブ、ネガティブを管理するそれぞれのクラスには、このクラスが所持している実態のポインタを渡す。
//----------------------------------------------------------
// キャラクターのテクスチャなどのリソースなどもこのクラスが管理する。
#ifndef _INC_ALL_CHARACTER_MANAGER_H
#define _INC_ALL_CHARACTER_MANAGER_H

//==========================================================
// インクルード
#include	"Texture.h"

#include	"Character.h"
#include	"ActiveCharacterManager.h"
#include	"NegativeCharacterManager.h"

#include	"RigidbodyPairManager.h"
#include	"GameState.h"

//==========================================================
// クラス名参照
class CAllStageObjectManager;
class CAllJointableObjectManager;

//==========================================================
// クラス定義
class CAllCharacterManager {
public:
	//==========================================================
	// 関数

	void Init();
	void Load(int _stageNo);
	void Destroy();
	void Update();
	void Sync();
	void Render();

	// あたり判定処理をまとめたもの
	void ProcCollide();

	// 仮)
	void SetRefInput1(CInput* _refInput){ activeCharacterManager.pRefInput1 = _refInput; pRefInput1 = _refInput; }
	CInput *pRefInput1;	// ここでも使えるようにしておく

	CCharacter*	GetPCharacterArray(){ return pCharacterArray; }
	int			GetCharacterNum() { return characterNum; }

	// 先頭(操作中)のキャラクター情報を取得
	CCharacter*	GetPCurCharacter(){ return activeCharacterManager.GetPCharacterListTail(); }
	CActiveCharacterManager* GetPActiveCharManager(){ return &activeCharacterManager; }


	// ステージ内の総キャラクター数を取得する
	int GetAllCharacterNum(){ return characterNum; }
	int GetActiveCharacterNum();

	// 参照する情報を設定する
	static void SetRefStageObj(CAllStageObjectManager *_pRef){ pRefStageObj = _pRef; }
	static void SetRefJointableObj(CAllJointableObjectManager *_pRef){ pRefJointableObj = _pRef;}

private:
	//==========================================================
	// 変数

	//// リソース
	//CTexture	*texCharacterBodyActive;
	//CTexture	*texCharacterBodyNegative;
	//CTexture	*texCharacterCoreActive;
	//CTexture	*texCharacterCoreNegative;
	//CTexture	*texCharacterCable;

	// キャラクターの実態
	CCharacter	*pCharacterArray;	// 配列を確保するポインタの先頭(リストでつなげるわけではない)
	int			characterNum;		// 全キャラクターの数

	// キャラクターの管理
	CActiveCharacterManager		activeCharacterManager;
	CNegativeCharacterManager	negativeCharacterManager;

	// 剛体の登録先
	CRigidbodyPairManager	*pRigidbody;

	// 参照する情報
	static CAllStageObjectManager	*pRefStageObj;
	static CAllJointableObjectManager *pRefJointableObj;

	//==========================================================
	// 関数

	// リソースのロード
	void LoadResource();

	// キャラクターのデータを読み込み
	void LoadCharacterPosition(int _stageNo);

	// リストの初期化
	void InitCharacterList();

	// アクティブのキャラクターのコアと、全てのネガティブのキャラクターのコアをつなぐ場所との判定をする。
	void CollideActiveCoreAndNegativeCoreSpace();

	// ジャンプフラグを更新するための衝突判定(先頭のキャラと、その他キャラ、ステージで判定
	void CollideGround();

	// 繋げられるオブジェクトとのあたり判定
	void CollideJointableObj();

	// コアを外す処理をまとめたもの
	void ProcSeperateCharacter();

	// キャラクターのあたり判定情報同期
	void SyncCharacterCollide();
	
public:
	CAllCharacterManager();
	~CAllCharacterManager();
};


#endif