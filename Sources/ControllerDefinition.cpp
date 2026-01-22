//==========================================================
// コントローラーを扱う際の、ボタンなどの識別子を定義する。
#include	"PlatformSelector.h"
#include	"ControllerDefinition.h"	// 定義
#ifdef _PLATFORM_DIRECTX
#include	"DXInput.h"					// DirectX用の入力定数を定義するため
#endif

namespace NControllerDefinition {
	//==========================================================
	// コントローラーごとのボタンを定義
#ifdef _PLATFORM_DIRECTX
	// ゲーム中に使用できるボタンの定義(仮想のコントローラー)
	// メモ)
	// 　配列は EControllerNo に対応している。(1つ目はプレイヤー1のボタン)
	// 注意)
	// 　プレイヤー３以降の定義は、適当。
	static const int	BUTTON_UP_ARRAY[CONTROLLER_MAX] = { DIK_UP, DIK_I, DIK_0, DIK_0 };
	static const int	BUTTON_DOWN_ARRAY[CONTROLLER_MAX] = { DIK_DOWN, DIK_K, DIK_0, DIK_0 };
	static const int	BUTTON_LEFT_ARRAY[CONTROLLER_MAX] = { DIK_LEFT, DIK_J, DIK_0, DIK_0 };
	static const int	BUTTON_RIGHT_ARRAY[CONTROLLER_MAX] = { DIK_RIGHT, DIK_L, DIK_0, DIK_0 };
	static const int	BUTTON_1_ARRAY[CONTROLLER_MAX] = { DIK_A, DIK_SEMICOLON, DIK_0, DIK_0 };
	static const int	BUTTON_2_ARRAY[CONTROLLER_MAX] = { DIK_S, DIK_COLON, DIK_0, DIK_0 };
	static const int	BUTTON_A_ARRAY[CONTROLLER_MAX] = { DIK_Z, DIK_P, DIK_0, DIK_0 };
	static const int	BUTTON_B_ARRAY[CONTROLLER_MAX] = { DIK_X, DIK_AT, DIK_0, DIK_0 };
	static const int	BUTTON_PLUS_ARRAY[CONTROLLER_MAX] = { DIK_Q, DIK_O, DIK_0, DIK_0 };
	static const int	BUTTON_MINUS_ARRAY[CONTROLLER_MAX] = { DIK_W, DIK_U, DIK_0, DIK_0 };

#endif
	// ボタンの配列をまとめる
	static const int	*BUTTON_ARRAY[] = {
		BUTTON_UP_ARRAY,
		BUTTON_DOWN_ARRAY,
		BUTTON_LEFT_ARRAY,
		BUTTON_RIGHT_ARRAY,
		BUTTON_1_ARRAY,
		BUTTON_2_ARRAY,
		BUTTON_A_ARRAY,
		BUTTON_B_ARRAY,
		BUTTON_PLUS_ARRAY,
		BUTTON_MINUS_ARRAY,
	};
	
	// コントローラーのボタンを定義したい配列にアクセスする。
	int GetButton(EControllerNo _controllerNo, EButtonID _buttonID){
		if (_controllerNo == CONTROLLER_MAX){
			// error
			return 0;	// 適当な値を返す(大丈夫？…要検討)
		}
		return (BUTTON_ARRAY[_buttonID][_controllerNo]);
	}
}