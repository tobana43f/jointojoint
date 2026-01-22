//==========================================================
// 使用するボタンを識別する定数を定義する
#ifndef _INC_CONTROLLER_DEFINITION_H
#define _INC_CONTROLLER_DEFINITION_H

namespace NControllerDefinition {
	//==========================================================
	// コントローラー番号を識別する定数
	enum EControllerNo {
		CONTROLLER_1,
		CONTROLLER_2,
		CONTROLLER_3,
		CONTROLLER_4,
		CONTROLLER_MAX,		 // 最大コントローラー接続台数
	};

	//==========================================================
	// コントローラーのボタンを指定する定数
	enum EButtonID{
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_1,
		BUTTON_2,
		BUTTON_A,
		BUTTON_B,
		BUTTON_PLUS,
		BUTTON_MINUS,
	};

	//==========================================================
	// 入力状態の取得方法
	enum EInputType {	
		TRIGGER,
		HOLD,
		RELEASE,
		TYPE_MAX,		// 最大値
	};

	//==========================================================
	// コントローラーとボタンを指定して、各プラットフォームで使用するボタンの定数を取得する。
	int GetButton(EControllerNo _controllerNo, EButtonID _buttonID);
}

#endif