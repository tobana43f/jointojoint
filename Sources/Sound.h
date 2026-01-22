//==========================================================
// 音声を扱うクラス
// メモ)
// 　DirectX wavファイル

#ifndef _INC_SOUND_H
#define _INC_SOUND_H

#include	"PlatformSelector.h"
#include	"SoundResourceInfo.h"

class CSound {
public:
	// 使用するサウンドを設定
	void Load(NSoundResourceInfo::ESoundLabel _label);

	void Play();
	void Stop();

	// 未実装
	void Pause();

private:
	//==========================================================
	// 変数

	// 使用するサウンドのラベル
	NSoundResourceInfo::ESoundLabel soundLabel;

	// 再生状態を格納する変数(Wii用)
	unsigned int	handle;

public:
	CSound();
	~CSound();
};

#endif