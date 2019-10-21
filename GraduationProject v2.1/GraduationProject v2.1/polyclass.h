#pragma once
#ifndef _POLYCLASS_H_
#define _POLYCLASS_H_
/////////////
// LINKING //
/////////////
#pragma comment(lib, "winmm.lib")
//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
class PolyClass
{
public:
	PolyClass();
	~PolyClass();
	void Initialize(int);
	void Frame();
	int GetCntPolygon();
private:
	int m_cntPolygon/*, m_count*/;
	unsigned long m_startTime;
};
#endif