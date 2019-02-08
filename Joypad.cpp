#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Joypad.h"

const int	stickBorderLine = 350;	// �u�X���Ă���v�Ɣ��f���鋫�E�l�C0 ~ 1000
const int	triggerBorderLine = 128;// �u�g���K�[���͂��Ă���v�Ɣ��f���鋫�E�l�C0 ~ 255

// Xinput
static int	keysX[ButtonXEnd];		// �{�^���̓��͏��
static int	sticksInputX[StickXEnd];// �X�e�B�b�N�̌X��������
static int	sticksFrameX[StickXEnd];// �X�e�B�b�N�̓��̓t���[��
static int	triggersFrameX[LRTriggerXEnd];// �g���K�[�̓��̓t���[��
float		angleX[LRStickXEnd];	// �X�e�B�b�N�̊p�x
static XINPUT_STATE inputX[4];		// ���̑��̏ڍׂȏ��

// Dinput
static int	keysD[ButtonDEnd];		// �{�^���̓��͏��
static int	keysPOV[JoypadPOVEnd];	// �\���L�[�̓��̓t���[��
static int	sticksInputD[StickDEnd];// �X�e�B�b�N�̌X��������
static int	sticksFrameD[StickDEnd];// �X�e�B�b�N�̓��̓t���[��
float		angleD[LRStickDEnd];	// �X�e�B�b�N�̊p�x
static DINPUT_JOYSTATE inputD[4];	// ���̓��͏��

// HORI
static int keysH[HORIButtonEnd];

static int padConnectingNum;		// �R���g���[�����ڑ�����Ă��鐔
static int padType[4];				// �R���g���[���̎��
void JoipadInit()
{
	padConnectingNum = GetJoypadNum();
	if ( !padConnectingNum )
	{
		padType[0] = T_Nothing;

		return;
	}
	// else

	// �Ƃ肠�����C�P�Ԃ����m�F
	if ( CheckJoypadXInput( DX_INPUT_PAD1 ) )
	{
		padType[0] = T_XInput;
	}
	else
	{
		padType[0] = T_DInput;
	}
}
void JoypadUpdate()
{
	/* * * * * * * * * * * * * * * * *

	��������
	�����̐ڑ��ɑΉ��ł���悤
	inputX ��z��Ƃ��Ă��邪�C
	�܂��Ή�����悤�ɂ͑g��ł��Ȃ��B

	�����Ή�����̂Ȃ�C
	keysX, sticksInputX, sticksFrameX
	���z��i�Q�����j�ɂ��C
	���݂̏�����for���ň͂ނƂ悢���낤�B
	�܂��C���̍�
	�擾�֐����Y�ꂸ�ɕύX���邱�ƁB

	* * * * * * * * * * * * * * * * */

	// XInput
	{
		// �{�^���̓��͂�����			GetJoypadXInputState
		{
			GetJoypadXInputState( DX_INPUT_PAD1, &inputX[0] );

			for ( int i = 0; i < ButtonXEnd; i++ )
			{
				keysX[i] = ( inputX[0].Buttons[i] ) ? keysX[i] + 1 : 0;
			}
			// �g���K�[����
			{
				triggersFrameX[XT_LEFT] =
					( triggerBorderLine < inputX[0].LeftTrigger )
					? triggersFrameX[XT_LEFT] + 1
					: 0;

				triggersFrameX[XT_RIGHT] =
					( triggerBorderLine < inputX[0].RightTrigger )
					? triggersFrameX[XT_RIGHT] + 1
					: 0;
			}
		}
		// �X�e�B�b�N�̌X��������		GetJoypadAnalogInput
		{
			GetJoypadAnalogInput( &sticksInputX[X_STICK_L_X], &sticksInputX[X_STICK_L_Y], DX_INPUT_PAD1 );
			GetJoypadAnalogInputRight( &sticksInputX[X_STICK_R_X], &sticksInputX[X_STICK_R_Y], DX_INPUT_PAD1 );

			for ( int i = 0; i < StickXEnd; i++ )
			{
				sticksFrameX[i] = 
					( 
						sticksInputX[i] <= -stickBorderLine ||
						stickBorderLine <= sticksInputX[i] 
					) 
					? sticksFrameX[i] + 1 
					: 0;
			}
		}
		// �X�e�B�b�N�̊p�x������		
		{
			// L�X�e�B�b�N
			if ( !sticksInputX[X_STICK_L_X] && !sticksInputX[X_STICK_L_Y] )
			{
				angleX[X_LEFT] = 0;
			}
			else
			{
				angleX[X_LEFT] = ToDegree( atan2f( scast<float>( sticksInputX[X_STICK_L_Y] ), scast<float>( sticksInputX[X_STICK_L_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
				if ( angleX[X_LEFT] < 0 )
				{
					// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
					angleX[X_LEFT] += 360;
				}
			}
			// R�X�e�B�b�N
			if ( !sticksInputX[X_STICK_R_X] && !sticksInputX[X_STICK_R_Y] )
			{
				angleX[X_RIGHT] = 0;
			}
			else
			{
				angleX[X_RIGHT] = ToDegree( atan2f( scast<float>( sticksInputX[X_STICK_R_Y] ), scast<float>( sticksInputX[X_STICK_R_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
				if ( angleX[X_RIGHT] < 0 )
				{
					// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
					angleX[X_RIGHT] += 360;
				}
			}
		}
		// DEBUG:���ׂĂ̏�Ԃ��o�͂���
		/*
		{
			clsDx();
			printfDx
			( "\nStickL   : ( %d, %d )\nStickR   : ( %d, %d )\nPushL    : %d\nPushR    : %d\nTriggerL : %d\nTriggerR : %d\nSTART   : %d\nSELECT  : %d\n�k : %d, �q : %d\n�` : %d, �a : %d, �w : %d, �x : %d\n�� : %d, �� : %d, �� : %d, �� : %d\nAnalog Input\nLX : %d, LY : %d\nRX : %d, RY : %d\nAnalog Frame\nLX : %d, LY : %d\nRX : %d, RY : %d\n\nAngleX : %f, AngleY : %f",
				inputX[0].ThumbLX, inputX[0].ThumbLY,
				inputX[0].ThumbRX, inputX[0].ThumbRY,
				keysX[XB_PUSH_L],
				keysX[XB_PUSH_R],
				inputX[0].LeftTrigger,
				inputX[0].RightTrigger,
				keysX[XB_START],
				keysX[XB_SELECT],
				keysX[XB_L],
				keysX[XB_R],
				keysX[XB_A], 
				keysX[XB_B],
				keysX[XB_X],
				keysX[XB_Y],
				keysX[XB_UP],
				keysX[XB_DOWN],
				keysX[XB_LEFT],
				keysX[XB_RIGHT],
				sticksInputX[X_STICK_L_X],
				sticksInputX[X_STICK_L_Y],
				sticksInputX[X_STICK_R_X],
				sticksInputX[X_STICK_R_Y],
				sticksFrameX[X_STICK_L_X],
				sticksFrameX[X_STICK_L_Y],
				sticksFrameX[X_STICK_R_X],
				sticksFrameX[X_STICK_R_Y], 
				angleX[X_LEFT], 
				angleX[X_RIGHT]
			);
		}
		*/
	}
	// DirectInput
	{
		// �{�^���̓��͂�����			GetJoypadDirectInputState
		{
			GetJoypadDirectInputState( DX_INPUT_PAD1, &inputD[0] );
			for ( int i = 0; i < ButtonDEnd; i++ )
			{
				keysD[i] = ( inputD[0].Buttons[i] ) ? keysD[i] + 1 : 0;
			}
			// �\���L�[
			for ( int i = 0; i < JoypadPOVEnd; i++ )
			{
				keysPOV[i] = // 45�x�����݂̊p�x���C100�{�������̂��i�[����Ă���
					( ( i * 45 * 100 ) == inputD[0].POV[0] ) 
					? keysPOV[i] + 1 
					: 0;
			}
		}
		// �X�e�B�b�N�̌X��������		GetJoypadAnalogInput
		{
			GetJoypadAnalogInput( &sticksInputD[D_STICK_L_X], &sticksInputD[D_STICK_L_Y], DX_INPUT_PAD1 );
			GetJoypadAnalogInputRight( &sticksInputD[D_STICK_R_X], &sticksInputD[D_STICK_R_Y], DX_INPUT_PAD1 );

			for ( int i = 0; i < StickDEnd; i++ )
			{
				sticksFrameD[i] =
					(
						sticksInputD[i] <= -stickBorderLine ||
						stickBorderLine <= sticksInputD[i]
						)
					? sticksFrameD[i] + 1
					: 0;
			}
		}
		// �X�e�B�b�N�̊p�x������
		{
			// L�X�e�B�b�N
			if ( !sticksInputD[D_STICK_L_X] && !sticksInputD[D_STICK_L_Y] )
			{
				angleD[D_LEFT] = 0;
			}
			else
			{
				angleD[D_LEFT] = ( atan2f( scast<float>( sticksInputD[D_STICK_L_Y] ), scast<float>( sticksInputD[D_STICK_L_X] ) ) * 180.0f / PI ) + 90.0f/* �O�x����ɂ��邽�� */;
				if ( angleD[D_LEFT] < 0 )
				{
					// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
					angleD[D_LEFT] += 360;
				}
			}
			// R�X�e�B�b�N
			if ( !sticksInputD[D_STICK_R_X] && !sticksInputD[D_STICK_R_Y] )
			{
				angleD[D_RIGHT] = 0;
			}
			else
			{
				angleD[D_RIGHT] = ( atan2f( scast<float>( sticksInputD[D_STICK_R_Y] ), scast<float>( sticksInputD[D_STICK_R_X] ) ) * 180.0f / PI ) + 90.0f/* �O�x����ɂ��邽�� */;
				if ( angleD[D_RIGHT] < 0 )
				{
					// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
					angleD[D_RIGHT] += 360;
				}
			}
		}
		// DEBUG:���ׂĂ̏�Ԃ��o�͂���
		/*
		{
			clsDx();
			printfDx( "\nX : %d, Y : %d, Z : %d\nRZ : %d\n�\�� : %d\nAngle\nL : %6f, R : %6f\n", 
				inputD[0].X,
				inputD[0].Y,
				inputD[0].Z,
				inputD[0].Rz,
				inputD[0].POV[0],
				angleD[D_LEFT],
				angleD[D_RIGHT]
				);
			for ( int i = 0; i < ButtonDEnd; i++ )
			{
				printfDx( "Button[%d] : %d\n", i, keysD[i] );
			}

			printfDx( "�֐�\n" );
			for ( int i = 0; i < ButtonDEnd; i++ )
			{
				printfDx( "Button[%d] : %d\n", i, GetJoypadButtonD( scast<JoypadButtonD>( i ) ) );
			}
		}
		*/
	}

	// TODO:DirectInput���Ƃꂽ���ۂ��̂ŁC���Ƃ̓��b�p�[�֐��ƁC�Â����̂��폜����̂ƁC�Ȃǂ���

	// HORI�p�̓���(�Â�) GetJoypadInputState
	{
		int input = GetJoypadInputState(DX_INPUT_PAD1);

		for ( int i = 0; i < HORIButtonEnd; i++ )
		{
			keysH[i] = ( input & ( 1 << i ) ) ? keysH[i] + 1 : 0;
		}
	}
}

// XBox
int GetJoypadButtonX		( JoypadButtonX button )
{
	assert( 0 <= button && button < ButtonXEnd );

	return keysX[button];
}
int GetJoypadStickInputX	( JoypadStickX stick, int borderLine )
{
	assert( 0 <= stick && stick < StickXEnd );

	return ( sticksInputX[stick] <= -borderLine ) ? -1 : ( ( borderLine <= sticksInputX[stick] ) ? 1 : 0 );
}
int GetJoypadStickFrameX	( JoypadStickX stick )
{
	assert( 0 <= stick && stick < StickXEnd );

	return sticksFrameX[stick];
}
int GetJoypadTriggerFrameX	( JoypadLRTriggerX trigger )
{
	assert( 0 <= trigger && trigger < LRTriggerXEnd );

	return triggersFrameX[trigger];
}
int GetJoypadStateX			( JoypadStateX state, int padNum )
{
	assert( 0 <= state && state < StateXEnd );
	assert( 0 <= padNum && padNum < 4 );

	switch ( state )
	{
	case XS_STICK_X_L: return inputX[padNum].ThumbLX;		//break;
	case XS_STICK_Y_L: return inputX[padNum].ThumbLY;		//break;
	case XS_STICK_X_R: return inputX[padNum].ThumbRX;		//break;
	case XS_STICK_Y_R: return inputX[padNum].ThumbRY;		//break;
	case XS_TRIGGER_L: return inputX[padNum].LeftTrigger;	//break;
	case XS_TRIGGER_R: return inputX[padNum].RightTrigger;	//break;
	}

	assert( !"�G���[�FGetJoypadStateX");
	return NULL;
}
float GetJoypadStickAngleX	( JoypadLRStickX side )
{
	assert( 0 <= side || side < LRStickXEnd );

	return angleX[side];
}
bool IsTiltJoypadStickX		( JoypadLRStickX side )
{
	assert( side != LRStickXEnd );

	if ( side == X_RIGHT )
	{
		if ( GetJoypadStickFrameX( X_STICK_R_X ) ) { return true; }
		if ( GetJoypadStickFrameX( X_STICK_R_Y ) ) { return true; }
		// else
		return false;
	}
	// else

	if ( GetJoypadStickFrameX( X_STICK_L_X ) ) { return true; }
	if ( GetJoypadStickFrameX( X_STICK_L_Y ) ) { return true; }
	// else
	return false;
}

// DirectInput
int GetJoypadButtonD		( JoypadButtonD button )
{
	assert( 0 <= button && button < ButtonDEnd );

	return keysD[button];
}
int GetJoypadPOV_D			( JoypadPOV_D button )
{
	assert( 0 <= button || button <= JoypadPOVEnd );

	return keysPOV[button];
}
int GetJoypadStickInputD	( JoypadStickD stick, int borderLine )
{
	assert( 0 <= stick && stick < StickDEnd );

	return ( sticksInputD[stick] <= -borderLine ) ? -1 : ( ( borderLine <= sticksInputD[stick] ) ? 1 : 0 );
}
int GetJoypadStickFrameD	( JoypadStickD stick )
{
	assert( 0 <= stick && stick < StickDEnd );

	return sticksFrameD[stick];

}
float GetJoypadStickAngleD	( JoypadLRStickD side )
{
	assert( 0 <= side || side < LRStickDEnd );

	return angleD[side];
}
bool IsTiltJoypadStickD		( JoypadLRStickD side )
{
	assert( side != LRStickDEnd );

	if ( side == D_RIGHT )
	{
		if ( GetJoypadStickFrameD( D_STICK_R_X ) ) { return true; }
		if ( GetJoypadStickFrameD( D_STICK_R_Y ) ) { return true; }
		// else
		return false;
	}
	// else

	if ( GetJoypadStickFrameD( D_STICK_L_X ) ) { return true; }
	if ( GetJoypadStickFrameD( D_STICK_L_Y ) ) { return true; }
	// else
	return false;
}

// OLD
int GetJoypadHORI			( JOYPAD_BUTTON_HORI button )
{
	assert( H_DOWN <= button && button < HORIButtonEnd );
	return keysH[button];
}
