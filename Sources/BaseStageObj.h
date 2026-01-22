//==========================================================
// ステージオブジェクトのテスト
#ifndef _INC_BASE_STAGE_OBJECT_H
#define _INC_BASE_STAGE_OBJECT_H

//==========================================================
// インクルード
#include	"BlockObject.h"
#include	"Allocator.h"

//==========================================================
// マクロ定義
#undef  PURE_FUNC
#define PURE_FUNC =0

//==========================================================
// クラス定義
class CBaseStageObject {
public:
	//==========================================================
	// 定数
	static const float BLOCK_SIZE;

	//==========================================================
	// 関数

	virtual void Init(SVector3D _initPos, float _initAngle = 0);
	virtual void Destroy();
	virtual void Update();
	virtual void Sync();
	virtual void Render();

	// セッター
	static void SetTexRefBlock(CTexture *_pTex){ pTexRefBlock = _pTex; }

	// ゲッター
	int GetBlockNum() { return blockNum; }
	CBlockObject*	GetPBlkockObject(int _idx){ if (_idx < blockNum){ return &block[_idx]; } return NULL; }
	CBlockObject*	GetBlockObjArray(){ return block; }

protected:
	//==========================================================
	// 変数

	static CTexture *pTexRefBlock;

	// 全体のオブジェクトの
	SVector3D	positionLeftTop;
	float		angleDegree;

	// 使用するブロックオブジェクトの数
	int blockNum;

	// 実際のブロックの配列(確保する)
	CBlockObject	*block;

public:
	CBaseStageObject();
	virtual ~CBaseStageObject();
};

#endif