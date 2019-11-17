#pragma once
#ifndef _OBJCLASS_H_
#define _OBJCLASS_H_
/////////////
// LINKING //
/////////////
#pragma comment(lib, "winmm.lib")
//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
class ObjClass
{
public:
	ObjClass();
	~ObjClass();
	void Initialize(int);
	void Frame();
	int GetCntObject();
private:
	int m_cntObject/*, m_count*/;
	unsigned long m_startTime;
};
#endif