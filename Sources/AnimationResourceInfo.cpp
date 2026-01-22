#define		_CRT_SECURE_NO_DEPRECATE
#include	"AnimationResourceInfo.h"	// 定義
#include	<stdio.h>					// 文字列操作

//==========================================================
// メソッド定義
CAnimationResourceInfo::CAnimationResourceInfo(char *_fileName){
	sprintf(fileName, "%s", _fileName);
}


namespace NAnimationResourceInfo {
	//==========================================================
	// アニメーション情報を持つグローバル変数
	CAnimationResourceInfo gAnimationResourceInfo[ANM_LABEL_MAX] = {
		CAnimationResourceInfo("Test")
	};

	//==========================================================
	// 指定したアニメーションラベルのアニメーション情報を取得する関数
	CAnimationResourceInfo GetAnimationResourceInfo(EAnimationLabel _label){
		if (_label >= ANM_LABEL_MAX || _label < 0){
			// error
			return CAnimationResourceInfo("");	// 無意味な内容のものを返す。 TODO:エラー処理で中断
		}
		return gAnimationResourceInfo[_label];
	}
}