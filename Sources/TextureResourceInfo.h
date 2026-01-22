//==========================================================
// 使用するテクスチャの管理
#ifndef _TEXTURE_RESOURCE_INFO_H
#define _TEXTURE_RESOURCE_INFO_H

//==========================================================
// テクスチャリソースの情報を保持するクラス
// グローバル変数で宣言するために、コンストラクタで全ての値を格納する。
class CTextureResourceInfo{
public:
	char*			GetFileName(){ return fileName; }
	unsigned char	GetDivX(){ return divX; }
	unsigned char	GetDivY(){ return divY; }
	unsigned short	GetSizeX(){ return sizeX; }
	unsigned short	GetSizeY(){ return sizeY; }

private:
	char			fileName[256];	// 注意) パスではない(直下ではない場合、Textureディレクトリからのパス)、拡張子もつけない。
	unsigned char	divX;			// X 軸の分割数			
	unsigned char	divY;			// Y 軸の分割数			
	unsigned short	sizeX;			// X 方向のドット数		
	unsigned short	sizeY;			// Y 方向のドット数		

public:
	CTextureResourceInfo(char* _fileName, unsigned char _divX, unsigned char _divY, unsigned short _sizeX, unsigned short _sizeY);
	~CTextureResourceInfo(){};
};

//==========================================================
// グローバル変数などの宣言
namespace NTextureResourceInfo{
	// テクスチャの指定をする定数
	enum ETextureLabel{
		// デバッグ用
		TEST,			// テクスチャとアニメーションのテスト用テクスチャ
		CIRCLE_COLLIDER,

		// フォント
		FONT_DOT_AND_SHADOW,

		// タイトル
		TITLE_LOGO,		// タイトルロゴ

		// 文字列
		PUSH_A_BUTTON,	// Push A Button
		STAGE_SELECT,	// ステージセレクト
		STORY_MODE,		// ストーリーモード
		RESTART,		// リスタート
		RESUME,			// ゲーム再開
		TITLE,			// タイトルに戻る
		PAUSE,			// ポーズ

		STAGE_SELECT_WHITE,
		RESTART_WHITE,

		STR_GAME_OVER,
		STR_STAGE_CLEAR,

		// 背景
		BACK_TEST,	// テスト用
		BACK_1_1,	// 背景１
		BACK_1_2,
		BACK_1_3,
		BACK_2_1,	// 背景２
		BACK_2_2,
		BACK_2_3,
		BACK_3_1,	// 背景３
		BACK_3_2,
		BACK_3_3,
		BACK_4_1,	// 背景４
		BACK_4_2,
		BACK_4_3,

		// キャラクタ
		CHARACTER_BODY_ACTIVE,		// ボディ	つながってる
		CHARACTER_BODY_NEGATIVE,	//			つながってない
		CHARACTER_CORE_ACTIVE,		// コア		つながってる
		CHARACTER_CORE_NEGATIVE,	//			つながってない
		CHARACTER_CABLE,			// ケーブル
		CHARACTER_CENTER,			// ボディ中心の光
		CHARACTER_CORE_LIGHT,		// コアの光

		// ステージオブジェクト
		STAGE_OBJ_BLOCK,

		// 繋げられるオブジェクト
		JOINTABLE_OBJ_SINGLE,
		JOINTABLE_OBJ_GOAL,

		// チュートリアル用画像
		TUTORIAL_BACK,
		TUTORIAL_MOVE,
		TUTORIAL_JUMP,
		TUTORIAL_JOINT_CHAR,
		TUTORIAL_JOINT_OBJ,
		//		TUTORIAL_JOINT_LIFT,
		TUTORIAL_PENDULAM,
		TUTORIAL_SEPARATE_CHAR,
		//		TUTORIAL_SEPARATE_OBJ,
		TUTORIAL_FALL,

		// ステージセレクトアイコン
		STAGE_ICON_TUTO,
		STAGE_ICON_1_1,
		STAGE_ICON_1_2,
		STAGE_ICON_1_3,
		STAGE_ICON_2_1,
		STAGE_ICON_2_2,
		STAGE_ICON_2_3,

		// セレクトアイコンのフレーム
		SELECT_FRAME_TUTO,
		SELECT_FRAME_1_1,
		SELECT_FRAME_1_2,
		SELECT_FRAME_1_3,
		SELECT_FRAME_2_1,
		SELECT_FRAME_2_2,
		SELECT_FRAME_2_3,

		TEX_LABEL_MAX,	// 注意) ラベルではない。
	};

	CTextureResourceInfo GetTextureResourceInfo(ETextureLabel _label); // 指定した テクスチャラベル のテクスチャ情報を取得する関数
}


#endif