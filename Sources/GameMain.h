//==========================================================
// ゲームのメイン処理の実行に関わるヘッダ
// 共通に処理するもののみ記述する。
// ゲーム内で使用するシーンクラスなどは、このクラスに内包する。

#ifndef _INC_GAME_MAIN_H
#define	_INC_GAME_MAIN_H

//==========================================================
// インクルード
#include	"Input.h"	
#include	"BaseScene.h"
#include	"Polygon2D.h"

//==========================================================
// クラス定義

class CGameMain {
public:
	//==========================================================
	// 関数

	// 基本的な関数
	void Init();
	void Destroy();
	void Run();		// すべての処理を実行する関数

private:
	//==========================================================
	// 変数

	CInput		*pInput[NControllerDefinition::CONTROLLER_MAX];	// 使用するコントローラーの参照先ポインタ
	CBaseScene	*pCurScene;										// 現在のシーンを確保するポインタ

	//==========================================================
	//　関数

	// 全シーンに共通する更新(入力情報の更新など)
	void Update();

	// 全シーンに共通する描画(使わないかも？)
	// 描画の際に上書きされないように、Run関数の最後に呼び出す。
	void Render();

	// コントローラー関係
	void AllocController();
	void FreeController();
	void InitController();
	void UpdateController();

public:
	CGameMain();
	~CGameMain();
};


#endif