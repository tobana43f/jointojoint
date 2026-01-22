//==========================================================
// DirectXのメインcpp
// DirectXでのみ必要な処理は、WinMain関数内に直接書くこと。

//==========================================================
// マクロ定義
#define	_MAIN_MODULE	// このcppをメインモジュールとして定義

//==========================================================
// インクルード
#include	"PlatformSelector.h"
#ifdef		_PLATFORM_DIRECTX

#include	<Windows.h>
#include	<d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include	"GameMain.h"	// メイン処理クラス
#include	"DXTimer.h"	
#include	"DXInput.h"		// 入力用
#include	"DXSound.h"		// 音声関係
#include	"Allocator.h"	
#include	"System.h"

//==========================================================
// 名前空間の指定
using namespace NSystem;
using namespace NDXSound;

//==========================================================
// 静的領域
static CGameMain	*pCGameMain;

//==========================================================
// プロトタイプ宣言
int		WINAPI	WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow);
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
bool	CreateMainWindow(HWND &_hWnd, HINSTANCE _hInstance, int _nCmdShow);	
HRESULT	InitD3D(HWND _hWnd);
void	RunD3D(void);
void	DestroyD3D(void);

//==========================================================
// エントリポイント
int WINAPI	WinMain(HINSTANCE _hInstance,HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow) {
	CDXTimer	*pCDXTimer;
	MSG			msg;

#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// メモリリーク検出用

	//_CrtSetBreakAlloc(209);										// 検出したメモリリークの場所を検知したいときに使う。(出力ウィンドウの[]中の数字がそれ)
#endif

	//----------------------------------------------------------
	// DirectXの初期化
	if (!CreateMainWindow(gHWnd, _hInstance, _nCmdShow))	// ウィンドウの作成
		return 1; 
	InitInput(_hInstance, gHWnd);	// DirectInput初期化
	InitSound();					// XAudio2の初期化
	pCGameMain = new CGameMain;
	pCDXTimer = new CDXTimer;
	pCGameMain->Init();

	//----------------------------------------------------------
	// メインループ
	while (true){
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	{
			if (GetMessage(&msg, NULL, 0, 0) == 0)		// "WM_QUIT"メッセージを取得した場合"0"を返す
				break;
			
			TranslateMessage(&msg);							// 仮想キーメッセージを文字メッセージへ変換
			DispatchMessage(&msg);							// ウィンドウプロシージャへメッセージを送出
		}
		else {
			if (pCDXTimer->frameControl())
				RunD3D();
		}
	}

	//----------------------------------------------------------
	// 解放処理
	pCGameMain->Destroy();
	delete pCDXTimer;
	delete pCGameMain;
	DestroySound();			// XAudio2の解放
	UninitInput();			// DirectInput解放
	DestroyD3D();			// DirectX終了処理関数
	return (int)msg.wParam;
}


//==========================================================
// ウィンドウプロシージャ
LRESULT CALLBACK	WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message){
		//----------------------------------------------------------
		// 必須メッセージ
		case WM_CREATE:
			break;
		case WM_CLOSE:
			DefWindowProc(_hWnd, _message, _wParam, _lParam);	//デフォルト処理を実行する。
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

			//----------------------------------------------------------
			// デフォルト処理
		default:
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

//==========================================================
// ウィンドウを作成する関数
bool CreateMainWindow(HWND &_hWnd, HINSTANCE _hInstance, int _nCmdShow)
{
	WNDCLASSEX	wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = _hInstance;
	wc.lpszClassName = WINDOW_NAME;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc)){
		MessageBox(NULL, "ウィンドウクラス構造体の初期化エラー", "", MB_OK);
		return false;
	}

	_hWnd = CreateWindow(
		wc.lpszClassName,
		WINDOW_TITLE,
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(int)WINDOW_WIDTH,
		(int)WINDOW_HEIGHT,
		NULL,
		NULL,
		_hInstance,
		NULL);

	// ウィンドウサイズを再計算、移動
	RECT	rWindow, rClient;
	GetWindowRect(_hWnd, &rWindow);
	GetClientRect(_hWnd, &rClient);
	int width = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + (int)WINDOW_WIDTH;
	int height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + (int)WINDOW_HEIGHT;
	// 設定を適応
	SetWindowPos(
		_hWnd,
		NULL,
		GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
		width - 1,
		height - 1,
		SWP_NOZORDER);

	// 画面の位置を保持
	GetWindowRect(_hWnd, &rWindow);
	GetClientRect(_hWnd, &rClient);
	windowPosLeftTop.x = (float)rWindow.left;
	windowPosLeftTop.y = (float)rWindow.top;

	// エラーが起こった場合
	if (!_hWnd)
		return false;

	// DirectX初期化関数
	if (FAILED(InitD3D(_hWnd)))
		return false;

	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);

	return true;
}

//==========================================================
// DirectX初期化関数
HRESULT InitD3D(HWND _hWnd)
{
	D3DPRESENT_PARAMETERS	d3dpp;

	if (NULL == (gpD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(gpD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&gpD3DDevice)))
	{
		return E_FAIL;
	}

	
	// 頂点フォーマットの指定は描画の直前に毎回行う
	gpD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	gpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gpD3DDevice->LightEnable(0, FALSE);

	return S_OK;
}

//==========================================================
// DirectX描画関数
void RunD3D(void)
{
	gpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(DEFAULT_BACKGROUND_COLOR_R, DEFAULT_BACKGROUND_COLOR_G, DEFAULT_BACKGROUND_COLOR_B), 1.0f, 0);

	if (SUCCEEDED(gpD3DDevice->BeginScene())){				// 描画開始
		// この間に描画処理を記述
		pCGameMain->Run();

		gpD3DDevice->EndScene();							// 描画終了
	}
	gpD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==========================================================
// DirectX終了処理関数
void DestroyD3D(void){
	if (gpD3DDevice != NULL){
		gpD3DDevice->Release();
	}
	if (gpD3D != NULL) {
		gpD3D->Release();
	}
}

#endif