//==========================================================
// DirectXのフレームレートの制御を行うクラス
#ifndef _INC_TIMER_H
#define	_INC_TIMER_H

//==========================================================
// インクルード
#include	<Windows.h>										// LARGE_INTEGER型を使用するため
#include	<mmsystem.h>									// timeBeginPeriod()等を使用するため
#pragma		comment(lib, "winmm.lib")						// 〃

//==========================================================
// クラス
class CDXTimer{
public:
	const int	MILLISEC_PER_SEC;		// 秒間毎のミリ秒(そのままの意味)
	const float	FRAME_RATE;				// フレームレート
	const float	MIN_FRAME_RATE;			// 最小のフレームレート
	const float	MIN_FRAME_TIME;			// 1フレーム辺りの時間
	const float	MAX_FRAME_TIME;			// 1フレーム辺りの最大の時間

	bool	frameControl();
	float	getFPS();

private:
	LARGE_INTEGER	start;			// 計測開始時間
	LARGE_INTEGER	end;			// 計測終了時間(現在時間)
	LARGE_INTEGER	freq;			// CPUの周波数(Hz)
	float			frameTime;		// 1フレーム毎の経過時間
	float			fps;			// frame per second
	DWORD			sleepTime;		// CPUを開放する時間を格納する

public:
	CDXTimer();
	~CDXTimer();
};

#endif // !_TIMER_H

