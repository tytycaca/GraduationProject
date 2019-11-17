#pragma once
#ifndef _SCREENSIZECLASS_H_
#define _SCREENSIZECLASS_H_
/////////////
// LINKING //
/////////////
#pragma comment(lib, "winmm.lib")
//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
class ScreenSizeClass
{
public:
	ScreenSizeClass();
	~ScreenSizeClass();
	void Initialize(int, int);
	void Frame();
	int GetScreenSizeX();
	int GetScreenSizeY();
private:
	int m_screenSizeX, m_screenSizeY/*, m_count*/;
	unsigned long m_startTime;
};
#endif