#include "stdafx.h"
#include "StatePoint.h"

IMPLEMENT_SERIAL(CStatePoint, CObject, 0);

CList<CStatePoint*> listStatePoints;


CStatePoint::CStatePoint()
{
}


CStatePoint::~CStatePoint()
{
}

void CStatePoint::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
		ar << m_fTemperature << m_fAbsHumidity;
	else
		ar >> m_fTemperature >> m_fAbsHumidity;
}