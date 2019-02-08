#ifndef INCLUDED_KEYBOARD_H_
#define INCLUDED_KEYBOARD_H_

//--------------------
//
//		Keyboard.h
//
//--------------------

// �L�[���������u�Ԃ��ǂ���
#define TRG( x )		( 1 == GetKeyboard( x ) )

// �L�[�������Ă��邩�ǂ���
#define PRESS( x )	( GetKeyboard( x ) )

// �g�p����{�^���̃}�N��
#define BUTTON_ACCEL_K		( KEY_INPUT_Z )
#define BUTTON_ATTACK_K		( KEY_INPUT_X )

int  GetKeyboard( const int KeyCode );
void KeyboardUpdate();

#endif // INCLUDED_KEYBOARD_H_
