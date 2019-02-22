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
#define PRESS( x )		( GetKeyboard( x ) )

#define PRESS_CTRL		( PRESS( KEY_INPUT_LCONTROL ) || PRESS( KEY_INPUT_RCONTROL ) )
#define PRESS_SHIFT		( PRESS( KEY_INPUT_LSHIFT )   || PRESS( KEY_INPUT_RSHIFT ) )

int  GetKeyboard( const int KeyCode );
void KeyboardUpdate();

#endif // INCLUDED_KEYBOARD_H_
