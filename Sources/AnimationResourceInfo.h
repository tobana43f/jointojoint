//==========================================================
// アニメーションリソースの管理
#ifndef _INC_ANIMATION_RESOURCE_INFO_H
#define _INC_ANIMATION_RESOURCE_INFO_H

//==========================================================
// アニメーションリソースの情報を保持するクラス
// グローバル変数として宣言できるように、コンストラクタで全ての値を格納する。
class CAnimationResourceInfo {
public:
	char*	GetFileName(){ return fileName; }

private:
	char	fileName[256];

public:
	CAnimationResourceInfo(char *_fileName);
	~CAnimationResourceInfo(){};
};

//==========================================================
// 定数と、グローバル変数の宣言
namespace NAnimationResourceInfo {
	// アニメーションを指定する定数
	enum EAnimationLabel {
		TEST,			// テスト用

		ANM_LABEL_MAX,	// 注意) ラベルではない
	};

	CAnimationResourceInfo GetAnimationResourceInfo(EAnimationLabel _label);
}

#endif