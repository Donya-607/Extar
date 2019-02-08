#ifndef INCLUDED_KEYBOARD_H_
#define INCLUDED_KEYBOARD_H_

//--------------------
//
//		Keyboard.h
//
//--------------------

// キーを押した瞬間かどうか
#define TRG( x )		( 1 == GetKeyboard( x ) )

// キーを押しているかどうか
#define PRESS( x )	( GetKeyboard( x ) )

// 使用するボタンのマクロ
#define BUTTON_ACCEL_K		( KEY_INPUT_Z )
#define BUTTON_ATTACK_K		( KEY_INPUT_X )

int  GetKeyboard( const int KeyCode );
void KeyboardUpdate();

#endif // INCLUDED_KEYBOARD_H_
