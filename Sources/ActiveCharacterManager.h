//==========================================================
// アクティブ状態のキャラクターを管理するクラス。
// (= プレイヤーが操作しているキャラクター)
//----------------------------------------------------------
// 注意)
// 　リスト構造の先頭には、ダミーデータがあることに注意すること
// 　実際にキャラクターがいるのは、先頭->NEXT　からである。
#ifndef _ACTIVE_CHARACTER_MANAGER_H
#define _ACTIVE_CHARACTER_MANAGER_H

//==========================================================
// インクルード

#include	"Character.h"
#include	"Texture.h"
#include	"Input.h"	

//==========================================================
// メソッド定義

class CActiveCharacterManager {
public:
	//==========================================================
	// 定数

	//==========================================================
	// 変数

	// 仮)入力
	CInput	*pRefInput1;

	//==========================================================
	// 関数

	void Init();
	void Destroy();
	void Update();
	void Sync();
	void Render();

	// 先頭のキャラクターを設定。(ダミーデータを除く先頭)
	// キャラクターの実態が生成された時に1度だけ呼び出す。
	void SetRefCharacterListTop(CCharacter *_pCharacter);

	// ダミーデータを除くリストの先頭を取得
	CCharacter*	GetPCharacterListHead(){ return pCharacterListHead->next; }
	CCharacter*	GetPCharacterListTail(){ return pCharacterListTail; }

	//----------------------------------------------------------
	// リスト操作

	// リストの最後尾にキャラクターを追加
	void AddCharacterToListTail(CCharacter *_pCharacter);

	// リストの最後尾のキャラクターを外す
	// 戻り値：外したキャラクターのポインタ
	CCharacter* SubCharacterFromListTail();

private:
	//==========================================================
	// 変数

	// リスト管理
	CCharacter	*pCharacterListHead;
	CCharacter	*pCharacterListTail;

	SVector3D	corePullAcc;	// コアが引っ張られる強さ

	//==========================================================
	// 関数

	void ProcDecVelocity();

	// キャラクターの更新
	void UpdateCharacterList();

	// キャラクターの座標を同期する関数(コアとボディの座標を同期させる)
	void SyncCharacterListPosition();

	// キャラクターの描画
	void RenderCharacterList();

	// キャラクターを操作する関数
	void OperateCharacter();

	// バネの影響を計算し適用(キャラクターごとではなく、全体の影響を考慮する)
	void ProcSpringMovement();

	// 止まっているときに先頭のコアを中心へ移動させる
	void ProcAdjustCore();

	// コアの加速度を速度に適用
	void ApllyAcceleration();

	// 回転させる
	void ApllyAngularAcceleration();

	//----------------------------------------------------------
	// ユーティリティ

	SVector3D CalcSpringAcceleration(SVector3D _myPos, SVector3D _tagPos, float _myMass, float _naturalLen);

public:
	CActiveCharacterManager();
	~CActiveCharacterManager();
};

#endif