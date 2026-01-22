//==========================================================
// 意味を持った領域をもったオブジェクトクラス
//----------------------------------------------------------
// 例えば、ゲームオーバーになる場所を判定したり、
// キャラクターが近づくと、チュートリアルを表示したりするクラス。
// チェックポイントなどの基本的に見えないオブジェクトを管理する。
#ifndef _INC_BASE_AREA_OBJECT_H
#define _INC_BASE_AREA_OBJECT_H

//==========================================================
// インクルード
#include	"MathTypeDefinition.h"
#include	"AABBCollider.h"
#include	"AllCharacterManager.h"

//==========================================================
// マクロ定義
#undef PURE_FUNC
#define PURE_FUNC =0

//==========================================================
// クラス定義
class CBaseAreaObject {
public:
	//==========================================================
	// 定数
	SColor	GetDefaultColor(){ SColor col = { 255, 255, 255, 192 }; return col; }	

	//==========================================================
	// 関数
	virtual void Init(SVector3D _initPos);
	virtual void Destroy();
	virtual void Update();		// 継承後に演出の更新
	virtual void Sync();
	virtual void Render();		// 継承後に演出の描画などをおこなう

	// リソース
	static void SetRefTexTutorialBack(CTexture *_pRef){ pRefTexTutorialBack = _pRef; }

	// 参照する情報の設定
	static void SetRefCharacter(CAllCharacterManager *_pRefCharacter){ pRefCharacter = _pRefCharacter; }

	// 初期位置
	void SetInitPosition(SVector3D _posCenter){ initPositionCenter = _posCenter; }
	SVector3D GetInitPosition(){ return initPositionCenter; }

protected:
	//==========================================================
	// 変数

	// リソース
	static CTexture	*pRefTexTutorialBack;

	// エリアを判定するあたり判定
	CAABBCollider	colArea;	

	// 判定の基準となる位置をポリゴンを使用して表す
	CPolygon3D		polyArea;

	// 汎用
	int cnt;
	int cntStart;

	//==========================================================
	// 関数

	// 情報を取得
	CAllCharacterManager* GetPCharacter(){ return pRefCharacter; }

	// 判定状況をセット
	void SetIsCollide(bool _state){ isCollide = _state; }

	// 判定状況を取得
	bool GetIsCollide(){ return isCollide; }

	// あたり判定を行う(継承後に、このクラスが持っている情報を使って、判定したいものと判定を行う)
	virtual void CollideArea();

private:
	//==========================================================
	// 変数

	// 参照する情報はあらかじめすべて保持しておく。(共通なのでなので、staticでOK)
	static CAllCharacterManager *pRefCharacter;

	SVector3D	initPositionCenter;

	//==========================================================
	// 変数

	// 判定状況
	bool isCollide;

public:
	CBaseAreaObject();
	virtual ~CBaseAreaObject();
};

#endif