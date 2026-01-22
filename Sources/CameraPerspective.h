//==========================================================
// ・3D空間上に配置したポリゴンを射影変換するカメラを扱うクラス
#ifndef _INC_CAMERA_PERSPECTIVE_H
#define _INC_CAMERA_PERSPECTIVE_H

#include	"MathTypeDefinition.h"

class CCameraPerspective {
public:
	static SMatrix44*	GetCurrentViewMatrix(){ return gpCurrentViewMatrix; }	

	void Deploy();		// カメラを3D空間上に配置する。(3D関係の描画のを開始する直前に必ず実行する必要がある)

	// 値を加算する関数
	void	AddCameraPosition(float _x, float _y, float _z){ cameraPosition.x += _x; cameraPosition.y += _y; cameraPosition.z += _z; }
	void	AddFocusPosition(float _x, float _y, float _z){ focusPosition.x += _x; focusPosition.y += _y; focusPosition.z += _z; }

	// セッター
	void	SetCameraPosition(float _x, float _y, float _z){ cameraPosition.x = _x; cameraPosition.y = _y; cameraPosition.z = _z; }
	void	SetFocusPosition(float _x, float _y, float _z){ focusPosition.x = _x; focusPosition.y = _y; focusPosition.z = _z; }
	void	SetUpDirection(float _x, float _y, float _z){ upDirection.x = _x; upDirection.y = _y; upDirection.z = _z; }
	void	SetViewAngleDeg(float _deg){ viewAngleDeg = _deg; };	// インクルードが増えるのでCppで定義
	void	SetScreenAspectRatio(float _ratio){ screenAspectRatio = _ratio; }	// 横/縦
	void	SetClipDistance(float _near, float _far){ nearClip = _near; farClip = _far; }

	// ゲッター
	SVector3D	GetCameraPosition(){ return cameraPosition; }
	SVector3D*  GetCameraPPosition(){ return &cameraPosition; }
	SVector3D	GetFocusPosition(){ return focusPosition; }
	SVector3D	GetUpDirection(){ return upDirection; }
	float		GetViewAngleDeg(){ return viewAngleDeg; }
	float		GetScreenAspectRatio(){ return screenAspectRatio; }
	float		GetNearClip(){ return nearClip; }
	float		GetFarClip(){ return farClip; }

	SMatrix44	GetViewMatrix(){ return mxView; }
	SMatrix44	GetProjectionMatrix(){ return mxProjection; }

private:
	static SMatrix44*	gpCurrentViewMatrix;	// 使用中のカメラのビュー変換行列を格納

	// 設定に必要な値
	SVector3D	cameraPosition;		// カメラの座標
	SVector3D	focusPosition;		// カメラの焦点座標 (見ている位置)
	SVector3D	upDirection;		// カメラの上方向のベクトル
	float		viewAngleDeg;		// 視野角
	float		screenAspectRatio;	// スクリーンの縦横比
	float		nearClip;			// クリッピング距離(最短)		
	float		farClip;			// クリッピング距離(最長)

	// 作成した行列を保持しておく変数
	SMatrix44	mxView;				// ビュー変換行列を保持
	SMatrix44	mxProjection;		// プロジェクション変換行列を保持

	void	SetViewTransform();			// ビュー変換行列の作成
	void	SetProjectionTransform();	// プロジェクトしょん変換行列の作成
	void	SetViewPort();				// ビューポートを設定

public:
	CCameraPerspective();
	~CCameraPerspective();
};

#endif