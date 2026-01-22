//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS
#include	"AllStageObjectManager.h"
#include	"Allocator.h"

#include	"StageObjectID.h"

#include	"File.h"	// キャラクター情報のロード
#include	<stdio.h>		// 書式付き文字列操作
#include	"AllTextureManager.h"

//==========================================================
// 名前空間
using namespace NStageObjectID;
using namespace NAllTextureManager;

//==========================================================
// ファイル名を定義(ファイル数が少ないのでそれぞれ定義)
// メモ)
// array[ステージ番号][文字列]　
// 　チュートリアルステージは 0 として考える

// データまでのパス
#ifdef _PLATFORM_DIRECTX
static const char	*STAGE_OBJ_DATA_DIRECTORY = "_Resource/Data/StageObject/";
#endif
static const char	*STAGE_OBJ_DATA_EXTENSION = ".csv";

// ファイル名だけを記述
static const char gStageObjDataFileName[7][256] = {
	{ "StageObjectData1_0" },	// チュートリアル
	{ "StageObjectData1_1" },	// 1
	{ "StageObjectData1_2" },	// 2
	{ "StageObjectData1_3" },	// 3
	{ "StageObjectData2_1" },	// 3
	{ "StageObjectData2_2" },	// 3
	{ "StageObjectData2_3" },	// 3
};

//==========================================================
// メソッド定義

CAllStageObjectManager::CAllStageObjectManager(){
	CBaseStageObject::SetTexRefBlock(GetPTexture(NTextureResourceInfo::STAGE_OBJ_BLOCK));
}

CAllStageObjectManager::~CAllStageObjectManager(){
	Destroy();
}

void CAllStageObjectManager::Init(){
	
}

void CAllStageObjectManager::Load(int _stageNo){
	// ブロックのクラスごとに必要になるテクスチャをすべてロードしておく
	LoadResource();
		
	// ブロック情報のロード
	LoadStageObjectData(_stageNo);
}

void CAllStageObjectManager::Destroy(){
	for (int i = 0; i < objNum; ++i){
		DeleteSafely(allObjArray[i]);
	}

	DeleteArraySafely(allObjArray);
}

void CAllStageObjectManager::Update(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Update();
	}
}

void CAllStageObjectManager::Sync(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Sync();
	}
}

void CAllStageObjectManager::Render(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Render();
	}
}

void CAllStageObjectManager::AllocStageObject(int _id, CBaseStageObject **_dest){
	switch (_id){
		case TEST:
			*_dest = new CStageObjTest;
			break;

		case NORMAL_H:
			*_dest = new CStageObjNormal;
			break;

		case LONG_H:
			*_dest = new CStageObjLong;
			break;

		case SHORT_BLOCK:
			*_dest = new CStageObjShort;
			break;

		case SQUARE:
			*_dest = new CStageObjSquare;
			break;

		case NORMAL_V:
			*_dest = new CStageObjNormalV;
			break;

		case LONG_V:
			*_dest = new CStageObjLongV;
			break;

		case CORNER:
			*_dest = new CStageObjCorner;
			break;

		default:
			// error
			break;
	}

}

void CAllStageObjectManager::LoadResource(){
	//texBlock = GetPTexture(NTextureResourceInfo::STAGE_OBJ_BLOCK);
}

void CAllStageObjectManager::LoadStageObjectData(int _stageNo){
	// ファイルパスを作成
	char filePath_[256];
	sprintf(filePath_, "%s%s%s", STAGE_OBJ_DATA_DIRECTORY, gStageObjDataFileName[_stageNo], STAGE_OBJ_DATA_EXTENSION);


	// リソースデータへアクセス
	CFile	file_;
	if (file_.Open(filePath_, CFile::READ)){
		// error
	}

	// キャラクター数を読み込む
	char	header_[256];			// ヘッダ情報の読み込み先
	int		objNum_;				// キャラクター数の読み込み先

	file_.Read("%[^,]", header_);	// ヘッダを読み飛ばす
	file_.Read("%d", &objNum_);		// オブジェクト数を読み込む
	file_.SkipLine();

	// キャラクター用のメモリを確保
	objNum = objNum_;
	allObjArray = new CBaseStageObject*[objNum];

	// キャラクターの初期位置をロード
	file_.Read("%[^,]", header_);		// ヘッダ(オブジェクトID)
	file_.Read("%[^,]", header_);		// ヘッダ(X)
	file_.Read("%[^,]", header_);		// ヘッダ(Y)
	file_.SkipLine();

	int			objID_;
	SVector3D	pos_(0, 0, 0);
	for (int i = 0; i < objNum; ++i){
		// IDを取得
		file_.Read("%d", &objID_);

		// 座標を取得
		file_.Read("%f", &pos_.x);
		file_.Read("%f", &pos_.y);
		pos_.z = 0;
		file_.SkipLine();

		// オブジェクト確保
		AllocStageObject(objID_, &allObjArray[i]);

		// 初期化
		allObjArray[i]->Init(pos_);
	}
}