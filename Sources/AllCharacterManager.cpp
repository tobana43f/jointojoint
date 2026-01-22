//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS
#include	"AllCharacterManager.h"
#include	"Allocator.h"
#include	"CollideDetecter.h"
#include	"AllStageObjectManager.h"
#include	"AllJointableObjectManager.h"
#include	"File.h"	// キャラクター情報のロード
#include	<stdio.h>		// 書式付き文字列操作
#include	"AllTextureManager.h"

//==========================================================
// 名前空間
using namespace NControllerDefinition;
using namespace NCollideDetecter;
using namespace NAllTextureManager;

//==========================================================
// ファイル名を定義(ファイル数が少ないのでそれぞれ定義)
// メモ)
// array[ステージ番号][文字列]　
// 　チュートリアルステージは 0 として考える

// データまでのパス
#ifdef _PLATFORM_DIRECTX
static const char	*CHARACTER_DATA_DIRECTORY = "_Resource/Data/Character/";
#endif
static const char	*CHARACTER_DATA_EXTENSION = ".csv";

// ファイル名だけを記述
static const char gCharacterDataFileName[7][256] = {
	{ "CharacterData1_0" },	// チュートリアル
	{ "CharacterData1_1" },
	{ "CharacterData1_2" },
	{ "CharacterData1_3" },
	{ "CharacterData2_1" },
	{ "CharacterData2_2" },
	{ "CharacterData2_3" },
};

//==========================================================
// 静的領域
CAllStageObjectManager	*CAllCharacterManager::pRefStageObj;
CAllJointableObjectManager *CAllCharacterManager::pRefJointableObj;
//==========================================================
// メソッド定義

CAllCharacterManager::CAllCharacterManager(){
	characterNum = 0;
	pCharacterArray = NULL;
	pRigidbody = NULL;
	pRefStageObj = NULL;

	// キャラクターにリソースをセット
	CCharacter::SetReferenceResource(
		GetPTexture(NTextureResourceInfo::CHARACTER_BODY_ACTIVE),
		GetPTexture(NTextureResourceInfo::CHARACTER_CORE_ACTIVE),
		GetPTexture(NTextureResourceInfo::CHARACTER_CORE_NEGATIVE),
		GetPTexture(NTextureResourceInfo::CHARACTER_CABLE)
		);
}

CAllCharacterManager::~CAllCharacterManager(){
	Destroy();
}

void CAllCharacterManager::Init(){
	for (int i = 0; i < GetCharacterNum(); ++i){
		SVector3D initPos_ = pCharacterArray[i].GetInitPosition();
	//	pCharacterArray[i].Init(); // rigidbodyのInitが呼ばれるのがまずい
		
		pCharacterArray[i].acceleartionCore = SVector3D(0, 0, 0);
		pCharacterArray[i].velocityCore = SVector3D(0, 0, 0);
		pCharacterArray[i].body.SetVelocity(0, 0, 0);
		pCharacterArray[i].body.SetAcceleration(SVector3D(0,0,0));
		pCharacterArray[i].body.SetAngularVelocityDegree(0);
		pCharacterArray[i].body.SetAngularAccelerationDegree(0);
		pCharacterArray[i].body.SetCircleAngleVelDeg(0);
		pCharacterArray[i].body.SetCircleAngleAccDeg(0);

		pCharacterArray[i].body.SetPositionCenter(initPos_.x, initPos_.y, initPos_.z);
		pCharacterArray[i].polyCore.SetPositionCenter(initPos_.x, initPos_.y, initPos_.z);

		pCharacterArray[i].SetIsJumpReady(false);
		pCharacterArray[i].SetIsConnected(false);
	}


	InitCharacterList();
}

void CAllCharacterManager::Load(int _stageNo){

	// リソースのロード
	LoadResource();

	// キャラクターのロード
	LoadCharacterPosition(_stageNo);
}

void CAllCharacterManager::Destroy(){
	DeleteArraySafely(pCharacterArray);
}

void CAllCharacterManager::Update(){
	activeCharacterManager.Update();
	negativeCharacterManager.Update();

	ProcCollide();

	ProcSeperateCharacter();
}

void CAllCharacterManager::Sync(){
	activeCharacterManager.Sync();
	negativeCharacterManager.Sync();

	SyncCharacterCollide();
}

void CAllCharacterManager::Render(){
	activeCharacterManager.Render();
	negativeCharacterManager.Render();
}

void CAllCharacterManager::ProcCollide(){
	CollideActiveCoreAndNegativeCoreSpace();
	CollideGround();
	CollideJointableObj();
}

int CAllCharacterManager::GetActiveCharacterNum(){
	CCharacter	*curActive_ = activeCharacterManager.GetPCharacterListHead();
	int num_ = 0;

	while (curActive_ != NULL){
		curActive_ = curActive_->next;
		num_++;
	}
	return num_;
}

void CAllCharacterManager::CollideActiveCoreAndNegativeCoreSpace(){
	// リストの先頭を取得する
	CCharacter	*curActive_ = NULL;
	CCharacter	*curNegative_ = NULL;
	bool isCollide = false;

	// 初期値を代入
	curActive_ = activeCharacterManager.GetPCharacterListTail();
	curNegative_ = negativeCharacterManager.GetPCharacterListHead();

	// アクティブの最後尾と、すべてのネガティブを総当たり
	// メモ)
	// 　途中で、コアがコアスペースにつながった場合は、それ以上の判定をとらない
	while (curNegative_ != NULL){

		// あたり判定
		isCollide = CollideCircleAndCircle(
			&curActive_->GetColliderCore(),
			&curNegative_->GetColliderCoreSpace()
			);

		// 衝突があれば終了する
		if (isCollide)
			break;

		// 次へ
		curNegative_ = curNegative_->next;
	}


	// あたり判定がない場合
	if (!isCollide)
		return;

	// コアを接続する
	// 接続
	negativeCharacterManager.SubTargetCharacterFromList(curNegative_);
	activeCharacterManager.AddCharacterToListTail(curNegative_);
	curActive_->SetIsConnected(true);
}

void CAllCharacterManager::LoadResource(){
	//texCharacterBodyActive = GetPTexture(NTextureResourceInfo::CHARACTER_BODY_ACTIVE);
	//texCharacterBodyNegative = GetPTexture(NTextureResourceInfo::TEST);
	//texCharacterCoreActive = GetPTexture(NTextureResourceInfo::CHARACTER_CORE_ACTIVE);
	//texCharacterCoreNegative = GetPTexture(NTextureResourceInfo::TEST);
	//texCharacterCable = GetPTexture(NTextureResourceInfo::CHARACTER_CABLE);
}

void CAllCharacterManager::LoadCharacterPosition(int _stageNo){
	// ファイルパスを作成
	char filePath_[256];
	sprintf(filePath_, "%s%s%s", CHARACTER_DATA_DIRECTORY, gCharacterDataFileName[_stageNo], CHARACTER_DATA_EXTENSION);

	// リソースデータへアクセス
	CFile	file_;
	if (file_.Open(filePath_, CFile::READ)){
		// error
	}

	// キャラクター数を読み込む
	char	header_[256];			// ヘッダ情報の読み込み先
	int		charNum_;				// キャラクター数の読み込み先

	file_.Read("%[^,]", header_);		// ヘッダを読み飛ばす
	file_.Read("%d", &charNum_);	// キャラクター数を読み込む
	file_.SkipLine();

	// キャラクター用のメモリを確保
	characterNum = charNum_;
	pCharacterArray = new CCharacter[characterNum];

	// キャラクターの初期位置をロード
	//file_.SkipLine();							// ヘッダ情報を読み飛ばし
	file_.Read("%[^,]", header_);		// ヘッダを読み飛ばす
	file_.Read("%[^,]", header_);		// ヘッダを読み飛ばす
	file_.SkipLine();

	SVector2D	pos_(0, 0);						// zは常に０になるので、2Dで定義。
	for (int i = 0; i < characterNum; ++i){
		pCharacterArray[i].Init();
		
		file_.Read("%f", &pos_.x);
		file_.Read("%f", &pos_.y);
		file_.SkipLine();

		pCharacterArray[i].SetInitPosition(SVector3D(pos_.x, pos_.y, 0));
		//pCharacterArray[i].body.SetPositionCenter(pos_.x, pos_.y, 0);
		//pCharacterArray[i].polyCore.SetPositionCenter(pos_.x, pos_.y, 0);
	}

	//// キャラクターの1体をアクティブ状態にして、マネージャーに渡す
	//activeCharacterManager.SetRefCharacterListTop(&pCharacterArray[0]);
	//

	//// アクティブにしたキャラクター以外のキャラクターはネガティブ状態でマネージャーに渡す
	//// 全キャラクターが1体しかいない場合は、ネガティブは存在しない。
	//for (int i = 1; i < characterNum; ++i){
	//	negativeCharacterManager.AddCharacterToListTail(&pCharacterArray[i]);
	//}
}

void CAllCharacterManager::InitCharacterList(){
	for (int i = 0; i < characterNum; ++i){
		pCharacterArray[i].next = NULL;
		pCharacterArray[i].prev = NULL;
	}

	activeCharacterManager.Init();
	negativeCharacterManager.Init();

	// キャラクターの1体をアクティブ状態にして、マネージャーに渡す
	//activeCharacterManager.SetRefCharacterListTop(&pCharacterArray[0]);
	activeCharacterManager.AddCharacterToListTail(&pCharacterArray[0]);

	// アクティブにしたキャラクター以外のキャラクターはネガティブ状態でマネージャーに渡す
	// 全キャラクターが1体しかいない場合は、ネガティブは存在しない。
	for (int i = 1; i < characterNum; ++i){
		negativeCharacterManager.AddCharacterToListTail(&pCharacterArray[i]);
	}
}

void CAllCharacterManager::CollideGround(){
	// 判定するキャラクター
	CCharacter		*char_ = GetPCurCharacter();
	CAABBCollider	colCharFoot_ = GetPCurCharacter()->GetColliderGround();

	//----------------------------------------------------------
	// ジャンプフラグを初期化
	char_->SetIsJumpReady(false);

	//----------------------------------------------------------
	// 先頭のキャラクターと他のキャラクターの判定
	{
		for (int i = 0; i < GetCharacterNum(); ++i){
			// 先頭のキャラクターの場合
			if (char_ == &GetPCharacterArray()[i])
				continue;

			// 衝突判定を行うAABB
			CAABBCollider	colOther_ = *GetPCharacterArray()[i].body.GetPCollideBroad();

			// 衝突判定
			if (CollideAABBAndAABB(&colCharFoot_, &colOther_)){
				char_->SetIsJumpReady(true);
				return;	// 衝突を判定し次第終了
			}
		}
	}

	//----------------------------------------------------------
	// 先頭のキャラクターとステージのあたり判定
	{
		int					stageObjNum_ = pRefStageObj->GetObjNum();
		CBaseStageObject	**stageObjArray_ = pRefStageObj->GetStageObjArray();

		// 全てのステージオブジェクト
		for (int i = 0; i < stageObjNum_; ++i){
			
			int				blockNum_		= stageObjArray_[i]->GetBlockNum();
			CBlockObject	*blockObjArray	= stageObjArray_[i]->GetBlockObjArray();

			// 全てのブロックオブジェクト
			for (int j = 0; j < blockNum_; ++j){

				// 衝突判定を行うAABB
				CAABBCollider	colOther_ = *blockObjArray[j].GetPCollideBroad();

				// 衝突判定
				if (CollideAABBAndAABB(&colCharFoot_, &colOther_)){
					char_->SetIsJumpReady(true);
					return;	// 衝突を判定し次第終了
				}
			}
		}
	}

	//----------------------------------------------------------
	// 先頭のキャラクター繋げられるオブジェクトとのあたり判定
	{
		int						objNum_ = pRefJointableObj->GetObjNum();
		CBaseJointableObject	**objArray_ = pRefJointableObj->GetObjArray();

		// 全てのステージオブジェクト
		for (int i = 0; i < objNum_; ++i){

			int				blockNum_ = objArray_[i]->GetBlockNum();
			CBlockObject	*blockObjArray = objArray_[i]->GetBlockObjArray();

			// 全てのブロックオブジェクト
			for (int j = 0; j < blockNum_; ++j){

				// 衝突判定を行うAABB
				CAABBCollider	colOther_ = *blockObjArray[j].GetPCollideBroad();

				// 衝突判定
				if (CollideAABBAndAABB(&colCharFoot_, &colOther_)){
					char_->SetIsJumpReady(true);
					return;	// 衝突を判定し次第終了
				}
			}
		}
	}
}

void CAllCharacterManager::CollideJointableObj(){
	// 判定するキャラクター
	CCharacter		*char_ = GetPCurCharacter();
	CCircleCollider	colCore_ = GetPCurCharacter()->GetColliderCore();

	//----------------------------------------------------------
	// すべての繋げられるオブジェクト
	{
		int objNum_ = pRefJointableObj->GetObjNum();
		CBaseJointableObject **objArray_ = pRefJointableObj->GetObjArray();

		for (int i = 0; i < objNum_; ++i){
			CCircleCollider	*colTag_ = objArray_[i]->GetPCollideCoreSpace();
			bool isCollide_ = false;

			isCollide_ = CollideCircleAndCircle(&colCore_, colTag_);

			if (isCollide_){
				char_->SetIsConnected(true);
				objArray_[i]->SetIsConnected(true);
				return;
			}		
		}

	}
}

void CAllCharacterManager::ProcSeperateCharacter(){
	// 先頭のコアがほかのオブジェクトにつながっている場合
	if (GetPCurCharacter()->GetIsConnected())
		return;

	// 解除
	if (pRefInput1->GetSeperate(TRIGGER)){
		// 外されたキャラクターのポインタを取得
		CCharacter *subChar_;
		subChar_ = activeCharacterManager.SubCharacterFromListTail();
		negativeCharacterManager.AddCharacterToListTail(subChar_);

		// 先頭のキャラクターのコアの判定を無効にする
		if (subChar_ != NULL){
			activeCharacterManager.GetPCharacterListTail()->ProcCoreSeperate();
			activeCharacterManager.GetPCharacterListTail()->SetIsConnected(false);
		}
	}
}

void CAllCharacterManager::SyncCharacterCollide(){
	for (int i = 0; i < characterNum; ++i){
		pCharacterArray[i].Sync();
	}
}