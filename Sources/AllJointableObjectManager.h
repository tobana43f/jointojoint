//==========================================================
// すべてのエリアオブジェクトを管理する
#ifndef _INC_ALL_JOINTABLE_OBJECT_MANAGER_H
#define _INC_ALL_JOINTABLE_OBJECT_MANAGER_H

//==========================================================
// インクルード
#include	"BaseJointableObj.h"

//==========================================================
// クラス定義
class CAllJointableObjectManager {
public:
	//==========================================================
	// 関数

	void Init();
	void Load(int _stageNo);
	void Destroy();
	void Update();
	void Sync();
	void Render();

	// オブジェクトの配列の先頭アドレスを取得する
	CBaseJointableObject**	GetObjArray(){ return allObjArray; }
	int	GetObjNum(){ return objNum; }

private:
	//==========================================================
	// 変数
	
	// リソース
	CTexture	*texBlock;
	CTexture	*texGoal;

	// ステージ内の全ブロックすう
	int objNum;

	// ブロック
	CBaseJointableObject	**allObjArray;

	//==========================================================
	// 関数

	// 識別子で判断して、クラスを確保する関数
	void AllocStageObject(int _id, CBaseJointableObject **_dest);

	// リソースのロード
	void LoadResource();

	// ステージデータのロード
	void LoadObjectData(int _stageNo);

public:
	CAllJointableObjectManager();
	~CAllJointableObjectManager();
};

#endif