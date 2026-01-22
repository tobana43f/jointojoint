//==========================================================
// インクルード
#include	"RigidbodyPair.h"
#include	"Math.h"
#include	"CollideDetecter.h"
#include	<Math.h>

//==========================================================
// 名前空間
using namespace NMath;
using namespace NCollideDetecter;

//==========================================================
// メソッド定義
CRigidbodyPair::CRigidbodyPair(){
	pRigid1 = NULL;
	pRigid2 = NULL;
	next = NULL;
	collisionPosition = SVector3D(0, 0, 0);
	collisionVector = SVector3D(0, 0, 0);
	isCollide = false;
}

CRigidbodyPair::~CRigidbodyPair(){
	pRigid1 = NULL;
	pRigid2 = NULL;
	next = NULL;
}

void CRigidbodyPair::SetPair(CRigidBody *_pRigid1, CRigidBody *_pRigid2){
	pRigid1 = _pRigid1;
	pRigid2 = _pRigid2;
}

void CRigidbodyPair::CollideDetect(){
	CollideRigidbodyAndRigidBody(pRigid1, pRigid2);
}

void CRigidbodyPair::CollideResponse(){
	if (!isCollide)
		return;

	// タイプを取得
	int type1_ = 0;
	int type2_ = 0;

	// 1のタイプを取得
	switch (pRigid1->GetType()){
		case CRigidBody::DYNAMIC:{type1_ = 0; break; }
		case CRigidBody::KINEMATIC:{type1_ = 1;	break; }
		case CRigidBody::STATIC:{type1_ = 2; break; }
	}

	// ２のタイプを取得
	switch (pRigid2->GetType()){
		case CRigidBody::DYNAMIC:{type2_ = 0; break; }
		case CRigidBody::KINEMATIC:{type2_ = 1;	break; }
		case CRigidBody::STATIC:{type2_ = 2; break; }
	}

	// 判定する
	int collideType_[3][3] = {
		{ 0,  1,  2 },	//   D K S
		{ 3, -1, -1 },	// D o o o
		{ 4, -1, -1 }	// K o x x
	};					// S o x x

	// 剛体の種類による応答をする
	switch (collideType_[type1_][type2_]){
		case 0:
			CollideResponseDynamicAndDynamic(pRigid1, pRigid2);
			break;
		case 1:
			collisionVector = collisionVector * -1;
			CollideResponseDynamicandKinematic(pRigid1, pRigid2);
			break;
		case 2:
			collisionVector = collisionVector * -1;
			CollideResponseDynamicAndStatic(pRigid1, pRigid2);
			break;
		case 3:
			CollideResponseDynamicandKinematic(pRigid2, pRigid1);
			break;
		case 4:
			CollideResponseDynamicAndStatic(pRigid2, pRigid1);
			break;			
	}

	return; 
}

//----------------------------------------------------------
// 衝突検出に使用する関数群

bool CRigidbodyPair::CollideRigidbodyAndRigidBody(CRigidBody *_pRigid1, CRigidBody *_pRigid2){
	//----------------------------------------------------------
	// _pRigid1をの中心を原点とするローカル座標に変換
	SVector3D	posRB1_ = _pRigid1->GetPositionCenter() - _pRigid1->GetPositionCenter();
	SVector3D	posRB2_ = _pRigid2->GetPositionCenter() - _pRigid1->GetPositionCenter();

	//----------------------------------------------------------
	// 各頂点へのベクトルを求める
	SVector3D	vecRB1_[4];
	SVector3D	vecRB2_[4];

	for (int i = 0; i < 4; ++i){
		vecRB1_[i] = _pRigid1->CalcVertexPosition(i) - _pRigid1->GetPositionCenter();
		vecRB2_[i] = _pRigid2->CalcVertexPosition(i) - _pRigid2->GetPositionCenter();
	}

	// ミンコフスキー差の頂点の座標を取得
	SVector3D	vxPosMincof_[16];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			vxPosMincof_[j + i * 4] = posRB2_ + vecRB2_[i] - vecRB1_[j];
		}
	}

	//----------------------------------------------------------
	// 再帰的に判定する単体(三角形)を求めていく
	// 衝突していない場合は、最接近点が求まると終了
	SVector3D	original(0, 0, 0);				// 原点(posRB1_)
	SVector3D	vecDir_ = posRB2_ - original;	// ある方向ベクトル

	bool		isCollide_ = false;				// 衝突フラグ
	SVector3D	posShortest_[3];				// 衝突を判定する三角形を作る頂点を格納する
	int			cntShortest_ = 0;				// 選択された頂点の数
	SVector3D	posExcluded_[16];				// 一度選択された頂点を格納していき、除外する。(最大でもミンコフスキーの差の頂点の数にしかならない)
	int			cntExcluded_ = 0;				// 除外された頂点の数

	// 配列初期化
	for (int i = 0; i < 3; ++i)
		posShortest_[i] = SVector3D(_pRigid2->GetPositionCenter());

	for (int i = 0; i < 16; ++i)
		posExcluded_[i] = SVector3D(_pRigid2->GetPositionCenter());

	//----------------------------------------------------------
	// ミンコフスキの差の頂点のうち、ベクトル方向に一番遠い点を選ぶ
	while (true){
		//----------------------------------------------------------
		// 原点に向かう線と、ベクトルの始点から最も遠い点を求める
		float vecFar_ = -1;										// 最も遠いい点までの距離を格納する
		for (int i = 0; i < 16; ++i){
			SVector3D vecDirSuv_ = CalcVector3DNormal(&vecDir_) * -99999;			// 線を伸ばして、無限に続く線とする。
			SVector3D crossPos_(0, 0, 0);
			CalcShortestDistXY(&vecDir_, &vecDirSuv_, &vxPosMincof_[i], &crossPos_);

			// 最長距離を更新
			SVector3D	crossVec_ = vecDir_ - crossPos_;
			float len_ = CalcVector3DLength(&crossVec_);		// ベクトル方向に最も遠い点と距離を求める。
			if (vecFar_ < len_){
				vecFar_ = len_;									// 最も遠いい点を更新
				posShortest_[cntShortest_] = vxPosMincof_[i];	// 遠い場合は、ミンコフスキーの差上の点を格納
			}
		}

		//----------------------------------------------------------
		// 再帰処理準備
		if (cntShortest_ == 0){
			// 求まった点から原点に向かうベクトルを使う
			vecDir_ = posShortest_[cntShortest_] - original;

			// 選んだ点を除外
			posExcluded_[cntExcluded_] = posShortest_[0];

			// 次の点へ
			cntShortest_++;
			cntExcluded_++;
		}
		else if (cntShortest_ == 1){
			// 2点目の頂点が求まらなかった場合は衝突していない
			if (posShortest_[1] == SVector3D(_pRigid2->GetPositionCenter()))
				break;

			// 除外したデータを再度選んだ場合
			//if (posShortest_[1] == posExcluded_[0])
			//	break;

			// 求まった2つの点を結んだ線分と、原点の最短距離を求め、その点から原点に向かうベクトルを使う
			SVector3D	pos3(0, 0, 0);
			CalcShortestDistXY(&posShortest_[0], &posShortest_[1], &original, &pos3);
			vecDir_ = pos3 - original;

			// 今選んだ点を除外
			posExcluded_[cntExcluded_] = posShortest_[1];

			cntShortest_++;
			cntExcluded_++;
		}
		else if (cntShortest_ == 2){
			// すでに選択されている頂点を選んだ場合
			bool isExcluded_ = false;	// 多重ループから抜け出すため
			for (int i = 0; i < 16; ++i){
				if (posShortest_[2] == posExcluded_[i]){
					isExcluded_ = true;
					break;
				}
			}
			if (isExcluded_)
				break;

			// 現在選ばれた点を除外する
			posExcluded_[cntExcluded_] = posShortest_[2];
			cntExcluded_++;

			// ３点をつなぐ三角形内に原点があるかどうかを調べる。
			isCollide_ = CollideTriangleAndPoint(&posShortest_[0], &posShortest_[1], &posShortest_[2], &original);
			// 三角形内にある場合は衝突している
			if (isCollide_)
				break;

			//----------------------------------------------------------
			// 現在の三角形に衝突していない場合

			// 原点から一番遠い頂点を探す。
			float	farestDist_ = CalcVector3DLength(&posShortest_[0]);
			int		indexTarget_ = 0;
			for (int i = 1; i < 3; ++i){
				float dist_ = CalcVector3DLength(&posShortest_[i]);

				if (dist_ > farestDist_){
					farestDist_ = dist_;
					indexTarget_ = i;
				}
			}

			// １番遠い点を外し、配列を整列する
			// メモ)[2]が不要な頂点
			switch (indexTarget_){
				case 0:
					posShortest_[0] = posShortest_[1];
				case 1:
					posShortest_[1] = posShortest_[2];

			}
			posShortest_[2] = SVector3D(0, 0, 0);	// 不要なので初期化

			// 最接近点を探し、
			// 新たな基準ベクトルを作成。
			SVector3D	pos3(0, 0, 0);
			CalcShortestDistXY(&posShortest_[0], &posShortest_[1], &original, &pos3);
			vecDir_ = pos3 - original;
		}
		else{
			break;
		}
	}

	// 衝突情報を初期化
	collisionVector = SVector3D(0, 0, 0);
	collisionPosition = SVector3D(0, 0, 0);
	isCollide = false;

	// 衝突なし
	if (!isCollide_)
		return isCollide_;

	isCollide = true;

	//----------------------------------------------------------
	// 衝突があった場合、貫通深度、衝突法線を求める
	CalcCollideVector(posShortest_, _pRigid1, _pRigid2, &collisionVector);

	//----------------------------------------------------------
	// 衝突座標を求める
	CalcCollidePosition(&collisionVector, _pRigid1, _pRigid2, &collisionPosition);

	return isCollide_;
}

void CRigidbodyPair::CalcCollideVector(SVector3D _triangle[3], CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColVec){
	//==========================================================
	// _pRigid1をの中心を原点とするローカル座標に変換
	SVector3D	posRB1_ = _pRigid1->GetPositionCenter() - _pRigid1->GetPositionCenter();
	SVector3D	posRB2_ = _pRigid2->GetPositionCenter() - _pRigid1->GetPositionCenter();

	//----------------------------------------------------------
	// 各頂点へのベクトルを求める
	SVector3D	vecRB1_[4];
	SVector3D	vecRB2_[4];
	for (int i = 0; i < 4; ++i){
		vecRB1_[i] = _pRigid1->CalcVertexPosition(i) - _pRigid1->GetPositionCenter();
		vecRB2_[i] = _pRigid2->CalcVertexPosition(i) - _pRigid2->GetPositionCenter();
	}

	// ミンコフスキー差の頂点の座標を取得
	SVector3D	vxPosMincof_[16];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			vxPosMincof_[j + i * 4] = posRB2_ + vecRB2_[i] - vecRB1_[j];
		}
	}

	//==========================================================
	// 引数の衝突中三角形を頂点に格納
	SVector3D	original(0, 0, 0);	// 原点の位置
	SVector3D	colVec_(0, 0, 0);	// 貫通深度を格納する

	int			MAX_VX_AREA = 100;	// 多角形を構成する頂点を格納する配列関係
	SVector3D	posVxArea_[100];
	int			cntVxArea_ = 0;		// 多角形を構成している頂点の数

	// 引数から受け取った頂点を格納
	for (int i = 0; i < 3; ++i){
		posVxArea_[i] = _triangle[i];
	}
	cntVxArea_ = 3;	// 3固定

	//==========================================================
	// ↓ここからは、多角形を構成していく
	// 　原点から、めり込んでいる最短距離を探す
	//==========================================================

	//==========================================================
	// 原点から、中心座標に向かうベクトルを取得
	SVector3D	baseDir_(0, 0, 0);	// 基準となるベクトルを設定する
	int			idxEdgeStart = 0;	// 頂点を挿入する辺の終点側の添え字

	while (true){
		//==========================================================
		// 原点から多角形を構成している辺の最短距離を求め、原点からその頂点へのベクトルを取得する。
		{
			float lenShortest_ = 999999999999999999.9f;	// 適当にでかい値で初期化
			for (int i = 0; i < cntVxArea_; ++i){
				SVector3D	s_ = posVxArea_[i];						// 辺の始点位置を取得
				SVector3D	e_ = posVxArea_[(i + 1) % cntVxArea_];	// 辺の終点位置を取得
				SVector3D	cp_;									// 辺と原点の最短距離の交差点を取得
				float		len_;									// 辺と原点の最短距離を取得

				len_ = CalcShortestDistXY(&s_, &e_, &original, &cp_);

				if (len_ < lenShortest_){
					lenShortest_ = len_;							// 最短距離として設定
					baseDir_ = cp_ - original;						// 原点から最短距離の点へ向かうベクトルを基準とする。

					// 正しい?
					idxEdgeStart = (i + 1) % cntVxArea_;				// 最短距離となった辺の始点側の添え字を取得
				}
			}
		}

		//////////////////////////////////////////////////////////////
		//// ベクトルを描画
		//{
		//	SVector3D	s_ = original;
		//	SVector3D	e_ = baseDir_ * 999999;
		//	dir.SetPositionStart(s_.x, s_.y, s_.z);
		//	dir.SetPositionEnd(e_.x, e_.y, e_.z);
		//	//return;
		//}
		//////////////////////////////////////////////////////////////

		//==========================================================
		// 基準のベクトル方向で一番[遠いい]ミンコフスキーの差の頂点を取得する。
		SVector3D	newPos_(0, 0, 0);									// 追加する頂点
		{
			float lenFarest = -1;										// 初期値は無効な値を入れておく
			for (int i = 0; i < 16; ++i){
				SVector3D	baseDirInf_ = baseDir_ * 999999999999.9f;	// 線を伸ばして無限の長さと仮定する。
				SVector3D	vxMincof_;									// 頂点の座標(原点からのベクトルに等しい)

				// ミンコフスキーの差の点と、ベクトルとの交差点を求める
				CalcShortestDistXY(&original, &baseDirInf_, &vxPosMincof_[i], &vxMincof_);
				// 交差点への距離をもとめる
				float len_ = CalcVector3DLength(&vxMincof_);

				// 最遠点判断
				if (len_ > lenFarest){
					lenFarest = len_;			// 最遠距離を更新
					newPos_ = vxPosMincof_[i];	// 追加する頂点として保存
				}
			}
		}

		//==========================================================
		// 取得したミンコフスキーの差の頂点が、すでに多角形を構成している場合(再度同じ点が選ばれた場合) 
		{
			bool isFindShortest = false;
			for (int i = 0; i < cntVxArea_; ++i){
				if (newPos_ == posVxArea_[i]){
					isFindShortest = true;	// 最短距離が求まったので、終了
				}
			}

			if (isFindShortest)
				break;
		}

		//==========================================================
		// 取得した最遠のミンコフスキーの差の頂点を、多角形に追加する
		{
			// 頂点を挿入する場所まで、配列を後ろにずらし、挿入する場所を開ける
			for (int i = MAX_VX_AREA - 1; i > idxEdgeStart; --i){
				posVxArea_[i] = posVxArea_[i - 1];
			}
			posVxArea_[idxEdgeStart] = newPos_;	// 新しい頂点を挿入	
			cntVxArea_++;						// 頂点数を加算
		}

	}

	//==========================================================
	// 最短距離(貫通深度が求まった場合)
	*_pOutColVec = baseDir_;

}

void CRigidbodyPair::CalcCollidePosition(SVector3D *_pColVec, CRigidBody *_pRigid1, CRigidBody *_pRigid2, SVector3D *_pOutColPos){
	//----------------------------------------------------------
	// 剛体の位置を求める
	SVector3D	posRB1_ = _pRigid1->GetPositionCenter();
	SVector3D	posRB2_ = _pRigid2->GetPositionCenter();

	//----------------------------------------------------------
	// 剛体の中心座標から、各頂点へのベクトルを求める
	SVector3D	vxRB1_[4];
	SVector3D	vxRB2_[4];

	for (int i = 0; i < 4; ++i){
		vxRB1_[i] = _pRigid1->CalcVertexPosition(i) - _pRigid1->GetPositionCenter();
		vxRB2_[i] = _pRigid2->CalcVertexPosition(i) - _pRigid2->GetPositionCenter();
	}

	//----------------------------------------------------------
	// 衝突した剛体を、衝突法線方向に多めに移動
	SVector3D	moveDist_ = *_pColVec * -1.2f;			// めり込み分を、反対方向に少し多めに(少し多めにするのは、確実に衝突をしてない状態にするため)
	SVector3D	movedPosRB2_ = posRB2_ + (moveDist_);	// 移動後のRB2の中心座標
	SVector3D	movedPosRB1_ = posRB1_ - (moveDist_);

	//----------------------------------------------------------
	// 各頂点と、各辺の最短距離を求める
	SVector3D	colPos_(0, 0, 0);
	float		shortestDist_ = CalcShortestDistXY(&SVector3D(posRB1_ + vxRB1_[0]), &SVector3D(posRB1_ + vxRB1_[1]), &SVector3D(movedPosRB2_ + vxRB2_[0]), &colPos_);

	// _rb1 の辺と、rb2の頂点
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			SVector3D p_(movedPosRB2_ + vxRB2_[i]);			// 点
			SVector3D s_(posRB1_ + vxRB1_[j]);				// 辺の始点
			SVector3D e_(posRB1_ + vxRB1_[(j + 1) % 4]);	// 辺の終点
			SVector3D crossPos_;							// 交差点

			float len_ = CalcShortestDistXY(&s_, &e_, &p_, &crossPos_);

			// 最短距離を更新
			if (len_ < shortestDist_){
				shortestDist_ = len_;
				colPos_ = crossPos_;
			}
		}
	}

	// _rb2 の辺と、rb1の頂点
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			SVector3D p_(movedPosRB1_ + vxRB1_[i]);					// 点
			SVector3D s_(posRB2_ + vxRB2_[j]);				// 辺の始点
			SVector3D e_(posRB2_ + vxRB2_[(j + 1) % 4]);	// 辺の終点
			SVector3D crossPos_;								// 交差点

			float len_ = CalcShortestDistXY(&s_, &e_, &p_, &crossPos_);

			// 最短距離を更新
			if (len_ < shortestDist_){
				shortestDist_ = len_;
				colPos_ = crossPos_;
			}
		}
	}

	if (_pOutColPos != NULL)
		*_pOutColPos = colPos_;
}

//----------------------------------------------------------
// 衝突応答用の関数

void CRigidbodyPair::CollideResponseDynamicAndDynamic(CRigidBody *_dynamic1, CRigidBody *_dynamic2){
	////----------------------------------------------------------
	SVector3D test_ = collisionVector * -1 / 2.0f;

	{
		SVector3D	colVec_ = test_ * -1;
		SVector3D	pushBack = _dynamic1->distPushBack;

		// 符号が同じ場合
		if (pushBack.x < 0 && colVec_.x < 0 || pushBack.x >= 0 && colVec_.x >= 0){
			if (fabs(pushBack.x) < fabs(colVec_.x))
				pushBack.x = colVec_.x;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.x = pushBack.x + colVec_.x;	
		}

		// 符号が同じ場合
		if (pushBack.y < 0 && colVec_.y < 0 || pushBack.y >= 0 && colVec_.y >= 0){
			if (fabs(pushBack.y) < fabs(colVec_.y))
				pushBack.y = colVec_.y;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.y = pushBack.y + colVec_.y;
		}

		//----------------------------------------------------------
		// 摩擦がセットされていない場合
		if (_dynamic1->pushFriction >= 1.0f){
			float friction_ = (_dynamic2->GetFriction() + _dynamic1->GetFriction()) / 2.0f;
			_dynamic1->pushFriction = friction_;
		}
		else {
			float curFriction_ = _dynamic1->pushFriction;
			float friction_ = (_dynamic2->GetFriction() + _dynamic1->GetFriction()) / 2.0f;

			// 摩擦がより強い場合
			if (curFriction_ > friction_)
				_dynamic1->pushFriction = friction_;
		}
		_dynamic1->distPushBack = pushBack;
		_dynamic1->newVel = pushBack;
	}

	{
		SVector3D	colVec_ = test_ * 1;
		SVector3D	pushBack = _dynamic2->distPushBack;

		// 符号が同じ場合
		if (pushBack.x < 0 && colVec_.x < 0 || pushBack.x >= 0 && colVec_.x >= 0){
			if (fabs(pushBack.x) < fabs(colVec_.x))
				pushBack.x = colVec_.x;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.x = pushBack.x + colVec_.x;
		}

		// 符号が同じ場合
		if (pushBack.y < 0 && colVec_.y < 0 || pushBack.y >= 0 && colVec_.y >= 0){
			if (fabs(pushBack.y) < fabs(colVec_.y))
				pushBack.y = colVec_.y;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.y = pushBack.y + colVec_.y;
		}

		//----------------------------------------------------------
		// 摩擦がセットされていない場合
		if (_dynamic2->pushFriction >= 1.0f){
			float friction_ = (_dynamic2->GetFriction() + _dynamic1->GetFriction()) / 2.0f;
			_dynamic2->pushFriction = friction_;
		}
		else {
			float curFriction_ = _dynamic1->pushFriction;
			float friction_ = (_dynamic2->GetFriction() + _dynamic1->GetFriction()) / 2.0f;

			// 摩擦がより強い場合
			if (curFriction_ > friction_)
				_dynamic2->pushFriction = friction_;
		}
		_dynamic2->distPushBack = pushBack;
		_dynamic2->newVel = pushBack;
	}
	return;
}

void CRigidbodyPair::CollideResponseDynamicandKinematic(CRigidBody *_dynamic, CRigidBody *_kinematic){
	SVector3D test_ = collisionVector;

	//_dynamic->AddPositionCenter(-test_.x, -test_.y, -test_.z);

	{
		SVector3D	colVec_ = test_ * -1;
		SVector3D	pushBack = _dynamic->distPushBack;

		// 符号が同じ場合
		if (pushBack.x < 0 && colVec_.x < 0 || pushBack.x >= 0 && colVec_.x >= 0){
			if (fabs(pushBack.x) < fabs(colVec_.x))
				pushBack.x = colVec_.x;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.x = pushBack.x + colVec_.x;
		}

		// 符号が同じ場合
		if (pushBack.y < 0 && colVec_.y < 0 || pushBack.y >= 0 && colVec_.y >= 0){
			if (fabs(pushBack.y) < fabs(colVec_.y))
				pushBack.y = colVec_.y;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.y = pushBack.y + colVec_.y;
		}

		//----------------------------------------------------------
		// 摩擦がセットされていない場合
		if (_dynamic->pushFriction >= 1.0f){
			float friction_ = (_dynamic->GetFriction() + _kinematic->GetFriction()) / 2.0f;
			_dynamic->pushFriction = friction_;
		}
		else {
			float curFriction_ = _dynamic->pushFriction;
			float friction_ = (_dynamic->GetFriction() + _kinematic->GetFriction()) / 2.0f;

			// 摩擦がより強い場合
			if (curFriction_ > friction_)
				_dynamic->pushFriction = friction_;
		}

		_dynamic->distPushBack = pushBack;
		_dynamic->newVel = pushBack;
	}
}

void CRigidbodyPair::CollideResponseDynamicAndStatic(CRigidBody *_dynamic, CRigidBody *_static){
	SVector3D test_ = collisionVector;

	{
		SVector3D	colVec_ = test_ * -1;
		SVector3D	pushBack = _dynamic->distPushBack;

		// 符号が同じ場合
		if (pushBack.x < 0 && colVec_.x < 0 || pushBack.x >= 0 && colVec_.x >= 0){
			if (fabs(pushBack.x) < fabs(colVec_.x))
				pushBack.x = colVec_.x;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.x = pushBack.x + colVec_.x;
		}

		// 符号が同じ場合
		if (pushBack.y < 0 && colVec_.y < 0 || pushBack.y >= 0 && colVec_.y >= 0){
			if (fabs(pushBack.y) < fabs(colVec_.y))
				pushBack.y = colVec_.y;	// 大きいほうを保持
		}
		else{	// 符号が違う場合
			pushBack.y = pushBack.y + colVec_.y;
			
		}

		//----------------------------------------------------------
		// 摩擦がセットされていない場合
		if (_dynamic->pushFriction >= 1.0f){
			float friction_ = (_dynamic->GetFriction() + _static->GetFriction()) / 2.0f;
			_dynamic->pushFriction = friction_;
		}
		else {
			float curFriction_ = _dynamic->pushFriction;
			float friction_ = (_dynamic->GetFriction() + _static->GetFriction()) / 2.0f;

			// 摩擦がより強い場合
			if (curFriction_ > friction_)
				_dynamic->pushFriction = friction_;
		}

		_dynamic->distPushBack = pushBack;
		_dynamic->newVel = pushBack;
	}
}