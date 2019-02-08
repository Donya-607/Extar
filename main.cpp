#include <time.h>
#include <memory>
#include "DxLib.h"

#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Scene.h"
#include "SceneManager.h"

#include "SceneTitle.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif

int WINAPI WinMain(	HINSTANCE	hCurInst/*インスタンスハンドル*/,	HINSTANCE	hPrevInst/*名残*/,	
					LPSTR		lpsCmdLine/*コマンドライン引数*/,	int			nCmdShow/*ウィンドウの表示状態の設定*/)
{
	// エラーチェック
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = ;	// メモリリークが起きていた場合，出力される番号をこれに代入することで，該当箇所で動的確保した際に止めてくれるようになる
	// _crtBreakAlloc = ;
#endif

	SetOutApplicationLogValidFlag( FALSE );			// FALSE:デバッグ情報用の Log.txt を出力しない
	SetGraphMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16 );// SizeX, SizeY, ColorBitNum
	SetWindowIconID( 128 );							// タイトルバーのアイコンを変更
	SetMainWindowText( "KATANUKI" );				// タイトルバーのテキストを変更
	ChangeWindowMode( TRUE );						// TRUE:ウィンドウモードで起動
	SetDrawScreen( DX_SCREEN_BACK );				// DX_SCREEN_BACK:描画先を裏画面に設定

	if ( DxLib_Init() == -1 )
	{
		return -1;	// エラー処理, 終了させる
	}

	srand( scast<unsigned int>( time( NULL ) ) );

#if USE_IMGUI
	// メッセージをフックするウインドウプロージャを登録する
	// 注意：DxLib_Initを呼んだ後でないと　登録されない。
	SetHookWinProc( WndProc );
	//ＩＭＥを使用する
	SetUseIMEFlag( TRUE );

	// Setup ImGui binding
	ImGui_ImplDxLib_Init();

	ImGuiIO &io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF
	(
		"C:\\Windows\\Fonts\\consolab.ttf",
		15.0f,
		NULL,
		io.Fonts->GetGlyphRangesJapanese()
	);

#endif

	MusicInit();

	// シーンマネージャを生成
	SceneMng *pManager = nullptr;
	pManager = new SceneMng();

	// 最初のシーンを生成
	pManager->mpScene = new Title( pManager );//DEBUG:開始シーン変更
	pManager->Init();

	// ゲームループ
	while ( ScreenFlip() != -1 && ProcessMessage() != -1 && ClearDrawScreen() != -1 ) //エラーが起きていないなら
	{
#if USE_IMGUI

		ImGui_ImplDxLib_NewFrame();

#endif

		KeyboardUpdate();
		JoypadUpdate();
		// エスケープキーで終了
		if ( TRG( KEY_INPUT_ESCAPE ) )
		{
			break;
		}

		pManager->Update();

		pManager->Draw();

#if USE_IMGUI

		ImGui::Render();

#endif
	}

	// 終了処理
	delete pManager;
	pManager = nullptr;

#if USE_IMGUI

	ImGui_ImplDxLib_Shutdown();

#endif

	MusicUninit();

	DxLib_End();	// DXライブラリ終了処理

	return 0;
}

#if USE_IMGUI

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplDxLib_WndProcHandler( wnd, msg, wParam, lParam ) )
	{
		SetUseHookWinProcReturnValue( TRUE );
		return 1;
	}

	//IME関連はOSに任せる
	switch ( msg )
	{
		//WM_IME_SETCONTEXTを受け取ったときに
		//lParam = 0;とするとIME関連の表示が行われなくなります。
	case WM_IME_SETCONTEXT:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_NOTIFY:
	case WM_IME_REQUEST:
		SetUseHookWinProcReturnValue( TRUE );
		return DefWindowProc( wnd, msg, wParam, lParam );

	case WM_SYSCOMMAND:
		if ( ( wParam & 0xfff0 ) == SC_KEYMENU )	 // Disable ALT application menu
		{
			SetUseHookWinProcReturnValue( TRUE );
			return 0;
		}
		break;
	}

	return 0;
}

#endif