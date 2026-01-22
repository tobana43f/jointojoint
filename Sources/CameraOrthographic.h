//==========================================================
// 2D用のポリゴンを正射影変換をするカメラを扱うクラス。
#ifndef _INC_CAMERA_ORTHOGRAPHIC_H
#define _INC_CAMERA_ORTHOGRAPHIC_H

#include	"MathTypeDefinition.h"

class CCameraOrthographic {
public:
	void Deploy();	// カメラを配置

private:

	void SetViewTransform();
	void SetProjectionTransform();

public:
	CCameraOrthographic();
	~CCameraOrthographic();
};


#endif