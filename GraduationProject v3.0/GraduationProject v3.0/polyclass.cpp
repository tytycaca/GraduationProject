#include "polyclass.h"
PolyClass::PolyClass()
{
}
PolyClass::~PolyClass()
{
}
void PolyClass::Initialize(int cntPoly)
{
	m_cntPolygon = cntPoly;
	//m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void PolyClass::Frame()
{
	//m_count++;
	if (timeGetTime() >= (m_startTime + 1000))
	{
		//m_count = 0;
		m_startTime = timeGetTime();
	}
	return;
}

int PolyClass::GetCntPolygon()
{
	return m_cntPolygon;
}