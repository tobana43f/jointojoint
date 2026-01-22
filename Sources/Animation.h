//==========================================================
// テクスチャアニメーション情報を扱うクラス
// テクスチャクラスに内包する。
#ifndef _INC_ANIMATION_H
#define _INC_ANIMATION_H

#include	"AnimationResourceInfo.h"	// アニメーションラベルの参照

class CAnimation {
public:
	enum EAnimationTerminator {	// アニメーション配列の終端子
		ANM_LOOP = 255,
		ANM_END  = 254,
		ANM_NULL = 253,
	};

	void	Load(NAnimationResourceInfo::EAnimationLabel _label);

	// ゲッター
	int		GetAnmData(int _anmNo, int _anmCnt);
	int		GetAnmNum(){ return anmNum; }

private:
	int	**ppAnmData;			// アニメーションデータの配列の確保する先のポインタ変数
	int	anmNum;					// アニメーション数

	void Release();

public:
	CAnimation();
	~CAnimation();
};


#endif