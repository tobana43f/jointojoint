//==========================================================
// ネガティブ状態のキャラクターを管理する。
// (=ステージ上に配置されているキャラクター)
#ifndef _INC_NAGATIVE_CHARACTER_MANAGER_H
#define _INC_NAGATIVE_CHARACTER_MANAGER_H

//==========================================================
// インクルード

#include	"Character.h"

//==========================================================
// クラス定義

class CNegativeCharacterManager {
public:
	//==========================================================
	// 関数

	void Init();
	void Destroy();
	void Update();
	void Sync();
	void Render();

	// ダミーデータを除くリストの先頭を取得
	CCharacter*	GetPCharacterListHead(){ return pCharacterListHead->next; }

	//----------------------------------------------------------
	// リスト操作

	// リストの最後尾にキャラクターを追加
	void AddCharacterToListTail(CCharacter *_pCharacter);

	// 引数で渡したキャラクターをリストから外す
	void SubTargetCharacterFromList(CCharacter *_pTarget);

private:
	//==========================================================
	// 変数
	CCharacter	*pCharacterListHead;
	CCharacter	*pCharacterListTail;

	//==========================================================
	// 関数

	// キャラクターの更新
	void UpdateCharacterList();

	// キャラクターの描画
	void RenderCharacterList();

	void ProcSpringMovement();

	SVector3D CalcSpringAcceleration(SVector3D _myPos, SVector3D _tagPos, float _myMass);


public:
	CNegativeCharacterManager();
	~CNegativeCharacterManager();
};

#endif