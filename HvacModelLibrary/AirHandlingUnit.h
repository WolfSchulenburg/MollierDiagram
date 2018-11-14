#pragma once

#include "Component.h"

class AFX_EXT_CLASS CAirHandlingUnit : public CComponent
{
public:
	DECLARE_SERIAL(CAirHandlingUnit);

	CAirHandlingUnit(CComponent *pParent = nullptr);
	virtual ~CAirHandlingUnit();

	void Serialize(CArchive& ar);

private:

};

