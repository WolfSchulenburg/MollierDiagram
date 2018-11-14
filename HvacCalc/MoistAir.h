#pragma once
#include <afx.h>

class CMoistAir : public CMedium
{
public:
	CMoistAir();
	~CMoistAir();

	//double PSA(double aTemperature);
	//double TS(double aTemperature);
private:
	double fpws(double tdb);
	double ftdew(double w, double patm);
	double fpww(double w, double patm);
	double fwpw(double pw, double patm);
};

