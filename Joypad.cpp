#include <array>
#include <vector>
#include <memory>
#include <math.h>

#include "DxLib.h"
#include "Common.h"	// assert.h�̓ǂݍ���, ToDegree�萔, scast�萔�̎g�p
#include "Joypad.h"

const int	stickBorderLine		= 350;	// 0 ~ 1000, �u�X���Ă���v�Ɣ��f���鋫�E�l
const int	triggerBorderLine	= 128;	// 0 ~ 255,  �u�g���K�[���͂��Ă���v�Ɣ��f���鋫�E�l

constexpr std::array<int, 4> INPUT_TYPES =
{
	DX_INPUT_PAD1, DX_INPUT_PAD2,
	DX_INPUT_PAD3, DX_INPUT_PAD4
};

struct XInput
{
	int		keys[ButtonXEnd];				// �{�^���̓��͏��
	int		sticksInput[StickXEnd];			// �X�e�B�b�N�̌X��������
	int		sticksFrame[StickXEnd];			// �X�e�B�b�N�̓��̓t���[��
	int		triggersFrame[LRTriggerXEnd];	// �g���K�[�̓��̓t���[��
	float	angle[LRStickXEnd];				// Degree
	XINPUT_STATE rowInput;
public:
	XInput() : keys(),
		sticksInput(), sticksFrame(),
		triggersFrame(), angle(),
		rowInput()
	{}
};
struct DInput
{
	int		keys[ButtonDEnd];		// �{�^���̓��͏��
	int		keysPOV[JoypadPOVEnd];	// �\���L�[�̓��̓t���[��
	int		sticksInput[StickDEnd];	// �X�e�B�b�N�̌X��������
	int		sticksFrame[StickDEnd];	// �X�e�B�b�N�̓��̓t���[��
	float	angle[LRStickDEnd];		// Degree
	DINPUT_JOYSTATE rowInput;
public:
	DInput() : keys(), keysPOV(),
		sticksInput(), sticksFrame(),
		angle(), rowInput()
	{}
};
struct Joypad
{
	// �g�����̃|�C���^�ɂ����C���̂�����`�ɂȂ�

	std::unique_ptr<XInput> pXInput;
	std::unique_ptr<DInput> pDInput;
public:
	Joypad() : pXInput( nullptr ), pDInput( nullptr ) {}
};

static std::vector<Joypad> pad;

/*
// XInput
static int	 keysX[ButtonXEnd];		// �{�^���̓��͏��
static int	 sticksInputX[StickXEnd];// �X�e�B�b�N�̌X��������
static int	 sticksFrameX[StickXEnd];// �X�e�B�b�N�̓��̓t���[��
static int	 triggersFrameX[LRTriggerXEnd];// �g���K�[�̓��̓t���[��
static float angleX[LRStickXEnd];	// �X�e�B�b�N�̊p�x
static XINPUT_STATE inputX[4];		// ���̑��̏ڍׂȏ��

// DInput
static int	 keysD[ButtonDEnd];		// �{�^���̓��͏��
static int	 keysPOV[JoypadPOVEnd];	// �\���L�[�̓��̓t���[��
static int	 sticksInputD[StickDEnd];// �X�e�B�b�N�̌X��������
static int	 sticksFrameD[StickDEnd];// �X�e�B�b�N�̓��̓t���[��
static float angleD[LRStickDEnd];	// �X�e�B�b�N�̊p�x
static DINPUT_JOYSTATE inputD[4];	// ���̓��͏��
*/

void JoypadInit()
{
	ReSetupJoypad();
	std::vector<Joypad>().swap( pad );

	int num = GetJoypadNum();
	if ( !num )
	{
		return;
	}
	// else

	for (
			int i = 0;
			num && i < scast<int>( INPUT_TYPES.size() );
			i++
		)
	{
		if ( CheckJoypadXInput( INPUT_TYPES[i] ) )
		{
			pad.push_back( Joypad() );
			pad.back().pXInput.reset( new XInput() );
			num -= 2;
		}
		else
		{
			pad.push_back( Joypad() );
			pad.back().pDInput.reset( new DInput() );
			num -= 1;
		}
	}
}
void JoypadUninit()
{
	std::vector<Joypad>().swap( pad );
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

	for ( int padIndex = 0; padIndex < scast<int>( pad.size() ); padIndex++ )
	{
		// XInput
		if ( pad.at( padIndex ).pXInput )
		{
			auto &&ref = pad.at( padIndex ).pXInput;

			// �{�^���̓��͂�����			GetJoypadXInputState
			{
				GetJoypadXInputState( INPUT_TYPES[padIndex], &ref->rowInput );

				for ( int i = 0; i < ButtonXEnd; i++ )
				{
					ref->keys[i] = ( ref->rowInput.Buttons[i] ) ? ref->keys[i] + 1 : 0;
				}
				// �g���K�[����
				{
					ref->triggersFrame[XT_LEFT] =
						( triggerBorderLine < ref->rowInput.LeftTrigger )
						? ref->triggersFrame[XT_LEFT] + 1
						: 0;

					ref->triggersFrame[XT_RIGHT] =
						( triggerBorderLine < ref->rowInput.RightTrigger )
						? ref->triggersFrame[XT_RIGHT] + 1
						: 0;
				}
			}
			// �X�e�B�b�N�̌X��������		GetJoypadAnalogInput
			{
				GetJoypadAnalogInput(		&ref->sticksInput[X_STICK_L_X], &ref->sticksInput[X_STICK_L_Y],  INPUT_TYPES[padIndex] );
				GetJoypadAnalogInputRight(	&ref->sticksInput[X_STICK_R_X], &ref->sticksInput[X_STICK_R_Y],  INPUT_TYPES[padIndex] );

				for ( int i = 0; i < StickXEnd; i++ )
				{
					if ( stickBorderLine <= ref->sticksInput[i] )
					{
						ref->sticksFrame[i] += 1;
					}
					else if ( ref->sticksInput[i] <= -stickBorderLine )
					{
						ref->sticksFrame[i] -= 1;
					}
					else
					{
						ref->sticksFrame[i] = 0;
					}
				}
			}
			// �X�e�B�b�N�̊p�x������		
			{
				// L�X�e�B�b�N
				if ( !ref->sticksInput[X_STICK_L_X] && !ref->sticksInput[X_STICK_L_Y] )
				{
					ref->angle[X_LEFT] = 0;
				}
				else
				{
					ref->angle[X_LEFT] = ToDegree( atan2f( scast<float>( ref->sticksInput[X_STICK_L_Y] ), scast<float>( ref->sticksInput[X_STICK_L_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
					ref->angle[X_LEFT] = ToDegree( atan2f( scast<float>( ref->sticksInput[X_STICK_L_Y] ), scast<float>( ref->sticksInput[X_STICK_L_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
					if ( ref->angle[X_LEFT] < 0 )
					{
						// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
						ref->angle[X_LEFT] += 360;
					}
				}
				// R�X�e�B�b�N
				if ( !ref->sticksInput[X_STICK_R_X] && !ref->sticksInput[X_STICK_R_Y] )
				{
					ref->angle[X_RIGHT] = 0;
				}
				else
				{
					ref->angle[X_RIGHT] = ToDegree( atan2f( scast<float>( ref->sticksInput[X_STICK_R_Y] ), scast<float>( ref->sticksInput[X_STICK_R_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
					if ( ref->angle[X_RIGHT] < 0 )
					{
						// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
						ref->angle[X_RIGHT] += 360;
					}
				}
			}
			// DEBUG:���ׂĂ̏�Ԃ��o�͂���
			/*
			{
				clsDx();
				printfDx
				( "\nStickL   : ( %d, %d )\nStickR   : ( %d, %d )\nPushL    : %d\nPushR    : %d\nTriggerL : %d\nTriggerR : %d\nSTART   : %d\nSELECT  : %d\n�k : %d, �q : %d\n�` : %d, �a : %d, �w : %d, �x : %d\n�� : %d, �� : %d, �� : %d, �� : %d\nAnalog Input\nLX : %d, LY : %d\nRX : %d, RY : %d\nAnalog Frame\nLX : %d, LY : %d\nRX : %d, RY : %d\n\nAngleX : %f, AngleY : %f",
					ref->rowInput.ThumbLX, ref->rowInput.ThumbLY,
					ref->rowInput.ThumbRX, ref->rowInput.ThumbRY,
					ref->keys[XB_PUSH_L],
					ref->keys[XB_PUSH_R],
					ref->rowInput.LeftTrigger,
					ref->rowInput.RightTrigger,
					ref->keys[XB_START],
					ref->keys[XB_SELECT],
					ref->keys[XB_L],
					ref->keys[XB_R],
					ref->keys[XB_A],
					ref->keys[XB_B],
					ref->keys[XB_X],
					ref->keys[XB_Y],
					ref->keys[XB_UP],
					ref->keys[XB_DOWN],
					ref->keys[XB_LEFT],
					ref->keys[XB_RIGHT],
					ref->sticksInput[X_STICK_L_X],
					ref->sticksInput[X_STICK_L_Y],
					ref->sticksInput[X_STICK_R_X],
					ref->sticksInput[X_STICK_R_Y],
					ref->sticksFrame[X_STICK_L_X],
					ref->sticksFrame[X_STICK_L_Y],
					ref->sticksFrame[X_STICK_R_X],
					ref->sticksFrame[X_STICK_R_Y],
					ref->angle[X_LEFT],
					ref->angle[X_RIGHT]
				);
			}
			*/

			continue;
		}
		// else

		// DInput
		if ( pad.at( padIndex ).pDInput )
		{
			auto &&ref = pad.at( padIndex ).pDInput;

			// �{�^���̓��͂�����			GetJoypadDirectInputState
			{
				GetJoypadDirectInputState( INPUT_TYPES[padIndex], &ref->rowInput );
				for ( int i = 0; i < ButtonDEnd; i++ )
				{
					ref->keys[i] = ( ref->rowInput.Buttons[i] ) ? ref->keys[i] + 1 : 0;
				}
				// �\���L�[
				for ( int i = 0; i < JoypadPOVEnd; i++ )
				{
					ref->keysPOV[i] = // 45�x�����݂̊p�x���C100�{�������̂��i�[����Ă���
						( ( i * 45 * 100 ) == ref->rowInput.POV[0] )
						? ref->keysPOV[i] + 1
						: 0;
				}
			}
			// �X�e�B�b�N�̌X��������		GetJoypadAnalogInput
			{
				GetJoypadAnalogInput(		&ref->sticksInput[D_STICK_L_X], &ref->sticksInput[D_STICK_L_Y], INPUT_TYPES[padIndex] );
				GetJoypadAnalogInputRight(	&ref->sticksInput[D_STICK_R_X], &ref->sticksInput[D_STICK_R_Y], INPUT_TYPES[padIndex] );

				for ( int i = 0; i < StickDEnd; i++ )
				{
					if ( stickBorderLine <= ref->sticksInput[i] )
					{
						ref->sticksFrame[i] += 1;
					}
					else if ( ref->sticksInput[i] <= -stickBorderLine )
					{
						ref->sticksFrame[i] -= 1;
					}
					else
					{
						ref->sticksFrame[i] = 0;
					}
				}
			}
			// �X�e�B�b�N�̊p�x������
			{
				// L�X�e�B�b�N
				if ( !ref->sticksInput[D_STICK_L_X] && !ref->sticksInput[D_STICK_L_Y] )
				{
					ref->angle[D_LEFT] = 0;
				}
				else
				{
					ref->angle[D_LEFT] = ToDegree( atan2f( scast<float>( ref->sticksInput[D_STICK_L_Y] ), scast<float>( ref->sticksInput[D_STICK_L_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
					if ( ref->angle[D_LEFT] < 0 )
					{
						// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
						ref->angle[D_LEFT] += 360;
					}
				}
				// R�X�e�B�b�N
				if ( !ref->sticksInput[D_STICK_R_X] && !ref->sticksInput[D_STICK_R_Y] )
				{
					ref->angle[D_RIGHT] = 0;
				}
				else
				{
					ref->angle[D_RIGHT] = ToDegree( atan2f( scast<float>( ref->sticksInput[D_STICK_R_Y] ), scast<float>( ref->sticksInput[D_STICK_R_X] ) ) ) + 90.0f/* �O�x����ɂ��邽�� */;
					if ( ref->angle[D_RIGHT] < 0 )
					{
						// -180 ~ 180 �̐��l���C0 ~ 360 �ɂ��Ă���
						ref->angle[D_RIGHT] += 360;
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

			continue;
		}
		// else

		assert( !"Error : Joypad type is not expected type." );
		exit( EXIT_FAILURE );
		return;
	}

	// TODO:DirectInput���Ƃꂽ���ۂ��̂ŁC���Ƃ̓��b�p�[�֐��ƁC�Â����̂��폜����̂ƁC�Ȃǂ���
}

// XBox
int GetJoypadButtonX		( JoypadButtonX button )
{
	assert( 0 <= button && button < ButtonXEnd );

	return pad.at( 0 ).pXInput->keys[button];
}
int GetJoypadStickInputX	( JoypadStickX stick, int borderLine )
{
	assert( 0 <= stick && stick < StickXEnd );

	return ( pad.at( 0 ).pXInput->sticksInput[stick] <= -borderLine )
		? -1
		:	(
				( borderLine <= pad.at( 0 ).pXInput->sticksInput[stick] )
				? 1
				: 0
			);
}
int GetJoypadStickFrameX	( JoypadStickX stick )
{
	assert( 0 <= stick && stick < StickXEnd );

	return pad.at( 0 ).pXInput->sticksFrame[stick];
}
int GetJoypadTriggerFrameX	( JoypadLRTriggerX trigger )
{
	assert( 0 <= trigger && trigger < LRTriggerXEnd );

	return pad.at( 0 ).pXInput->triggersFrame[trigger];
}
int GetJoypadStateX			( JoypadStateX state, int padNum )
{
	assert( 0 <= state && state < StateXEnd );
	assert( 0 <= padNum && padNum < 4 );

	switch ( state )
	{
	case XS_STICK_X_L: return pad.at( 0 ).pXInput->rowInput.ThumbLX;		//break;
	case XS_STICK_Y_L: return pad.at( 0 ).pXInput->rowInput.ThumbLY;		//break;
	case XS_STICK_X_R: return pad.at( 0 ).pXInput->rowInput.ThumbRX;		//break;
	case XS_STICK_Y_R: return pad.at( 0 ).pXInput->rowInput.ThumbRY;		//break;
	case XS_TRIGGER_L: return pad.at( 0 ).pXInput->rowInput.LeftTrigger;	//break;
	case XS_TRIGGER_R: return pad.at( 0 ).pXInput->rowInput.RightTrigger;	//break;
	}

	assert( !"�G���[�FGetJoypadStateX");
	return NULL;
}
float GetJoypadStickAngleX	( JoypadLRStickX side )
{
	assert( 0 <= side || side < LRStickXEnd );

	return pad.at( 0 ).pXInput->angle[side];
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

	return pad.at( 0 ).pDInput->keys[button];
}
int GetJoypadPOV_D			( JoypadPOV_D button )
{
	assert( 0 <= button || button <= JoypadPOVEnd );

	return pad.at( 0 ).pDInput->keysPOV[button];
}
int GetJoypadStickInputD	( JoypadStickD stick, int borderLine )
{
	assert( 0 <= stick && stick < StickDEnd );

	return ( pad.at( 0 ).pDInput->sticksInput[stick] <= -borderLine )
		? -1
		:	(
				( borderLine <= pad.at( 0 ).pDInput->sticksInput[stick] )
				? 1
				: 0
			);
}
int GetJoypadStickFrameD	( JoypadStickD stick )
{
	assert( 0 <= stick && stick < StickDEnd );

	return pad.at( 0 ).pDInput->sticksFrame[stick];

}
float GetJoypadStickAngleD	( JoypadLRStickD side )
{
	assert( 0 <= side || side < LRStickDEnd );

	return pad.at( 0 ).pDInput->angle[side];
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
