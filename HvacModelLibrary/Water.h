#pragma once

class AFX_EXT_CLASS CWater : public CMedium
{
	DECLARE_SERIAL(CWater);

public:
	CWater();
	~CWater();

	double getThermalHeat();    // get thermal heat in kJ/kgK
	double getViscosity();      // viscosity in Pa*s
	double getDensity();        // density rho in kg/cbm

	void Serialize(CArchive& ar);

private:

	static double Cps(double t);
	static double Cvs(double v, double t);
	static double Hfg(double tc);
	static double Hs(double pkpa, double tc);
	static double Hsats(double tc);
	static double Hsatw(double tc);
	static double Psats(double tc);
	static double Ss(double pkpa, double tc);
	static double Ssats(double tc);
	static double Ssatw(double tc);
	static double Steamk(double t);
	static double Tpss(double p, double s);
	static double Tsats(double pkpa);
	static double Vissph(double t);
	static double Vissv(double p);
	static double Vs(double pkpa, double tc);
	static double Vsats(double pkpa, double tc);
	static double Vsatw(double tc);
	static double Wcp(double tw);
	static double Wk(double tw);
	static double Wmu(double tw);
	static double Wrho(double tw);

};

