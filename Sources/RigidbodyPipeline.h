//==========================================================
// 存在する全剛体のポインタを保持しておく。剛体シミュレーションパイプラインとして機能する
// もしも、剛体を削除(delete)することがある場合は、このクラス内で削除し、ポインタを繋げること。
#ifndef _INC_RIGIDBODY_PIPELINE_H
#define _INC_RIGIDBODY_PIPELINE_H

//==========================================================
// インクルード
#include	"MathTypeDefinition.h"

//==========================================================
// クラス定義の名前を参照
class CRigidBody;
class CRigidbodyPairManager;

//==========================================================
// クラス定義
class CRigidbodyPipeline {
public:
	//==========================================================
	// 関数

	void Update();

	// 新たに登録する剛体のポインタを渡す。
	void AddRigidbodyPointer(CRigidBody *_pRigidbody);

	// 重力を設定する
	void SetGravity(SVector3D _gravity){ gravity = _gravity; }

private:
	//==========================================================
	// 変数

	// 全ての剛体をリストとして管理する
	CRigidBody	*rigidbodyListTop;
	int			rigidbodyNum;

	// 剛体の衝突判定を行うクラス変数
	// 衝突の可能性がある剛体のペアを設定
	// ナローフェーズの当たり判定はまかせる。
	CRigidbodyPairManager	*pairManager;

	// 次フレームの状態を仮にシミュレーションする
	// 実体で持つ。
	// メモ)
	// 　rigidbodyクラス内のnextの値もコピーしたいので、配列で管理する。
	CRigidBody				*pProxyArray;


	// タイムステップ
	float		deltaTime;
	
	// 重力
	SVector3D	gravity;

	//==========================================================
	// 関数	
	
	// ダミーデータを除くデータの先頭を取得する
	CRigidBody* GetPRigidbodyListTop();

	// 剛体リストをすべて解放する。
	void FreeAllRigidbodyList();

	//----------------------------------------------------------

	// リストにある要素を数える
	void CountRigidbody();

	// 剛体の更新
	void UpdateRigidbody();

	// 剛体に共通に影響する処理
	void ProcGravity();	// 重力を加える

	// 加速度を速度に適用する
	void ApllyVelocity();

	// 摩擦などの減速k
	void ProcFriction();

	// 次フレームの状態を作成する
	void AllocProxy();

	// 同期処理
	void SyncProxy();

	// 作成したプロキシを使ってブロードフェーズの当たり判定をおこなう
	void CollideBroad();

	// プロキシに押し戻しを適用
	void ProcPushBack();

	// コピーしたプロキシを実際の剛体に適用
	void ApllyProxy();

	// プロキシを初期化
	void FreeProxy();

public:
	CRigidbodyPipeline();
	~CRigidbodyPipeline();
};

#endif