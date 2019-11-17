#include "objclass.h"
ObjClass::ObjClass()
{
}
ObjClass::~ObjClass()
{
}
void ObjClass::Initialize(int cntObj)
{
	m_cntObject = cntObj;
	//m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void ObjClass::Frame()
{
	//m_count++;
	if (timeGetTime() >= (m_startTime + 1000))
	{
		//m_count = 0;
		m_startTime = timeGetTime();
	}
	return;
}

int ObjClass::GetCntObject()
{
	return m_cntObject;
}