#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_
////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//////////////
// INCLUDES //
//////////////
#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();
	//void Initialize();
	//void KeyDown(unsigned int);
	//void KeyUp(unsigned int);
	//bool IsKeyDown(unsigned int);

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();
	bool IsEscapePressed();
	bool IsKeyPressed(int);
	void GetMouseLocation(int&, int&);
	void GetMouseRotation(DIMOUSESTATE&);
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
private:
	//bool m_keys[256];

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};
#endif