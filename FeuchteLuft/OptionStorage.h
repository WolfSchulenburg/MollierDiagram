#pragma once
class COptionStorage : public CRegistry
{
public:
	COptionStorage();
	~COptionStorage();

	double GetStandardAthmosphericPressure() { return 101.325; }

};

