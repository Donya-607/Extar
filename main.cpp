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

int WINAPI WinMain(	HINSTANCE	hCurInst/*�C���X�^���X�n���h��*/,	HINSTANCE	hPrevInst/*���c*/,	
					LPSTR		lpsCmdLine/*�R�}���h���C������*/,	int			nCmdShow/*�E�B���h�E�̕\����Ԃ̐ݒ�*/)
{
	// �G���[�`�F�b�N
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = ;	// ���������[�N���N���Ă����ꍇ�C�o�͂����ԍ�������ɑ�����邱�ƂŁC�Y���ӏ��œ��I�m�ۂ����ۂɎ~�߂Ă����悤�ɂȂ�
	// _crtBreakAlloc = ;
#endif

	SetOutApplicationLogValidFlag( FALSE );			// FALSE:�f�o�b�O���p�� Log.txt ���o�͂��Ȃ�
	SetGraphMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16 );// SizeX, SizeY, ColorBitNum
	SetWindowIconID( 128 );							// �^�C�g���o�[�̃A�C�R����ύX
	SetMainWindowText( "KATANUKI" );				// �^�C�g���o�[�̃e�L�X�g��ύX
	ChangeWindowMode( TRUE );						// TRUE:�E�B���h�E���[�h�ŋN��
	SetDrawScreen( DX_SCREEN_BACK );				// DX_SCREEN_BACK:�`���𗠉�ʂɐݒ�

	if ( DxLib_Init() == -1 )
	{
		return -1;	// �G���[����, �I��������
	}

	srand( scast<unsigned int>( time( NULL ) ) );

#if USE_IMGUI
	// ���b�Z�[�W���t�b�N����E�C���h�E�v���[�W����o�^����
	// ���ӁFDxLib_Init���Ă񂾌�łȂ��Ɓ@�o�^����Ȃ��B
	SetHookWinProc( WndProc );
	//�h�l�d���g�p����
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

	// �V�[���}�l�[�W���𐶐�
	SceneMng *pManager = nullptr;
	pManager = new SceneMng();

	// �ŏ��̃V�[���𐶐�
	pManager->mpScene = new Title( pManager );//DEBUG:�J�n�V�[���ύX
	pManager->Init();

	// �Q�[�����[�v
	while ( ScreenFlip() != -1 && ProcessMessage() != -1 && ClearDrawScreen() != -1 ) //�G���[���N���Ă��Ȃ��Ȃ�
	{
#if USE_IMGUI

		ImGui_ImplDxLib_NewFrame();

#endif

		KeyboardUpdate();
		JoypadUpdate();
		// �G�X�P�[�v�L�[�ŏI��
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

	// �I������
	delete pManager;
	pManager = nullptr;

#if USE_IMGUI

	ImGui_ImplDxLib_Shutdown();

#endif

	MusicUninit();

	DxLib_End();	// DX���C�u�����I������

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

	//IME�֘A��OS�ɔC����
	switch ( msg )
	{
		//WM_IME_SETCONTEXT���󂯎�����Ƃ���
		//lParam = 0;�Ƃ����IME�֘A�̕\�����s���Ȃ��Ȃ�܂��B
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