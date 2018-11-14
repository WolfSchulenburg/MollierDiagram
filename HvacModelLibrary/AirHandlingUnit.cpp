#include "stdafx.h"

#include "Component.h"
#include "AirHandlingUnit.h"

IMPLEMENT_SERIAL(CAirHandlingUnit, CComponent, 0)

CAirHandlingUnit::CAirHandlingUnit(CComponent *pParent) 
	: CComponent(pParent)
{
}


CAirHandlingUnit::~CAirHandlingUnit()
{
}

void CAirHandlingUnit::Serialize(CArchive& ar) {

	CComponent::Serialize(ar);

	/*
	if (ar.IsStoring())
		ar << empID << empName;
	else
		ar >> empID >> empName;
	*/
}
