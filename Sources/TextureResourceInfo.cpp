#define		_CRT_SECURE_NO_DEPRECATE
#include	"TextureResourceInfo.h"
#include	<stdio.h>	// 文字列操作

//==========================================================
// CTextureResourceInfoクラスのメソッドの定義
CTextureResourceInfo::CTextureResourceInfo(char* _fileName, unsigned char _divX, unsigned char _divY, unsigned short _sizeX, unsigned short _sizeY){
	sprintf(fileName, "%s", _fileName);
	divX = _divX;
	divY = _divY;
	sizeX = _sizeX;
	sizeY = _sizeY;
}

//==========================================================
// 定数定義

// 見やすくするためだけに定数にしただけ
// テクスチャ全体のドット数
// 最大 縦横 1024ドット。
// テクスチャサイズは２のべき乗であることが好ましいが、1024以下で、スクロール処理などをしないなら、他のサイズでも問題ない。
enum TextureSize {
	SIZE_1 = 1,
	SIZE_2 = 2,
	SIZE_4 = 4,
	SIZE_8 = 8,
	SIZE_16 = 16,
	SIZE_32 = 32,
	SIZE_64 = 64,
	SIZE_128 = 128,
	SIZE_256 = 256,
	SIZE_512 = 512,
	SIZE_1024 = 1024,
};

// 見やすいように定義しただけ
// 1枚のテクスチャ内の分割数。
// 最大縦横15分割。(アニメーションの終端子の関係上)
enum TextureDivisionNum{
	DIV_1 = 1,
	DIV_2,
	DIV_3,
	DIV_4,
	DIV_5,
	DIV_6,
	DIV_7,
	DIV_8,
	DIV_9,
	DIV_10,
	DIV_11,
	DIV_12,
	DIV_13,
	DIV_14,
	DIV_15,
	DIV_16,
};


namespace NTextureResourceInfo{
	//==========================================================
	// テクスチャ情報を持つグローバル変数
	static CTextureResourceInfo gTextureResourceInfo[TEX_LABEL_MAX] = {
		// デバッグ用
		CTextureResourceInfo("Test", DIV_4, DIV_2, SIZE_2, SIZE_2),
		CTextureResourceInfo("CircleCollider", DIV_1, DIV_1, SIZE_128, SIZE_128),

		// フォント
		CTextureResourceInfo("Font_DotAndShadow", DIV_16, DIV_8, SIZE_512, SIZE_256),

		// タイトル
		CTextureResourceInfo("TitleLogo", DIV_1, DIV_1, SIZE_512, SIZE_512),	// タイトルロゴ

		// 文字列
		CTextureResourceInfo("push", DIV_1, DIV_1, SIZE_512, SIZE_64),			// Push A Button
		CTextureResourceInfo("stage", DIV_1, DIV_1, SIZE_512, SIZE_64),			// ステージセレクト
		CTextureResourceInfo("story", DIV_1, DIV_1, SIZE_512, SIZE_64),			// ストーリーモード
		CTextureResourceInfo("restart", DIV_1, DIV_1, SIZE_512, SIZE_64),		// リスタート
		CTextureResourceInfo("resume", DIV_1, DIV_1, SIZE_512, SIZE_64),		// ゲーム再開
		CTextureResourceInfo("title", DIV_1, DIV_1, SIZE_512, SIZE_64),			// タイトルに戻る
		CTextureResourceInfo("pause", DIV_1, DIV_1, SIZE_512, SIZE_64),			// ポーズ
	
		CTextureResourceInfo("StringStageSelectWhite", DIV_1, DIV_1, SIZE_512, SIZE_64),			// ポーズ
		CTextureResourceInfo("StringRestartWhite", DIV_1, DIV_1, SIZE_512, SIZE_64),			// ポーズ

		CTextureResourceInfo("StringGameOver", DIV_1, DIV_1, SIZE_512, SIZE_64),			// ポーズ
		CTextureResourceInfo("StringStageClear", DIV_1, DIV_1, SIZE_1024, SIZE_128),			// ポーズ

		// 背景
		CTextureResourceInfo("Test", DIV_4, DIV_2, SIZE_2, SIZE_2),
		CTextureResourceInfo("back1-1", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back1-2", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back1-3", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back2-1", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back2-2", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back2-3", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back3-1", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back3-2", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back3-3", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back4-1", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back4-2", DIV_1, DIV_1, SIZE_1024, SIZE_512),
		CTextureResourceInfo("back4-3", DIV_1, DIV_1, SIZE_1024, SIZE_512),

		// キャラクタ
		CTextureResourceInfo("CharacterBodyActive", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("CharacterBodyNegative", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("CoreActive", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("CoreNegative", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("CharacterCenter", DIV_1, DIV_1, SIZE_128, SIZE_128),
		CTextureResourceInfo("CharacterCenter", DIV_1, DIV_1, SIZE_128, SIZE_128),
		CTextureResourceInfo("CoreLight", DIV_1, DIV_1, SIZE_256, SIZE_256),	// コアの光

		// ステージオブジェクト
		CTextureResourceInfo("StageObjct", DIV_1, DIV_1, SIZE_512, SIZE_128),

		// 繋げられるオブジェクト
		CTextureResourceInfo("JointableObjectSingle", DIV_1, DIV_1, SIZE_512, SIZE_512),
		CTextureResourceInfo("JointableObjectGoal", DIV_1, DIV_1, SIZE_512, SIZE_512),

		// チュートリアル用画像
		CTextureResourceInfo("TutorialBack", DIV_1, DIV_1, SIZE_256, SIZE_256),		// ホログラム背景
		CTextureResourceInfo("TutorialMove", DIV_2, DIV_1, SIZE_512, SIZE_256),		// 移動
		CTextureResourceInfo("TutoJump", DIV_2, DIV_1, SIZE_512, SIZE_256),
		CTextureResourceInfo("TutoJointChar", DIV_4, DIV_1, SIZE_512, SIZE_256),
		CTextureResourceInfo("TutoJointObj", DIV_2, DIV_1, SIZE_512, SIZE_256),
		//		CTextureResourceInfo("TutoJointLift", DIV_1, DIV_1, SIZE_512, SIZE_256),
		CTextureResourceInfo("TutoPendulam", DIV_4, DIV_1, SIZE_512, SIZE_256),
		CTextureResourceInfo("TutoSeparateChar", DIV_2, DIV_1, SIZE_512, SIZE_256),
		//		CTextureResourceInfo("TutoSeparateObj", DIV_2, DIV_1, SIZE_512, SIZE_256),
		CTextureResourceInfo("TutoFall", DIV_2, DIV_1, SIZE_512, SIZE_256),

		// ステージセレクトアイコン
		CTextureResourceInfo("StageIcon0-0", DIV_1, DIV_1, SIZE_128, SIZE_128),	// チュートリアル
		CTextureResourceInfo("StageIcon1-1", DIV_1, DIV_1, SIZE_128, SIZE_128),	// 1-1
		CTextureResourceInfo("StageIcon1-2", DIV_1, DIV_1, SIZE_128, SIZE_128),	// 1-2
		CTextureResourceInfo("StageIcon1-3", DIV_1, DIV_1, SIZE_128, SIZE_128),	// 1-3
		CTextureResourceInfo("StageIcon2-1", DIV_1, DIV_1, SIZE_128, SIZE_128),	// 1-1
		CTextureResourceInfo("StageIcon2-2", DIV_1, DIV_1, SIZE_128, SIZE_128),	// 1-2
		CTextureResourceInfo("StageIcon2-3", DIV_1, DIV_1, SIZE_128, SIZE_128),	// 1-3
		// セレクトアイコンのフレーム
		CTextureResourceInfo("StageFrameTutorial", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("StageFrame1-1", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("StageFrame1-2", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("StageFrame1-3", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("StageFrame2-1", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("StageFrame2-2", DIV_1, DIV_1, SIZE_256, SIZE_256),
		CTextureResourceInfo("StageFrame2-3", DIV_1, DIV_1, SIZE_256, SIZE_256),
	};

	//==========================================================
	// 指定したテクスチャラベルのテクスチャ情報を取得する関数
	CTextureResourceInfo GetTextureResourceInfo(ETextureLabel _label){
		if (_label >= TEX_LABEL_MAX || _label < 0){
			// error
			return CTextureResourceInfo("", 1, 1, 1, 1);	// 無意味な内容のものを返す。　TODO : エラー処理で停止してOK
		}

		return gTextureResourceInfo[_label];
	}
}

