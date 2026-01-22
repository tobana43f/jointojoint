//=============================================================================
// DirectX専用、音声処理関係
#ifndef _INC_DX_SOUND_H
#define _INC_DX_SOUND_H

#include	"SoundResourceInfo.h"
#include	<xaudio2.h>

//==========================================================
// プロトタイプ宣言

namespace NDXSound {
	HRESULT	InitSound(void);
	void	DestroySound(void);

	// 再生
	void	PlaySound(NSoundResourceInfo::ESoundLabel _label);

	// 停止
	void	StopSound(NSoundResourceInfo::ESoundLabel _label);

	// 一時停止
	void	PauseSound(NSoundResourceInfo::ESoundLabel _label);
}

#endif
