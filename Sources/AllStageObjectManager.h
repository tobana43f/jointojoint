//==========================================================
// ステージ上のオブジェクトを管理するクラス
#ifndef _INC_ALL_STAGE_OBJECT_MANAGER_H
#define _INC_ALL_STAGE_OBJECT_MANAGER_H

//==========================================================
// インクルード
#include	"Texture.h"

#include	"BaseStageObj.h"

//==========================================================
// クラス定義
class CAllStageObjectManager {
public:
	//==========================================================
	// 変数

	//==========================================================
	// 関数
	
	void Init();
	void Load(int _stageNo);
	void Destroy();
	void Update();
	void Sync();
	void Render();

	// オブジェクトの配列の先頭アドレスを取得する
	CBaseStageObject**	GetStageObjArray(){ return allObjArray; }
	int	GetObjNum(){ return objNum; }

private:
	//==========================================================
	// 変数

	// 全てのリソース
	//CTexture			*texBlock;

	// ステージ内の全ブロックすう
	int objNum;	

	// ブロック
	CBaseStageObject	**allObjArray;

	//==========================================================
	// オブジェクトの種類のメモリを確保
	void AllocStageObject(int _id, CBaseStageObject **_dest);

	// リソースのロード
	// メモ)
	// 　ブロックを定義したクラスで必要になるテクスチャはすべてここでロードする。
	void LoadResource();

	// ステージオブジェクトの情報をロード
	void LoadStageObjectData(int _stageNo);

public:
	CAllStageObjectManager();
	~CAllStageObjectManager();

};

#endif