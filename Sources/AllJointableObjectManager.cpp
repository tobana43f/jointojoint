//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS
#include	"AllJointableObjectManager.h"
#include	"Allocator.h"

#include	"JointableObjID.h"

#include	"File.h"
#include	<stdio.h>		// 書式付き文字列操作
#include	"AllTextureManager.h"

//==========================================================
// 名前空間
using namespace NJointableObjectID;
using namespace NAllTextureManager;

//==========================================================
// ファイル名を定義(ファイル数が少ないのでそれぞれ定義)
// メモ)
// array[ステージ番号][文字列]　
// 　チュートリアルステージは 0 として考える

// データまでのパス
#ifdef _PLATFORM_DIRECTX
static const char	*STAGE_OBJ_DATA_DIRECTORY = "_Resource/Data/JointableObject/";
#endif
static const char	*STAGE_OBJ_DATA_EXTENSION = ".csv";

// ファイル名だけを記述
static const char gStageObjDataFileName[7][256] = {
	{ "JointableObjectData1_0" },	// チュートリアル
	{ "JointableObjectData1_1" },	// 1	
	{ "JointableObjectData1_2" },	// 2
	{ "JointableObjectData1_3" },	// 3
	{ "JointableObjectData2_1" },	// 3
	{ "JointableObjectData2_2" },	// 3
	{ "JointableObjectData2_3" },	// 3
};


//==========================================================
// メソッド定義

CAllJointableObjectManager::CAllJointableObjectManager(){
	CBaseJointableObject::SetTexRefBlock(GetPTexture(NTextureResourceInfo::JOINTABLE_OBJ_SINGLE));
	CJointableObjGoal::SetRefTexture(GetPTexture(NTextureResourceInfo::JOINTABLE_OBJ_GOAL));
}

CAllJointableObjectManager::~CAllJointableObjectManager(){
	Destroy();
}

void CAllJointableObjectManager::Init(){
	for (int i = 0; i < objNum; ++i){
		//allObjArray[i]->Init(allObjArray[i]->GetInitPosition());	// いろいろめんどくさい
		
		allObjArray[i]->SetIsConnected(false);
	}
}

void CAllJointableObjectManager::Load(int _stageNo){
	// 必要なリソースをロード
	LoadResource();

	// マップデータをロード
	LoadObjectData(_stageNo);
}

void CAllJointableObjectManager::Destroy(){
	for (int i = 0; i < objNum; ++i){
		DeleteSafely(allObjArray[i]);
	}

	DeleteArraySafely(allObjArray);
}

void CAllJointableObjectManager::Update(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Update();
	}
}

void CAllJointableObjectManager::Sync(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Sync();
	}
}

void CAllJointableObjectManager::Render(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Render();
	}
}

void CAllJointableObjectManager::AllocStageObject(int _id, CBaseJointableObject **_dest){
	switch (_id){
		case TEST:
			*_dest = new CJointableObjPendulam;
			break;

		case GOAL:
			*_dest = new CJointableObjGoal;
			break;



		default:
			// error
			break;
	}
}

void CAllJointableObjectManager::LoadResource(){
	//texBlock = GetPTexture(NTextureResourceInfo::JOINTABLE_OBJ_SINGLE);
	//texGoal = GetPTexture(NTextureResourceInfo::JOINTABLE_OBJ_GOAL);
}

void CAllJointableObjectManager::LoadObjectData(int _stageNo){
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
	allObjArray = new CBaseJointableObject*[objNum];

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
		//allObjArray[i]->SetInitPosition(pos_);
	}
}