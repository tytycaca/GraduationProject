#include "screensizeclass.h"
ScreenSizeClass::ScreenSizeClass()
{
}
ScreenSizeClass::~ScreenSizeClass()
{
}
void ScreenSizeClass::Initialize(int x, int y)
{
	m_screenSizeX = x;
	m_screenSizeY = y;
	//m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void ScreenSizeClass::Frame()
{
	//m_count++;
	if (timeGetTime() >= (m_startTime + 1000))
	{
		//m_count = 0;
		m_startTime = timeGetTime();
	}
	return;
}

int ScreenSizeClass::GetScreenSizeX()
{
	return m_screenSizeX;
}

int ScreenSizeClass::GetScreenSizeY()
{
	return m_screenSizeY;
}