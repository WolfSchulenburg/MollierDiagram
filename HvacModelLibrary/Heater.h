#pragma once

#include "Component.h"

class AFX_EXT_CLASS CHeater : public CComponent
{
public:
	DECLARE_SERIAL(CHeater);

	CHeater(CComponent *pParent = nullptr);
	~CHeater();

	void Serialize(CArchive& ar);
};

