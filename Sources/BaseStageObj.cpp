//==========================================================
// インクルード
#include	"BaseStageObj.h"
#include	"Allocator.h"

//==========================================================
// 静的領域
const float CBaseStageObject::BLOCK_SIZE = 20;
CTexture *CBaseStageObject::pTexRefBlock = NULL;

//==========================================================
// メソッド定義

CBaseStageObject::CBaseStageObject(){
	block = NULL;
}

CBaseStageObject::~CBaseStageObject(){
	Destroy();
}

void CBaseStageObject::Init(SVector3D _initPos, float _initAngle){
	
}

void CBaseStageObject::Destroy(){
	DeleteArraySafely(block);
}

void CBaseStageObject::Update(){
	// ポリゴンの更新
	for (int i = 0; i < blockNum; ++i){
		GetPBlkockObject(i)->Update();
	}
}

void CBaseStageObject::Sync(){
	for (int i = 0; i < blockNum; ++i){
		GetPBlkockObject(i)->Sync();
	}
}

void CBaseStageObject::Render(){
	for (int i = 0; i < blockNum; ++i){
		GetPBlkockObject(i)->Render();
	}
}