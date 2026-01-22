//==========================================================
// インクルード
#include	"AABBCollider.h"
#include	"Math.h"

//==========================================================
// メソッド定義

CAABBCollider::CAABBCollider(){
	relativeSize = SVector3D(0, 0, 0);
	SetType(CONSTANT);
	size = SVector3D(0, 0, 0);
}

CAABBCollider::~CAABBCollider(){

}

void CAABBCollider::Update(){
	switch (type){
		case CONSTANT:
			UpdateConstant();
			break;

		case WRAPPER:
			UpdateWrapper();
			break;
	}
}

void CAABBCollider::UpdateConstant(){
	SVector3D newSize_(0, 0, 0);
	SVector2D ownerSize(0, 0);

	if (GetPOwner() != NULL)
		ownerSize = GetPOwner()->GetSize();

	newSize_.x = ownerSize.x + relativeSize.x;
	newSize_.y = ownerSize.y + relativeSize.y;
	newSize_.z = 0;

	SetSize(newSize_);
	polyCollider.SetSize(newSize_.x, newSize_.y);
}

void CAABBCollider::UpdateWrapper(){
	SVector3D vx_[CSquarePolygonData::VERTEX_MAX];
	SVector2D minSize_(0, 0);
	SVector2D maxSize_(0, 0);

	// オーナーの頂点へのベクトルを取得(原点でのサイズ)
	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		vx_[i] = GetPOwner()->CalcVertexPosition(i) - GetPOwner()->GetPositionCenter();
	}

	// サイズを更新
	for (int i = 0; i < CSquarePolygonData::VERTEX_MAX; ++i){
		// xの最大を更新
		if (vx_[i].x > maxSize_.x)
			maxSize_.x = vx_[i].x;
		// xの最小を更新
		else if (vx_[i].x < minSize_.x)
			minSize_.x = vx_[i].x;

		// yの最大を更新
		if (vx_[i].y > maxSize_.y)
			maxSize_.y = vx_[i].y;
		else if (vx_[i].y < minSize_.y)
			minSize_.y = vx_[i].y;
	}

	// 新しい判定用サイズを作成
	SVector3D newSize_(0, 0, 0);
	newSize_.x = fabs(minSize_.x) + fabs(maxSize_.x);
	newSize_.y = fabs(minSize_.y) + fabs(maxSize_.y);
	newSize_.z = 0;

	// 相対サイズを適用
	newSize_.x += relativeSize.x;
	newSize_.y += relativeSize.y;
	newSize_.z += 0;

	// 適用
	SetSize(newSize_);
	polyCollider.SetSize(newSize_.x, newSize_.y);
}