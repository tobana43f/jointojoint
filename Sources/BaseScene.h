//==========================================================
// ゲーム内の画面遷移のベースにするクラス。
// 各画面遷移は、このクラスを必ず継承すること。
#ifndef _INC_BASE_SCENE_H
#define _INC_BASE_SCENE_H

//==========================================================
// インクルード
#include	"Input.h"				// シーンで使用する入力クラス
#include	"CameraPerspective.h"
#include	"CameraOrthographic.h"
#include	"Polygon2D.h"

//==========================================================
// マクロ定義
#undef  PURE_FUNC		// 再定義防止
#define PURE_FUNC =0	// 純粋仮想関数をぱっと見で区別をしやすくするため。

//==========================================================
// クラス定義
class CBaseScene {
public:
	//==========================================================
	// 関数

	// 初期化処理
	virtual void Init() PURE_FUNC;

	// 解放処理
	virtual void Destroy() PURE_FUNC;

	// シーン内の更新、描画の実行
	virtual CBaseScene* Run() PURE_FUNC;

	// 参照するコントローラーを設定する
	static void		SetRefController(CInput *_pInput[NControllerDefinition::CONTROLLER_MAX]);

	//------------------------------------------------------
	// フェードインアウト
	// 初期化
	void	InitFade();
	// 更新関数
	// フェードインアウトが完了した時点でtrueを返す
	bool	UpdateFadeIn();
	bool	UpdateFadeOut();
	// 描画
	void	RenderFade();
	// フェードインアウト・ここまで
	//------------------------------------------------------

protected:
	//==========================================================
	// 変数

	CCameraPerspective	*pCameraPers;	// ここに使用するカメラを確保しても、使用するカメラのポインタを参照させてもOK
	CCameraOrthographic	*pCameraOrtho;	// 基本的にはOrthographicのカメラは1つだけでいいので、ここに確保して使用する。

	// フェードイン・アウト用
	CPolygon2D fade;	// ポリゴン
	int fadeAlpha;		// アルファ値


	//==========================================================
	// 関数

	// リソースのロードなど
	// メモ)
	// 　コンストラクタで呼び出す。
	virtual void Load() PURE_FUNC;

	// 次のフレームで使用するシーンを返す
	// ゲームシーンを変更しない場合は、thisを返せば継続できる。(基本的な処理はこれ)
	// ゲームシーンを更新する際は、シーンクラスを確保し、それを返す。
	// 注意)
	// 　この際に、自分を必ず解放すること。(delete this)(メモリリーク)
	// Run()関数の最後に呼び出し、IsSceneEndの値を見て判断する。
	virtual CBaseScene* ChangeGameScene() PURE_FUNC;

	// シーンの終了状態をセットする
	void	SetIsSceneEnd(bool _state){ isSceneEnd = _state; }

	// シーンの終了状態を取得する
	bool	GetIsSceneEnd(){ return isSceneEnd; }

	// カメラを配置する関数。
	// 3D、2Dの描画を開始する前に1度だけ呼び出す。
	// メモ)何度呼び出しても問題ない
	void	BeginRender3D(){ pCameraPers->Deploy(); }
	void	BeginRender2D(){ pCameraOrtho->Deploy(); }

	// 参照している指定のコントローラーのポインタを取得。
	CInput*	GetPInput(NControllerDefinition::EControllerNo _no);

private:
	//==========================================================
	// 変数

	// 使用するコントローラーを参照するポインタ(ここには確保しない。)
	// GameMainのほうで確保したコントローラーを参照する。
	// 継承した全てのシーンから同じコントローラーにアクセスできるように、static で持つ。
	static CInput	*pInput[NControllerDefinition::CONTROLLER_MAX];

	// ゲームシーンの終了フラグ。(true:終了)
	bool			isSceneEnd;

public:
	CBaseScene();
	virtual ~CBaseScene();
};

#endif