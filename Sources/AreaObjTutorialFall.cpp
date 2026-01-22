//==========================================================
// インクルード
#include	"AreaObjTutorialFall.h"
#include	"CollideDetecter.h"

//==========================================================
// 名前空間
using namespace NCollideDetecter;

//==========================================================
// 静的領域
CTexture	*CAreaObjectTutorialFall::pRefTexTutorialFall = NULL;

//==========================================================
// メソッド定義
CAreaObjectTutorialFall::CAreaObjectTutorialFall(){

}

CAreaObjectTutorialFall::~CAreaObjectTutorialFall(){

}

void CAreaObjectTutorialFall::Init(SVector3D _initPos){
	CBaseAreaObject::Init(_initPos);

	// あたり判定を行う場所を設定
	colArea.SetRelativeSize(100, 100);
	colArea.SetRelativePositionCenter(SVector3D(0, -30, 0));

	// 背景に描画するポリゴンを設定
	polyBack.Init();
	polyBack.SetColor(255, 255, 255, 0);
	polyBack.SetPositionCenter(_initPos.x, _initPos.y, _initPos.z);
	polyBack.SetSize(120, 80);
	polyBack.SetReferenceTexture(pRefTexTutorialBack);
	polyBack.SetAngleDegree(0, 0, 0);

	// イメージ
	polyImage.Init();
	polyImage.SetColor(255, 255, 255, 0);
	polyImage.SetPositionCenter(_initPos.x, _initPos.y, _initPos.z);
	polyImage.SetSize(80, 80);
	polyImage.SetReferenceTexture(pRefTexTutorialFall);
	polyImage.SetAngleDegree(0, 0, 0);

	anmNum = 2;
	anmCnt = 0;
}

void CAreaObjectTutorialFall::Update(){
	CBaseAreaObject::Update();

	// あたり判定判定
	CollideArea();

	// ポリゴン更新
	polyBack.Update();
	polyImage.Update();

	// テクスチャアニメーションで、表示するImageを変化させる
	{
		if (cnt > cntStart + GetFlipSpeed()){
			anmCnt++;
			anmCnt = anmCnt % anmNum;
			polyImage.SetAnmNo(anmCnt);
			cntStart = cnt;
		}
	}

	// 色を変更
	if (GetIsCollide()){
		int alphaInc = 3;
		int alphaMax = 192;
		polyBack.AddColor(0, 0, 0, alphaInc);
		polyImage.AddColor(0, 0, 0, alphaInc);
		if (polyBack.GetColor(CSquarePolygonData::VERTEX_0).a > alphaMax){
			polyBack.SetColor(255, 255, 255, alphaMax);
			polyImage.SetColor(255, 255, 255, alphaMax);
		}
	}
	else{
		int alphaDec = -5;

		polyBack.AddColor(0, 0, 0, alphaDec);
		polyImage.AddColor(0, 0, 0, alphaDec);
	}
}

void CAreaObjectTutorialFall::Sync(){
	CBaseAreaObject::Sync();
}

void CAreaObjectTutorialFall::Render(){
	CBaseAreaObject::Render();

	polyBack.Render();
	polyImage.Render();
}
