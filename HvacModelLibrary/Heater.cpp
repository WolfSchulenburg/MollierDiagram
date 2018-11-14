#include "stdafx.h"
#include "Heater.h"

IMPLEMENT_SERIAL(CHeater, CComponent, 0)

CHeater::CHeater(CComponent *pParent) : CComponent(pParent)
{

}

CHeater::~CHeater()
{
}

void CHeater::Serialize(CArchive& ar) {
	
	CComponent::Serialize(ar);
	/*
	if (ar.IsStoring())
		ar << empID << empName;
	else
		ar >> empID >> empName;
	*/
}
