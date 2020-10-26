#include "pch.h"
#include "Input.h"
#include <Windows.h>

std::map<const char*, std::map<int, bool>>	Input::m_registred_calls = std::map<const char*, std::map<int, bool>>();

int Input::GetKeyPressedId() {
	BYTE* arr = new BYTE[256];
	int result = 0;

	memset(arr, 0, sizeof(256));
	GetKeyState(0);
	if (GetKeyboardState(arr)) {
		for (int i = 0; i < 256; i++)
			if ((arr[i] >> 7) == true) {
				result = i;
				break;
			}
	}

	delete[] arr;
	return result;
}

bool Input::BaseGetKey(KeyCode key) {
	BYTE* arr = new BYTE[256];
	bool result = false;

	memset(arr, 0, sizeof(256));
	GetKeyState(0);
	if (GetKeyboardState(arr)) {
		int temp = (int)arr[(int)key];
		temp >>= 7;
		result = (bool)temp;
	}
	
	delete[] arr;

	return result;
}
