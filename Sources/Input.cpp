#include	"PlatformSelector.h"
#include	"Input.h"	// 定義

#ifdef _PLATFORM_DIRECTX
#include	"DXInput.h"
#endif

using namespace NControllerDefinition;

CInput::CInput(){
	Init(CONTROLLER_1);	// 一応 １ で初期化する。
}

CInput::~CInput(){

}

// プラットフォーム独自の更新をする。(メインループで1度だけ呼び出す。)
// 注意)
// 　CInputクラス内のUpdate で呼び出してはいけない。
// 注意２)
// 　呼び出すときは CInput:: から参照すること。(オブジェクトから呼び出さない)
void CInput::UpdatePlatformInput(){
#ifdef _PLATFORM_DIRECTX
	UpdateInput();
#endif
}

// 指定したコントローラーのボタンを初期設定に戻す。
// メモ)
// 　プログラムの初めに1回は呼び出す。
void CInput::Init(EControllerNo _no){
	SetControllerNo(_no);
	SetConfigHorizontal(BUTTON_LEFT, BUTTON_RIGHT);
	SetConfigVertical(BUTTON_DOWN, BUTTON_UP);

	SetConfigJump(BUTTON_A);
	SetConfigSeperate(BUTTON_B);

	SetConfigDecide(BUTTON_A);
	SetConfigCancel(BUTTON_B);
}

// 入力状態の更新
void CInput::Update(){

}

//===========================================================================
// ボタン系
// 様々な入力状態を取得する。
bool CInput::GetInputState(EButtonID _buttonId, EInputType _type){
	bool state_ = false;
	switch (_type){
		case TRIGGER:
			state_ = GetTrigger(_buttonId);
			break;

		case HOLD:
			state_ = GetHold(_buttonId);
			break;

		case RELEASE:
			state_ = GetRelease(_buttonId);
			break;
	}
	return state_;
}

//-----------
// トリガー
bool CInput::GetTrigger(EButtonID _buttonId){
#ifdef _PLATFORM_DIRECTX
	return GetKeyboardTrigger(GetButton(controllerNo, _buttonId));
#endif
}

//-----------
// ホールド
bool CInput::GetHold(EButtonID _buttonId){
#ifdef _PLATFORM_DIRECTX
	return GetKeyboardPress(GetButton(controllerNo, _buttonId));
#endif
}

//-----------
// リリース
bool CInput::GetRelease(EButtonID _buttonId){
#ifdef _PLATFORM_DIRECTX
	return GetKeyboardRelease(GetButton(controllerNo, _buttonId));
#endif
}

//=====================================================================
// スティック系
//-----------
// トリガー
//-------
// うえ
bool CInput::GetTriggerStickUp(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardTrigger(GetButton(controllerNo, BUTTON_UP));
	state |= GetJoystickUpTrigger();

	return state;
#endif
}
//-------
// した
bool CInput::GetTriggerStickDown(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardTrigger(GetButton(controllerNo, BUTTON_DOWN));
	state |= GetJoystickDownTrigger();
	return state;
#endif
}
//-------
// みぎ
bool CInput::GetTriggerStickRight(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardTrigger(GetButton(controllerNo, BUTTON_RIGHT));
	state |= GetJoystickRightTrigger();
	return state;
#endif
}
//---------
// ひだり
bool CInput::GetTriggerStickLeft(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardTrigger(GetButton(controllerNo, BUTTON_LEFT));
	state |= GetJoystickLeftTrigger();
	return state;
#endif
}

//-----------
// ホールド
//-------
// うえ
bool CInput::GetHoldStickUp(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardPress(GetButton(controllerNo, BUTTON_UP));
	state |= GetJoystickUpHold();
	return state;
#endif
}
// した
bool CInput::GetHoldStickDown(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardPress(GetButton(controllerNo, BUTTON_DOWN));
	state |= GetJoystickDownHold();
	return state;
#endif
}
// みぎ
bool CInput::GetHoldStickRight(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardPress(GetButton(controllerNo, BUTTON_RIGHT));
	state |= GetJoystickRightHold();
	return state;
#endif
}
// ひだり
bool CInput::GetHoldStickLeft(){
#ifdef _PLATFORM_DIRECTX
	bool state = false;
	state |= GetKeyboardPress(GetButton(controllerNo, BUTTON_LEFT));
	state |= GetJoystickLeftHold();
	return state;
#endif
}