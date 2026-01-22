//==========================================================
// インクルード
#include	"CollideDetecter.h"
#include	"MathTypeDefinition.h"
#include	"Math.h"

//==========================================================
// 名前空間
using namespace NMath;

//==========================================================
// メソッド定義

bool NCollideDetecter::CollideCircleAndCircle(CCircleCollider *_pObj1, CCircleCollider *_pObj2, float *_pOutCollideLen){
	// あたり判定が有効な状態かどうか判断する
	if (!_pObj1->GetIsActive() || !_pObj2->GetIsActive())
		return false;

	// 各軸の2点間の距離を求める
	SVector3D distVecCoord_ = SVector3D(
		_pObj2->GetWorldPositionCenter().x - _pObj1->GetWorldPositionCenter().x,
		_pObj2->GetWorldPositionCenter().y - _pObj1->GetWorldPositionCenter().y,
		0
		);

	// 各軸の距離の2乗を取得
	float distPowX_ = distVecCoord_.x * distVecCoord_.x;
	float distPowY_ = distVecCoord_.y * distVecCoord_.y;

	// 2点間の直線距離を求める
	float distVec_ = sqrt(distPowX_ + distPowY_);

	// 2つのオブジェクトの半径の合計を取得
	float radVecLen_ = _pObj1->GetRadius() + _pObj2->GetRadius();

	// 円のあたり判定を行う
	bool isCollide = false;
	if (distVec_ <= radVecLen_)
		isCollide =  true;

	// めり込み距離を求める
	// 結果を入れる先が設定されていない場合は終了
	if (_pOutCollideLen != NULL)
		*_pOutCollideLen = distVec_ - radVecLen_;

	return isCollide;
}

bool NCollideDetecter::CollideAABBAndAABB(CAABBCollider *_pObj1, CAABBCollider *_pObj2, SVector3D *_pOutPosCenter, SVector3D *_pOutSize){
	// あたり判定が友好の状態がどうか判断する
	if (!_pObj1->GetIsActive() || !_pObj2->GetIsActive())
		return false;

	// オブジェクトの左上座標を求める
	SVector3D	posLTObj1_ = _pObj1->GetWorldPositionCenter() - (_pObj1->GetSize() * 0.5f);
	SVector3D	posLTObj2_ = _pObj2->GetWorldPositionCenter() - (_pObj2->GetSize() * 0.5f);

	// オブジェクトの幅へのアクセスを楽にするため、変数に格納
	SVector3D	sizeObj1_ = _pObj1->GetSize();
	SVector3D	sizeObj2_ = _pObj2->GetSize();

	// 判定を行う
	bool isCollideX_ = false;
	bool isCollideY_ = false;
	bool isCollideZ_ = false;
	bool isCollide_ = false;

	// X
	if (posLTObj1_.x >= posLTObj2_.x - sizeObj1_.x &&
		posLTObj1_.x <= posLTObj2_.x + sizeObj2_.x)	
		isCollideX_ = true;

	// Y
	if (posLTObj1_.y >= posLTObj2_.y - sizeObj1_.y &&
		posLTObj1_.y <= posLTObj2_.y + sizeObj2_.y)
		isCollideY_ = true;

	// Z
	if (posLTObj1_.z >= posLTObj2_.z - sizeObj1_.z &&
		posLTObj1_.z <= posLTObj2_.z + sizeObj2_.z)
		isCollideZ_ = true;
	
	// 各軸のあたり判定状態から、実際に衝突しているか判断する
	if (isCollideX_ && isCollideY_ && isCollideZ_)
		isCollide_ = true;

	// 衝突していない場合は終了
	if (!isCollide_)
		return isCollide_;

	// 詳細な情報がいらない場合
	if (_pOutPosCenter == NULL && _pOutSize == NULL)
		return isCollide_;

	// 衝突を起こした座標などの情報を格納する
	// 衝突しているAABBが重なっている、左上座標を求める(座標の大きいほうの値を格納、3D座標系)
	SVector3D	colPosLT_(0, 0, 0);
	posLTObj1_.x > posLTObj2_.x ? colPosLT_.x = posLTObj1_.x : colPosLT_.x = posLTObj2_.x;
	posLTObj1_.y > posLTObj2_.y ? colPosLT_.y = posLTObj1_.y : colPosLT_.y = posLTObj2_.y;
	posLTObj1_.z > posLTObj2_.z ? colPosLT_.z = posLTObj1_.z : colPosLT_.z = posLTObj2_.z;
		
	// 衝突しているAABBが重なっている、右下座標求める(座標が小さいほうの値を格納)
	SVector3D	colPosRB_(0, 0, 0);
	posLTObj1_.x + sizeObj1_.x < posLTObj2_.x + sizeObj2_.x ? colPosRB_.x = posLTObj1_.x + sizeObj1_.x : colPosRB_.x = posLTObj2_.x + sizeObj2_.x;
	posLTObj1_.y + sizeObj1_.y < posLTObj2_.y + sizeObj2_.y ? colPosRB_.y = posLTObj1_.y + sizeObj1_.y : colPosRB_.y = posLTObj2_.y + sizeObj2_.y;
	posLTObj1_.z + sizeObj1_.z < posLTObj2_.z + sizeObj2_.z ? colPosRB_.z = posLTObj1_.z + sizeObj1_.z : colPosRB_.z = posLTObj2_.z + sizeObj2_.z;

	// 中心座標と、サイズに変換し、ポインタに格納
	if (_pOutSize != NULL)
		*_pOutSize = colPosRB_ - colPosLT_;
	
	if (_pOutPosCenter != NULL)
		*_pOutPosCenter = colPosLT_ + (colPosRB_ - colPosLT_) / 2.0f;

	return isCollide_;
}

bool NCollideDetecter::CollideTriangleAndPoint(SVector3D *_pPosA, SVector3D *_pPosB, SVector3D *_pPosC, SVector3D *_pPosP){
	// 判定する三角形の各頂点座標を求める
	SVector3D	vxPos_[3];
	vxPos_[0] = *_pPosA;
	vxPos_[1] = *_pPosB;
	vxPos_[2] = *_pPosC;

	// 三角形の各辺へのベクトルを取得
	SVector3D	edgeVec_[3];
	edgeVec_[0] = vxPos_[1] - vxPos_[0];	// 0→1のベクトル
	edgeVec_[1] = vxPos_[2] - vxPos_[1];	// 1→2のベクトル
	edgeVec_[2] = vxPos_[0] - vxPos_[2];	// 2→0のベクトル

	// vxPos[0]から見た辺の向きベクトルを得る
	SVector3D	ce1_ = vxPos_[1] - vxPos_[0];
	SVector3D	ce2_ = vxPos_[2] - vxPos_[0];
	int			ccw = 1;

	// 判定する法線方向を求める
	if (CalcCrossVector3D(&ce1_, &ce2_).z < 0.0f)
		ccw = -1;

	// 点が三角形の内側にあるかを判定
	for (int i = 0; i < 3; ++i){
		SVector3D	cp_ = *_pPosP - vxPos_[i];
		if (CalcCrossVector3D(&edgeVec_[i], &cp_).z * ccw <= 0)
			return false;
	}

	// 点が三角形の中に存在する
	return true;
}
