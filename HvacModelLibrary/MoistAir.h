#pragma once
#include <afx.h>

#include "Medium.h"

class AFX_EXT_CLASS CMoistAir : public CMedium
{
	DECLARE_SERIAL(CMoistAir);

public:
	CMoistAir();
	CMoistAir(double dryBulbTemp, double absoluteHumidity);
	~CMoistAir();

	//double PSA(double aTemperature);
	//double TS(double aTemperature);

	double AbsoluteHumidity() { return m_fAbsoluteHumid; }
	double DryBulbTemperature() { return m_fDryBulbTemp;  }
	double WetBulbTemperature() { return m_fWetBulbTemp;  }

	static double CalculateLatentHeat(CMoistAir& mAir) 
	{
		return (((1.846 * mAir.DryBulbTemperature()) + 2501.0) * mAir.AbsoluteHumidity());
	}
		
	static double CalculateLatentHeatDifference(CMoistAir& mAir1, CMoistAir& mAir2)
	{
		return ((mAir1.AbsoluteHumidity() - mAir2.AbsoluteHumidity()) * 2501.0);
	}

	static double CalculateSensibleHeat(CMoistAir& mAir)
	{ 
		return (mAir.DryBulbTemperature() * 1.006); 
	}

	static double CalculateSensibleHeatDifference(CMoistAir& mAir1, CMoistAir& mAir2)
	{
		return (((1.006 + (1.846 * mAir1.AbsoluteHumidity())) * mAir1.DryBulbTemperature()) - ((1.006 + (1.846 * mAir2.AbsoluteHumidity())) * mAir2.DryBulbTemperature()));
	}
		
	static double PSA(double xx);
	static double TS(double xx);

	void Serialize(CArchive& ar);

private:
	double m_fAbsoluteHumid;
	double m_fRelativeHumid;
	double m_fDryBulbTemp;
	double m_fWetBulbTemp;
	double m_fEnthalpy;

	static double fhair(double tdb, double w);
	static double fhc(double waterTemp);
	static double fhsat(double tsat, double patm);
	static double fndbhw(double enthalpy, double wetBulbTemp, double atm);
	static double fndbrh(double relativeHumid, double enthalpy, double atm);
	static double fndbrw(double relativeHumid, double wetBulbTemp, double atm);
	static double fphi(double tdb, double w, double patm);
	static double fpws(double tdb);
	static double ftdew(double w, double patm);
	static double ftdb(double w, double ha);
	static double ftpws(double pws);
	static double ftsat(double hs, double patm);
	static double ftwb(double tdb, double w, double patm);
	static double fwha(double tdb, double ha);
	static double fwphi(double tdb, double phi, double patm);
	static double fpww(double w, double patm);
	static double fwpw(double pw, double patm);
	static double fwtwb(double tdb, double twb, double patm);
	static double fwwbdb(double absoluteHumid, double wetBulbTemp, double atm);

};

