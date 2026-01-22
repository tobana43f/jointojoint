//==========================================================
// 使用するサウンドリソースの情報を管理する
#ifndef _INC_SOUND_RESOURCE_INFO_H
#define _INC_SOUND_RESOURCE_INFO_H

//==========================================================
// サウンドのリソースの情報を保持するクラス
class CSoundResourceInfo {
public:
	char*	GetFilePath(){ return filePath; }
	bool	GetIsLoop(){ return isLoop; }

private:
	char	filePath[256];	// サウンドリソースのみファイルパスを保持する。
	bool	isLoop;			// サウンドのループフラグ

public:
	CSoundResourceInfo(char *_fileName, bool _isLoop);
	~CSoundResourceInfo(){};
};


//==========================================================
// サウンドリソースを指定する定数などを定義
namespace NSoundResourceInfo {
	//----------------------------------------------------------
	// サウンドリソースを指定する定数。
	enum ESoundLabel {
		BGM_TEST,
		SE_TEST,

		// BGM

		BGM_TITLE,
		BGM_STAGE_SELECT,

		BGM_STAGE_1_0,

		BGM_STAGE_1_1,
		BGM_STAGE_1_2,
		BGM_STAGE_1_3,

		BGM_STAGE_2_1,
		BGM_STAGE_2_2,
		BGM_STAGE_2_3,
		
		// SE

		SE_TITLE_ENTER,
		
		SE_CURSOR,
		SE_DECIDE,
		SE_CANCEL,

		SOUND_LABEL_MAX,	// 注意) ラベルではない。
	};

	CSoundResourceInfo GetSoundResourceInfo(ESoundLabel _label);	// 指定した サウンドラベルのサウンド情報を取得する関数。
}





#endif
