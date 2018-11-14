#pragma once
#include <afx.h>

class CStatePoint : public CObject
{
	DECLARE_SERIAL(CStatePoint);
public:
	CStatePoint();
	~CStatePoint();

	double GetTemperature() { return m_fTemperature; }
	double GetAbsHumidity() { return m_fAbsHumidity; }

	void Serialize(CArchive & ar);

private:
	double m_fTemperature;
	double m_fAbsHumidity;
};

