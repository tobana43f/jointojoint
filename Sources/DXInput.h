//=============================================================================
//
// 入力処理 [Input.h]
//
//=============================================================================
#ifndef _INC_DX_INPUT_H
#define _INC_DX_INPUT_H

#include	"PlatformSelector.h"
#ifdef 		_PLATFORM_DIRECTX

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <dinputd.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND gHWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

bool GetMouseLeftPress(void);
bool GetMouseLeftTrigger(void);
bool GetMouseRightPress(void);
bool GetMouseRightTrigger(void);
bool GetMouseCenterPress(void);
bool GetMouseCenterTrigger(void);
long GetMouseAxisX(void);
long GetMouseAxisY(void);
long GetMouseAxisZ(void);
long GetMouseX(void);
long GetMouseY(void);

bool GetJoystickUpHold();
bool GetJoystickDownHold();
bool GetJoystickRightHold();
bool GetJoystickLeftHold();

bool GetJoystickUpTrigger();
bool GetJoystickDownTrigger();
bool GetJoystickRightTrigger();
bool GetJoystickLeftTrigger();

bool GetJoystickButtonHold(int _no);
bool GetJoystickButtonTrigger(int _no);


#endif

#endif