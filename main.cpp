#include <time.h>
#include <memory>
#include "DxLib.h"

#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Scene.h"
#include "SceneManager.h"

#if DEBUG_MODE

// #include "SceneGame.h"
#include "SceneTitle.h"

#else

#include "SceneTitle.h"

#endif // DEBUG_MODE

void FontPrepare();
void FontRelease();

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif // USE_IMGUI

int WINAPI WinMain(	HINSTANCE	hCurInst/*インスタンスハンドル*/,	HINSTANCE	hPrevInst/*名残*/,	
					LPSTR		lpsCmdLine/*コマンドライン引数*/,	int			nCmdShow/*ウィンドウの表示状態の設定*/)
{
	// エラーチェック
#if defined( DEBUG ) | defined( _DEBUG )
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = ;	// メモリリークが起きていた場合，出力される番号をこれに代入することで，該当箇所で動的確保した際に止めてくれるようになる
	// _crtBreakAlloc = ;
#endif

	SetOutApplicationLogValidFlag( FALSE );			// FALSE:デバッグ情報用の Log.txt を出力しない
	SetGraphMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16 );// SizeX, SizeY, ColorBitNum
	SetWindowIconID( 128 );							// タイトルバーのアイコンを変更
	SetMainWindowText( "エクスタ" );					// タイトルバーのテキストを変更
	ChangeWindowMode( TRUE );						// TRUE:ウィンドウモード, FALSE:フルスクリーンモード

	if ( DxLib_Init() == -1 )
	{
		return -1;	// エラー処理, 終了させる
	}

	SetMouseDispFlag( FALSE );						// FALSE:マウスカーソルの非表示
	SetDrawScreen( DX_SCREEN_BACK );				// DX_SCREEN_BACK:描画先を裏画面に設定

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

#endif // USE_IMGUI
	
	FontPrepare();

	MusicInit();

	JoypadInit();

	FadeImage::Load();
	Fade::GetInstance()->Init
	(
		NULL,
		{ scast<float>( SCREEN_WIDTH ), scast<float>( SCREEN_HEIGHT ) }
	);
	// 初回だけ，フェード終了フラグをオンにしておきたい
	Fade::GetInstance()->Update();

	// シーンマネージャを生成
	SceneMng *pManager = nullptr;
	pManager = new SceneMng();

	// 最初のシーンを生成

#if DEBUG_MODE

	// pManager->mpScene = new Game( pManager );
	pManager->mpScene = new Title( pManager );

#else

	pManager->mpScene = new Title( pManager );

#endif // DEBUG_MODE

	pManager->Init();

	// ゲームループ
	while ( ScreenFlip() != -1 && ProcessMessage() != -1 && ClearDrawScreen() != -1 ) //エラーが起きていないなら
	{
#if USE_IMGUI

		ImGui_ImplDxLib_NewFrame();

#endif // USE_IMGUI

		KeyboardUpdate();

		if ( TRG( KEY_INPUT_ESCAPE ) ) { break; }
		if ( TRG( KEY_INPUT_F5 ) ) { JoypadInit(); }

		JoypadUpdate();

		pManager->Update();

		pManager->Draw();

#if USE_IMGUI

		ImGui::Render();

#endif // USE_IMGUI
	}

	// 終了処理
	delete pManager;
	pManager = nullptr;

	FadeImage::Release();

#if USE_IMGUI

	ImGui_ImplDxLib_Shutdown();

#endif // USE_IMGUI

	JoypadUninit();

	MusicUninit();

	FontRelease();

	DxLib_End();	// DXライブラリ終了処理

	return 0;
}

void FontPrepare()
{
	LPCSTR fontname = ".\\Data\\Font\\JF-Dot-K12.TTF";
	AddFontResourceEx( fontname, FR_PRIVATE, 0 );
}
void FontRelease()
{
	// https://docs.microsoft.com/ja-jp/windows/desktop/api/wingdi/nf-wingdi-removefontresourceexa

	LPCSTR fontname = ".\\Data\\Font\\JF-Dot-K12.TTF";
	RemoveFontResourceEx( fontname, FR_PRIVATE, 0 );
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

#endif // USE_IMGUI