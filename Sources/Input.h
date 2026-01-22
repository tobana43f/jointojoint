//==========================================================
// ゲームに必要な入力を行うクラス
// コントローラーの数だけ宣言して使う
// メモ)
// 　ボタンの名前は、キーの名前ではなく、使用する目的の名前を付けると、後での変更に対応しやすい。
// メモ2)
// 　DirectX側のInput関数と名前がかぶりそうなので注意

#ifndef _INC_INPUT_H
#define	_INC_INPUT_H

//==========================================================
// インクルード
#include	"PlatformSelector.h"
#include	"ControllerDefinition.h"	// コントローラー関係の定数の型を参照

#ifdef _PLATFORM_DIRECTX
#include	"DXInput.h"
enum DX_JOYSTICK_BUTTON {
	DX_BUTTON_A,
	DX_BUTTON_B,
	DX_BUTTON_START,
	DX_BUTTON_SELECT,
};
#endif
//==========================================================
// クラス定義

class CInput {
public:
	//==========================================================
	// 関数

	// プラットフォーム独自の更新をする。(メインループで1度だけ呼び出す。)
	static void UpdatePlatformInput();

	// コントローラーのボタンに初期状態を設定する。
	void	Init(NControllerDefinition::EControllerNo _no);

	// 更新
	void	Update();

	// セッター
	void	SetControllerNo(NControllerDefinition::EControllerNo _no){ controllerNo = _no; }
	void	SetConfigHorizontal(NControllerDefinition::EButtonID _negative, NControllerDefinition::EButtonID _positive){ configHorizontalNegative = _negative, configHorizontalPositive = _positive; }
	void	SetConfigVertical(NControllerDefinition::EButtonID _negative, NControllerDefinition::EButtonID _positive){ configVerticalNegative = _negative, configVerticalPositive = _positive; }
	void	SetConfigJump(NControllerDefinition::EButtonID _buttonId){ configJump = _buttonId; }
	void	SetConfigSeperate(NControllerDefinition::EButtonID _buttonId){ configSeperate = _buttonId; }
	void	SetConfigDecide(NControllerDefinition::EButtonID _buttonId){ configDecide = _buttonId; };
	void	SetConfigCancel(NControllerDefinition::EButtonID _buttonId){ configCancel = _buttonId; };

	// ゲッター(入力状態の取得)
	bool	GetHorizontalPositive(NControllerDefinition::EInputType _type){ return GetInputState(configHorizontalPositive, _type); }
	bool	GetHorizontalNegative(NControllerDefinition::EInputType _type){ return GetInputState(configHorizontalNegative, _type); };
	bool	GetVerticalPositive(NControllerDefinition::EInputType _type){ return GetInputState(configVerticalPositive, _type); };
	bool	GetVerticalNegative(NControllerDefinition::EInputType _type){ return GetInputState(configVerticalNegative, _type); };

	bool	GetJump(NControllerDefinition::EInputType _type){
		bool state = false;
	#ifdef _PLATFORM_DIRECTX
		int no = DX_BUTTON_A;
		if (_type == NControllerDefinition::EInputType::TRIGGER)
			state |= GetJoystickButtonTrigger(no);
		else if(_type == NControllerDefinition::EInputType::HOLD)
			state |= GetJoystickButtonHold(no);
	#endif
		state |= GetInputState(configJump, _type);
		return state;
	}

	bool	GetSeperate(NControllerDefinition::EInputType _type){ 
		bool state = false;
	#ifdef _PLATFORM_DIRECTX
		int no = DX_BUTTON_B;
		if (_type == NControllerDefinition::EInputType::TRIGGER)
			state |= GetJoystickButtonTrigger(no);
		else if (_type == NControllerDefinition::EInputType::HOLD)
			state |= GetJoystickButtonHold(no);
	#endif
		state |= GetInputState(configSeperate, _type); 
		return state;
	}

	bool	GetDecide(NControllerDefinition::EInputType _type){
		bool state = false;
	#ifdef _PLATFORM_DIRECTX
		int no = DX_BUTTON_A;
		if (_type == NControllerDefinition::EInputType::TRIGGER)
			state |= GetJoystickButtonTrigger(no);
		else if (_type == NControllerDefinition::EInputType::HOLD)
			state |= GetJoystickButtonHold(no);
	#endif
		state |= GetInputState(configDecide, _type);
		return state;
	}
	
	bool	GetCancel(NControllerDefinition::EInputType _type){ 
		bool state = false;
	#ifdef _PLATFORM_DIRECTX
		int no = DX_BUTTON_B;
		if (_type == NControllerDefinition::EInputType::TRIGGER)
			state |= GetJoystickButtonTrigger(no);
		else if (_type == NControllerDefinition::EInputType::HOLD)
			state |= GetJoystickButtonHold(no);
	#endif
		state |= GetInputState(configCancel, _type); 
		return state;
	}

	// ここからスティック(キー入力でも対応させておく)
	// トリガーたち
	bool	GetTriggerStickUp();
	bool	GetTriggerStickDown();
	bool	GetTriggerStickRight();
	bool	GetTriggerStickLeft();
	// ホールドたち
	bool	GetHoldStickUp();
	bool	GetHoldStickDown();
	bool	GetHoldStickRight();
	bool	GetHoldStickLeft();

private:
	//==========================================================
	// 変数

	// コントローラーの番号
	NControllerDefinition::EControllerNo	controllerNo;

	// ボタンとして設定するキーの定数を保持する変数(ゲーム中にキーコンフィグができるように。)
	NControllerDefinition::EButtonID		configHorizontalPositive;	// 平行方向
	NControllerDefinition::EButtonID		configHorizontalNegative;	// ┗左右(Negative ← → Positive)
	NControllerDefinition::EButtonID		configVerticalPositive;		// 水平方向 
	NControllerDefinition::EButtonID		configVerticalNegative;		// ┗上下 (Negative ↓　↑ Positive)

	NControllerDefinition::EButtonID		configJump;					// ジャンプボタン
	NControllerDefinition::EButtonID		configSeperate;				// 分離ボタン(キャラクターを外す)

	NControllerDefinition::EButtonID		configDecide;				// 決定
	NControllerDefinition::EButtonID		configCancel;				// キャンセル

	//==========================================================
	// 関数

	bool	GetInputState(NControllerDefinition::EButtonID _buttonId, NControllerDefinition::EInputType _type);

	// 入力状態の取得方法別のゲッター
	bool	GetTrigger(NControllerDefinition::EButtonID _buttonId);
	bool	GetHold(NControllerDefinition::EButtonID _buttonId);
	bool	GetRelease(NControllerDefinition::EButtonID _buttonId);

public:
	CInput();
	~CInput();
};

#endif