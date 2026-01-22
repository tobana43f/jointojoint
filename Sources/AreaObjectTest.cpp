//==========================================================
// インクルード
#include	"AreaObjectTest.h"
#include	"CollideDetecter.h"

//==========================================================
// 名前空間
using namespace NCollideDetecter;

//==========================================================
// 静的領域

//==========================================================
// メソッド定義
CAreaObjectTest::CAreaObjectTest(){

}

CAreaObjectTest::~CAreaObjectTest(){

}

void CAreaObjectTest::Init(SVector3D _initPos){
	CBaseAreaObject::Init(_initPos);

	// あたり判定を行う場所を設定
	colArea.SetRelativeSize(100, 100);
	colArea.SetRelativePositionCenter(SVector3D(0, -30, 0));

	// 背景に描画するポリゴンを設定
	polyBack.Init();
	polyBack.SetColor(255, 255, 255,0);
	polyBack.SetPositionCenter(_initPos.x, _initPos.y, _initPos.z);
	polyBack.SetSize(120, 80);
	polyBack.SetReferenceTexture(pRefTexTutorialBack);
	polyBack.SetAngleDegree(0, 0, 0);
}

void CAreaObjectTest::Update(){
	CBaseAreaObject::Update();

	// あたり判定判定
	CollideArea();

	// ポリゴン更新
	polyBack.Update();

	// 色を変更
	if (GetIsCollide()){
		polyBack.AddColor(0, 0, 0, 3);
		if (polyBack.GetColor(CSquarePolygonData::VERTEX_0).a > 192)
			polyBack.SetColor(255, 255, 255, 192);
	}
	else{
		polyBack.AddColor(0, 0, 0, -5);
	
	}
}

void CAreaObjectTest::Sync(){
	CBaseAreaObject::Sync();
}

void CAreaObjectTest::Render(){
	CBaseAreaObject::Render();

	polyBack.Render();
}
