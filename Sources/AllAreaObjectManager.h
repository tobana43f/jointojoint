//==========================================================
// すべてのエリアオブジェクトを管理する
#ifndef _INC_ALL_AREA_OBJECT_MANAGER_H
#define _INC_ALL_AREA_OBJECT_MANAGER_H

//==========================================================
// インクルード
#include	"BaseAreaObject.h"

//==========================================================
// クラス定義
class CAllAreaObjectManager {
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
	CBaseAreaObject**	GetObjArray(){ return allObjArray; }
private:
	//==========================================================
	// 変数
	// ステージ内の全ブロックすう
	int objNum;

	// ブロック
	CBaseAreaObject	**allObjArray;

	// 必要なリソース
	//CTexture	*texTutorialBack;
	//CTexture	*texTutorialMove;
	//CTexture	*texTutorialJump;
	//CTexture	*texTutorialJointChar;
	//CTexture	*texTutorialJointObj;
	////CTexture	texTutorialJointLift;
	//CTexture	*texTutorialPendulam;
	//CTexture	*texTutorialSeparateChar;
	////CTexture	texTutorialSeparateObj;
	//CTexture	*texTutorialFall;

	//==========================================================
	// 関数

	// 識別子で判断して、クラスを確保する関数
	void AllocStageObject(int _id, CBaseAreaObject **_dest);

	// リソースのロード
	void LoadResource();

	// ステージデータのロード
	void LoadAreaObjectData(int _stageNo);

public:
	CAllAreaObjectManager();
	~CAllAreaObjectManager();
};

#endif