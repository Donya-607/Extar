#include "DxLib.h"
#include "Keyboard.h"

const int KEYBOARD_NUM = 256;

static char keys[KEYBOARD_NUM] = { 0 };

void KeyboardUpdate()
{
	char tmp[KEYBOARD_NUM];

	GetHitKeyStateAll(tmp);

	for (int i = 0; i < KEYBOARD_NUM; i++)
	{
		keys[i] = (tmp[i] != 0) ? keys[i] + 1 : 0;
	}
}

int GetKeyboard(const int keyCode) //KeyCodeのキーボード入力状態を返す
{
	return keys[keyCode];
}