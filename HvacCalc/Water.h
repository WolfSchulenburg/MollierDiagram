#pragma once

class CWater : public CMedium
{
public:
	CWater();
	~CWater();

	double getThermalHeat();    // get thermal heat in kJ/kgK
	double getViscosity();      // viscosity in Pa*s
	double getDensity();        // density rho in kg/cbm

};

