//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS
#include	"AllAreaObjectManager.h"
#include	"Allocator.h"

#include	"AreaObjectID.h"

#include	"File.h"
#include	<stdio.h>		// 書式付き文字列操作
#include	"AllTextureManager.h"

//==========================================================
// 名前空間
using namespace NAreaObjectID;
using namespace NAllTextureManager;

//==========================================================
// ファイル名を定義(ファイル数が少ないのでそれぞれ定義)
// メモ)
// array[ステージ番号][文字列]　
// 　チュートリアルステージは 0 として考える

// データまでのパス
#ifdef _PLATFORM_DIRECTX
static const char	*STAGE_OBJ_DATA_DIRECTORY = "_Resource/Data/AreaObject/";
#endif
static const char	*STAGE_OBJ_DATA_EXTENSION = ".csv";

// ファイル名だけを記述
static const char gStageObjDataFileName[7][256] = {
	{ "AreaObjectData1_0" },	// チュートリアル
	{ "AreaObjectData1_1" },
	{ "AreaObjectData1_2" },
	{ "AreaObjectData1_3" },
	{ "AreaObjectData2_1" },
	{ "AreaObjectData2_2" },
	{ "AreaObjectData2_3" },
};


//==========================================================
// メソッド定義

CAllAreaObjectManager::CAllAreaObjectManager(){
	//CBaseAreaObject::SetRefTexTutorialBack(texTutorialBack);
	//CAreaObjectTutorialMove::SetRefTex(texTutorialMove);
	//CAreaObjectTutorialJump::SetRefTex(texTutorialJump);
	//CAreaObjectTutorialJointChar::SetRefTex(texTutorialJointChar);
	//CAreaObjectTutorialJointObj::SetRefTex(texTutorialJointObj);
	//CAreaObjectTutorialPendulam::SetRefTex(texTutorialPendulam);
	//CAreaObjectTutorialSeparateChar::SetRefTex(texTutorialSeparateChar);
	//CAreaObjectTutorialFall::SetRefTex(texTutorialFall);

	CBaseAreaObject::SetRefTexTutorialBack(GetPTexture(NTextureResourceInfo::TUTORIAL_BACK));
	CAreaObjectTutorialMove::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_MOVE));
	CAreaObjectTutorialJump::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_JUMP));
	CAreaObjectTutorialJointChar::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_JOINT_CHAR));
	CAreaObjectTutorialJointObj::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_JOINT_OBJ));
	CAreaObjectTutorialPendulam::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_PENDULAM));
	CAreaObjectTutorialSeparateChar::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_SEPARATE_CHAR));
	CAreaObjectTutorialFall::SetRefTex(GetPTexture(NTextureResourceInfo::TUTORIAL_FALL));
}

CAllAreaObjectManager::~CAllAreaObjectManager(){
	Destroy();
}

void CAllAreaObjectManager::Init(){

}

void CAllAreaObjectManager::Load(int _stageNo){
	// 必要なリソースをロード
	LoadResource();

	// マップデータをロード
	LoadAreaObjectData(_stageNo);
}

void CAllAreaObjectManager::Destroy(){
	for (int i = 0; i < objNum; ++i){
		DeleteSafely(allObjArray[i]);
	}

	DeleteArraySafely(allObjArray);
}

void CAllAreaObjectManager::Update(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Update();
	}
}

void CAllAreaObjectManager::Sync(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Sync();
	}
}

void CAllAreaObjectManager::Render(){
	for (int i = 0; i < objNum; ++i){
		allObjArray[i]->Render();
	}
}

void CAllAreaObjectManager::AllocStageObject(int _id, CBaseAreaObject **_dest){
	switch (_id){
	case TEST:
		*_dest = new CAreaObjectTest;
		break;

	case TUTORIAL_MOVE:
		*_dest = new CAreaObjectTutorialMove;
		break;

	case TUTORIAL_JUMP:
		*_dest = new CAreaObjectTutorialJump;
		break;

	case TUTORIAL_JOINT_CHAR:
		*_dest = new CAreaObjectTutorialJointChar;
		break;

	case TUTORIAL_JOINT_OBJ:
		*_dest = new CAreaObjectTutorialJointObj;
		break;

	//case TUTORIAL_JOINT_LIFT:
	//	*_dest = new CAreaobjj;
	//	break;

	case TUTORIAL_PENDULAM:
		*_dest = new CAreaObjectTutorialPendulam;
		break;

	case TUTORIAL_SEPARATE_CHAR:
		*_dest = new CAreaObjectTutorialSeparateChar;
		break;

	case TUTORIAL_SEPARATE_OBJ:
		*_dest = new CAreaObjectTutorialSeparateObj;
		break;

	case TUTORILA_FALL:
		*_dest = new CAreaObjectTutorialFall;
		break;

	

	default:
		// error
		break;
	}
}

void CAllAreaObjectManager::LoadResource(){
	//texTutorialBack = GetPTexture(NTextureResourceInfo::TUTORIAL_BACK);
	//texTutorialMove = GetPTexture(NTextureResourceInfo::TUTORIAL_MOVE);
	//texTutorialJump = GetPTexture(NTextureResourceInfo::TUTORIAL_JUMP);
	//texTutorialJointChar = GetPTexture(NTextureResourceInfo::TUTORIAL_JOINT_CHAR);
	//texTutorialJointObj = GetPTexture(NTextureResourceInfo::TUTORIAL_JOINT_OBJ);
	//texTutorialPendulam = GetPTexture(NTextureResourceInfo::TUTORIAL_PENDULAM);
	//texTutorialSeparateChar = GetPTexture(NTextureResourceInfo::TUTORIAL_SEPARATE_CHAR);
	//texTutorialFall = GetPTexture(NTextureResourceInfo::TUTORIAL_FALL);

}

void CAllAreaObjectManager::LoadAreaObjectData(int _stageNo){
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
	allObjArray = new CBaseAreaObject*[objNum];

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