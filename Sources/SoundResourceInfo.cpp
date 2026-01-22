#define		_CRT_SECURE_NO_WARNINGS
#include	"PlatformSelector.h"
#include	"SoundResourceInfo.h"
#include	<stdio.h>					// 文字列操作

//==========================================================
// サウンドリソースまでのパス
// サウンドの拡張子
#ifdef _PLATFORM_DIRECTX
const char	*SOUND_RESOURCE_DIRECTORY = "_Resource/Sound/";
const char	*SOUND_RESOURCE_EXTENSION = ".wav";

#endif

//==========================================================
// CSoundResourceInfoクラスのメソッド定義
CSoundResourceInfo::CSoundResourceInfo(char *_fileName, bool _isLoop){
	// ファイルパスを作成し、それを保持する
	sprintf(filePath, "%s%s%s", SOUND_RESOURCE_DIRECTORY, _fileName, SOUND_RESOURCE_EXTENSION);
	isLoop = _isLoop;
}

namespace NSoundResourceInfo {
	//==========================================================
	// サウンド情報を持つグローバル変数
	// メモ)
	// 　リソースを追加した際は、ここに追加し、ラベルのほうも追加すること。
	static CSoundResourceInfo gSoundResourceInfo[SOUND_LABEL_MAX] = {
		CSoundResourceInfo("_BGMTest", true),
		CSoundResourceInfo("_SETest", false),

		CSoundResourceInfo("BGMTitle", true),
		CSoundResourceInfo("BGMStageSelect", true),

		CSoundResourceInfo("BGMStage1_0", true),

		CSoundResourceInfo("BGMStage1_1", true),
		CSoundResourceInfo("BGMStage1_2", true),
		CSoundResourceInfo("BGMStage1_3", true),

		CSoundResourceInfo("BGMStage2_1", true),
		CSoundResourceInfo("BGMStage2_2", true),
		CSoundResourceInfo("BGMStage2_3", true),

		CSoundResourceInfo("SETitleEnter", false),
		CSoundResourceInfo("SECursor", false),
		CSoundResourceInfo("SEDecide", false),
		CSoundResourceInfo("SECancel", false),
	};


	//==========================================================
	// 指定したサウンドラベルのサウンド情報を取得する関数
	CSoundResourceInfo GetSoundResourceInfo(ESoundLabel _label){
		if (_label >= SOUND_LABEL_MAX || _label < 0){
			// error
			return CSoundResourceInfo("",false);	// 無意味な内容のものを返す。　TODO : エラー処理で停止してOK
		}

		return gSoundResourceInfo[_label];
	}
}