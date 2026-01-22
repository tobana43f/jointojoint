#define		_CRT_SECURE_NO_WARNINGS
#include	"PlatformSelector.h"
#include	"Animation.h"	// 定義
#include	"Allocator.h"	// 安全な解放
#include	"File.h"		// ファイル操作を行う
#include	<stdio.h>		// 書式付き文字列操作

using namespace NAnimationResourceInfo;
using namespace NAllocator;

#undef NULL	// 再定義防止
#define NULL 0

// アニメーションリソースまでのパス
// アニメーションの拡張子
#ifdef _PLATFORM_DIRECTX
const char	*ANIMATION_RESOURCE_DIRECTORY = "_Resource/Animation/";
const char	*ANIMATION_RESOURCE_EXTENSION = ".anm";
#endif

//==========================================================
// 読み込みに使用するクラスを定義(このCPPでしか使用しないので、ここで定義(不正使用防止))
// 1つのパターンを読み込むリスト
class PatternLoader{
public:
	int				anmNo;
	PatternLoader	*next;
};

// 1つのアニメーションを一時的に読み込むリスト
class AnimationLoader{
public:
	PatternLoader	*anmPat;
	AnimationLoader	*next;
};

//==========================================================
// メソッド定義
CAnimation::CAnimation(){
	ppAnmData = NULL;
	anmNum = 0;
}

CAnimation::~CAnimation(){
	Release();
}

// アニメーションを、csv形式のファイルから読み込む
// メモ)
// 　過去に作ったやつを流用したので、
// 　結構無駄な処理が多い
void CAnimation::Load(NAnimationResourceInfo::EAnimationLabel _label){
	PatternLoader	*patTop_;
	AnimationLoader *anmTop_;
	CFile			file_;
	char			filePath_[256];		// ファイルパスを作成
	char			lineHeader_[256];	// 行の先頭列のヘッダ情報の読込先

	//==========================================================
	// 読み込み前準備
	// ファイルパスを作成
	sprintf(filePath_, "%s%s%s", ANIMATION_RESOURCE_DIRECTORY, GetAnimationResourceInfo(_label).GetFileName(), ANIMATION_RESOURCE_EXTENSION);

	// アニメーションファイルのオープン
	if (!file_.Open(filePath_, CFile::READ)){
		// error
		return;	// TODO:エラー処理で中断させる
	}

	// リストの先頭にダミーデータを確保
	patTop_ = new PatternLoader;		// 読込先のリストの先頭にダミーデータを確保
	anmTop_ = new AnimationLoader;
	anmTop_->anmPat = NULL;				// エラー防止

	// データ数の読込
	// 先頭列は説明なので、無視
	file_.Read("%[^,]", lineHeader_);	// 先頭列の読み飛ばし
	file_.Read("%d", &anmNum);	// データ数の読込(読み込み処理を簡略化するため)
	file_.SkipLine();

	//==========================================================
	// 実際のパターンを全て展開する。
	PatternLoader	*patCur_ = patTop_;
	AnimationLoader *anmCur_ = anmTop_;

	for (int i = 0; i < this->anmNum; ++i){
		file_.Read("%[^,]", lineHeader_);		// 先頭列の読み飛ばし

		// パターンの終端子まで繰り返す
		do{
			patCur_->next = new PatternLoader;	// 次のメモリを確保
			patCur_ = patCur_->next;			// 確保したメモリに移動
			patCur_->next = NULL;				// 次ポインタにNULLを代入し、リストの終端とする
			file_.Read("%d", &patCur_->anmNo);// データの読込
		} while (patCur_->anmNo != ANM_END && patCur_->anmNo != ANM_LOOP && patCur_->anmNo != ANM_NULL);

		file_.SkipLine();							// 次の行へ

		anmCur_->next = new AnimationLoader;		// 次メモリを確保
		anmCur_ = anmCur_->next;					// 確保したメモリに移動
		anmCur_->next = NULL;						// NULLを代入しリストの最後とする

		anmCur_->anmPat = patTop_;					// 読み込んできたパターンを代入

		// 余分にメモリを確保しないため
		if (i == this->anmNum - 1)
			break;

		patTop_ = new PatternLoader;				// 別の先頭を確保
		patCur_ = patTop_;							// その先頭を現在の参照先に変更
	}

	//==========================================================
	// メンバ変数のメモリを確保する
	int patLen_;				// パターンの長さ(anmNoの数)
	int anmNum_;				// アニメーションの数(patternの数)
	int refAnmNo_, refAnmPat_;	// ppAnmDataの添え字(ppAnmData[refAnmNo_][refAnmPat_])

	//----------------------------------------------------------
	// 確保する配列数を知るために、アニメーション数をカウントする
	anmNum_ = 0;
	for (anmCur_ = anmTop_->next; anmCur_ != NULL; anmCur_ = anmCur_->next)
		anmNum_++;

	ppAnmData = new int*[anmNum_];	// ポインタ配列を確保(配列を確保するための先頭アドレス)

	//----------------------------------------------------------
	// パターンを格納する配列を確保
	refAnmNo_ = 0;
	// アニメーションリスト走査
	for (anmCur_ = anmTop_->next; anmCur_ != NULL; anmCur_ = anmCur_->next){
		patLen_ = 0;
		// パターンリスト走査
		for (patCur_ = anmCur_->anmPat->next; patCur_ != NULL; patCur_ = patCur_->next){
			if (patCur_->anmNo == ANM_NULL){	// アニメーションが無い
				patLen_ = 0;	// 0にしておく(判断するため)
				break;
			}
			patLen_++;
		}

		// パターンを代入する配列を確保
		if (patLen_ != 0)
			ppAnmData[refAnmNo_] = new int[patLen_];	// 配列を確保
		else
			ppAnmData[refAnmNo_] = NULL;				// 空にする
		refAnmNo_++;	// 次のアニメーションへ
	}

	//==========================================================
	// メンバ変数に代入し、使用できる状態にする。
	refAnmNo_ = 0;
	for (anmCur_ = anmTop_->next; anmCur_ != NULL; anmCur_ = anmCur_->next){
		refAnmPat_ = 0;
		for (patCur_ = anmCur_->anmPat->next; patCur_ != NULL; patCur_ = patCur_->next){
			if (patCur_->anmNo == ANM_NULL)
				continue;
			ppAnmData[refAnmNo_][refAnmPat_] = patCur_->anmNo;
			refAnmPat_++;
		}
		refAnmNo_++;
	}

	//==========================================================
	// 一時的の確保したクラスを開放する処理
	//----------------------------------------------------------
	// パターンの開放
	for (anmCur_ = anmTop_; anmCur_ != NULL; anmCur_ = anmCur_->next){
		patCur_ = anmCur_->anmPat;
		PatternLoader *pPrev_ = patCur_;
		while (patCur_ != NULL){
			pPrev_ = patCur_;
			patCur_ = patCur_->next;

			delete pPrev_;
			pPrev_ = NULL;
		}
	}

	//----------------------------------------------------------
	// 一時読込用リストの開放
	AnimationLoader *aPrev_;
	anmCur_ = anmTop_;
	while (anmCur_ != NULL){
		aPrev_ = anmCur_;
		anmCur_ = anmCur_->next;

		delete aPrev_;
		aPrev_ = NULL;
	}
}

// 読み込んだアニメーションデータ内の指定した場所のデータを返す。
// アニメーションがない場合は 0 を返す。
int CAnimation::GetAnmData(int _anmNo, int _anmCnt){
	if (ppAnmData != NULL)
		return ppAnmData[_anmNo][_anmCnt];

	return 0;	
}

// 解放処理を気にしなくていいように
// デストラクタで呼び出すようにしておく
void CAnimation::Release(){
	if (ppAnmData == NULL)
		return;

	for (int i = 0; i < anmNum; ++i){
		DeleteArraySafely(ppAnmData[i]);
	}

	DeleteArraySafely(ppAnmData);
}