#include "stdafx.h"

#include "Medium.h"
#include "MoistAir.h"

IMPLEMENT_SERIAL(CMoistAir, CMedium, 0);

CMoistAir::CMoistAir()
{
	// set constants for a  standard air condition
	m_fDryBulbTemp = 26.0;
	m_fRelativeHumid = 60.0;

	m_fAbsoluteHumid = fwphi(m_fDryBulbTemp, m_fRelativeHumid, CMedium::ATM);
	m_fWetBulbTemp = ftwb(m_fDryBulbTemp, m_fAbsoluteHumid, CMedium::ATM);
	m_fEnthalpy = fhair(m_fDryBulbTemp, m_fAbsoluteHumid);
}

CMoistAir::CMoistAir(double dryBulbTemp, double absoluteHumidity)
{
	dryBulbTemp = dryBulbTemp;
	m_fAbsoluteHumid = absoluteHumidity;
	m_fRelativeHumid = fphi(dryBulbTemp, m_fAbsoluteHumid, CMedium::ATM);
	m_fEnthalpy = fhair(dryBulbTemp, m_fAbsoluteHumid);
	m_fWetBulbTemp = ftwb(dryBulbTemp, m_fAbsoluteHumid, CMedium::ATM);
}

CMoistAir::~CMoistAir()
{
}

void CMoistAir::Serialize(CArchive & ar)
{
	CMedium::Serialize(ar);
	if (ar.IsStoring())
		ar << m_fAbsoluteHumid << m_fRelativeHumid << m_fDryBulbTemp << m_fWetBulbTemp << m_fEnthalpy;
	else
		ar >> m_fAbsoluteHumid >> m_fRelativeHumid >> m_fDryBulbTemp >> m_fWetBulbTemp >> m_fEnthalpy;
}

double CMoistAir::PSA(double xx)
{
	if ((0.01 <= xx) && (xx <= 100)) {
		// Verdampfungskurve
		return (611 * exp(-0.000191275 + 0.07258 * xx - 0.0002939 * pow(xx, 2) + 0.0000009841 * pow(xx, 3) - 0.00000000192 * pow(xx, 4)));
	}
	else if ((-20 <= xx) && (xx < 0.01)) {
		// Sublimationskurve
		return (611 * exp(-0.0004909965 + 0.08183197 * xx - 0.0005552967 * pow(xx, 2) - 0.00002228376 * pow(xx, 3) - 0.0000006211808 * pow(xx, 4)));
	}
	return 0;
}


double CMoistAir::TS(double xx)
{
	double Y = log(xx);

	if ((611 <= xx) && (xx <= 101320)) {
		// Verdampfungskurve
		return (-63.16113 + 5.36859 * Y + 0.973587 * pow(Y, 2) - 0.0738636 * pow(Y, 3) + 0.00481832 * pow(Y, 4));
	}
	else if ((103 <= xx) && (xx < 611)) {
		// Sublimationskurve
		return (-61.125785 + 8.1386 * Y - 0.07422003 * pow(Y, 2) + 0.06283721 * pow(Y, 3) - 0.0027237063 * pow(Y, 4));
	}
	return 0;
}

/*
'Berechnung aller Zustandsgrößen
pS = PSA(t)
If pS > p Then ooo = "Dampfdruck > Luftdruck, t kleiner wählen!" : GoTo LabelRechnen
pW = x * p / (0.6222 + x)
KK = (t + 273.15) / p
If pS - pW >= -0.00001 Then
xW = x
phi = pW / pS
Else
xS = 0.6222 * pS / (p - pS)
xW = xS
ptau = pW
pW = pS
phi = 2
End If
pL = p - pW
v = (xW / 0.6222 + 1) * RL * KK
rho = (1 + x) / ((xW + 0.6222) * RW * KK)
h = 1.01 * t + xW * (2501 + 1.86 * t)
If phi <= 1 Then
If pW >= 100 Then
ttau = TS(pW)
tf = Int(ttau - 2)
Else
tf = -22
ttau = -100
End If
Do
tf = tf + 1
hfr = hf
hSr = hS
pSstern = PSA(tf)
xS = 0.6222 * pSstern / (p - pSstern)
hf = h + (xS - x) * 4.19 * tf
hS = 1.01 * tf + xS * (2501 + 1.86 * tf)
Loop While hS <= hf
KK = (hfr - hSr) / (hS - hf)
tf = (tf * (1 + KK) - 1) / (1 + KK)
Else
If t > 0 Then
h = h + (x - xS) * 4.19 * t
Else
h = h + (x - xS) * (-334 + 2.09 * t)
phi = 3
End If
ttau = TS(ptau)
tf = t
End If
'Wärmekapazität
cLL = 1.0065 + 0.000005309587 * t + 0.0000004758596 * t ^ 2 - 0.0000000001136145 * t ^ 3
cWL = 1.863 + 0.0002680862 * t + 0.0000006794704 * t ^ 2 - 0.0000000002641422 * t ^ 3
cp = (cLL + x * cWL) / (1 + x)
'Wärmeleitfähigkeit
lamL = 0.024178 + 0.00007634878 * t - 0.00000004663859 * t ^ 2 + 0.00000000004612639 * t ^ 3
lamW = 0.016976 + 0.000057535 * t + 0.0000001277125 * t ^ 2 - 0.00000000008951228 * t ^ 3
PHIW = x / (0.6222 + x)
Lambda = (1 - PHIW) * lamL + PHIW * lamW
'Dynamische Viskosität
etaL = 0.0000172436 + 0.0000000504587 * t - 0.00000000003923361 * t ^ 2 + 0.00000000000004046118 * t ^ 3
etaW = 0.0000091435 + 0.0000000281979 * t + 0.00000000004486993 * t ^ 2 - 0.00000000000004928814 * t ^ 3
PHIW = x / (0.6222 + x)
Eta = (1 - PHIW) * etaL + PHIW * etaW
'Kinematische Viskosität
If x = 0 Then
NyL = 0.0000135198 + 0.00000008930841 * t + 0.0000000001094808 * t ^ 2 - 0.00000000000003659345 * t ^ 3
Ny = NyL * 100000 / p
Else
Ny = Eta / rho
End If
'Temperaturleitfähigkeit 
If x = 0 Then
aL = 0.0000188328 + 0.0000001286753 * t + 0.0000000001680101 * t ^ 2 - 0.0000000000001240072 * t ^ 3
a = aL * 100000 / p
Else
a = Lambda / (cp * 1000) / rho
End If
'Prandtlzahl 
If x = 0 Then
PrL = 0.71789 - 0.0001675739 * t + 0.0000006514142 * t ^ 2 - 0.0000000006687762 * t ^ 3
Pr = PrL
Else
Pr = Eta * (cp * 1000) / Lambda
End If
*/


double CMoistAir::fhair(double tdb, double w)
{
	return ((1.006 * tdb) + (w * ((1.846 * tdb) + 2501.0)));
}

double CMoistAir::fhc(double waterTemp)
{
	if (waterTemp < 0.0)
	{
		return (-333.6 + (2.093 * waterTemp));
	}
	return (4.187 * waterTemp);
}

double CMoistAir::fhsat(double tsat, double patm)
{
	double w = fwpw(fpws(tsat), patm);
	return fhair(tsat, w);
}

double CMoistAir::fndbhw(double enthalpy, double wetBulbTemp, double atm)
{
	int num = 0;
	double tdb = wetBulbTemp;
	do
	{
		double num3 = tdb - fwwbdb(fwha(tdb, enthalpy), wetBulbTemp, atm);
		if (fabs(num3) < 1E-09)
		{
			return tdb;
		}
		double num4 = (tdb + 1E-10) - fwwbdb(fwha(tdb + 1E-10, enthalpy), wetBulbTemp, atm);
		tdb -= num3 / ((num4 - num3) / 1E-10);
		num++;
	} while (20 >= num);
	throw("Iteration Error");
}

double CMoistAir::fndbrh(double relativeHumid, double enthalpy, double atm)
{
	int num = 0;
	double tdb = 25.0;
	do
	{
		double num3 = enthalpy - fhair(tdb, fwphi(tdb, relativeHumid, atm));
		if (fabs(num3) < 0.001)
		{
			return tdb;
		}
		double num4 = enthalpy - fhair(tdb + 1E-10, fwphi(tdb + 1E-10, relativeHumid, atm));
		tdb -= num3 / ((num4 - num3) / 1E-10);
		num++;
	} while (20 >= num);
	throw("Iteration Error");
}

double CMoistAir::fndbrw(double relativeHumid, double wetBulbTemp, double atm)
{
	int num = 0;
	double tdb = wetBulbTemp;
	do
	{
		double num3 = tdb - fwwbdb(fwphi(tdb, relativeHumid, atm), wetBulbTemp, atm);
		if (fabs(num3) < 1E-09)
		{
			return tdb;
		}
		double num4 = (tdb + 1E-10) - fwwbdb(fwphi(tdb + 1E-10, relativeHumid, atm), wetBulbTemp, atm);
		tdb -= num3 / ((num4 - num3) / 1E-10);
		num++;
	} while (20 >= num);
	throw("Iteration Error");
}

double CMoistAir::fphi(double tdb, double w, double patm)
{
	double num = fpww(w, patm);
	double num2 = fpws(tdb);
	if (num2 == 0.0)
	{
		return 0.0;
	}
	return ((100.0 * num) / num2);
}

/// <summary>To obtain the saturated water vapor partial pressure [kPa] from the dry-bulb temperature [℃]</summary>
/// <param name="tdb">Dry-bulb temperature[°C]</param>
/// <returns>Saturated water vapor partial pressure [kPa]</returns>
/// <remarks>The formula by Wexler-Hyland</remarks>
double CMoistAir::fpws(double tdb)
{
	// Approximate range confirmation
	if (tdb < -100 || 200 < tdb)
	{
		throw 1;
		// throw new InputValueOutOfRangeException(rm.GetString("MoistAir_DrybulbTemperature_Error1"), 200, -100, tdb);
	}
	double td = tdb + TCONV;
	//-100~0C // Triple point
	if (tdb < 0.01)
	{
		const double c[] = { -5.6745359e3, 6.3925247, -9.6778430e-3, 6.2215701e-7, 2.0747825e-9, -9.4840240e-13, 4.1635019 };
		//const double c1 = -5.6745359e3;
		//const double c2 = 6.3925247;
		//const double c3 = -9.6778430e-3;
		//const double c4 = 6.2215701e-7;
		//const double c5 = 2.0747825e-9;
		//const double c6 = -9.4840240e-13;
		//const double c7 = 4.1635019;
		return exp(c[0] / td + c[1] + c[2] * td + c[3] * pow(td, 2) + c[4] * pow( td, 3) + c[5] * pow(td, 4) + c[6] * log(td)) / 1000;
		// return Math.Exp(c1 / td + c2 + c3 * td + c4 * Math.Pow(td, 2) + c5 * Math.Pow(td, 3) + c6 * Math.Pow(td, 4) + c7 * Math.Log(td)) / 1000.0d;
	}
	//0~200C
	else
	{
		const double c[] = { -5.8002206e3, 1.3914993, -4.8640239e-2, 4.1764768e-5, -1.4452093e-8, 6.5459673 };
		//const double c8 = -5.8002206e3;
		//const double c9 = 1.3914993;
		//const double c10 = -4.8640239e-2;
		//const double c11 = 4.1764768e-5;
		//const double c12 = -1.4452093e-8;
		//const double c13 = 6.5459673;
		return exp(c[0] / td + c[1] + c[2] * td + c[3] * pow(td, 2) + c[4] * pow(td, 3) + c[5] * log(td)) / 1000;
		//return Math.Exp(c8 / td + c9 + c10 * td + c11 * td * td + c12 * td * td * td + c13 * Math.Log(td)) / 1000.0d;
	}
}

/// <summary>To find the dew point temperature [℃] from absolute humidity [kg / kg] and atmospheric pressure [kPa]</summary>
/// <param name="w">Absolute humidity [kg/kg]</param>
/// <param name="patm">Atmospheric pressure [kPa]: 1 atm 101.325 [kPa]</param>
/// <returns>Dew-point temperature [°C]</returns>
double CMoistAir::ftdew(double w, double patm)
{
	const double c[] = { 6.54, 14.526, 0.7389, 0.09486, 0.4569 };
	//const double c0 = 6.54;
	//const double c1 = 14.526;
	//const double c2 = 0.7389;
	//const double c3 = 0.09486;
	//const double c4 = 0.4569;

	double ps = fpww(w, patm);
	if (ps < 0.000001)
	{
		return 0.0;
	}
	else
	{
		double alpha = log(ps);
		if (0.611213 < ps)
		{
			return c[0] + alpha * (c[1] + alpha * (c[2] + alpha * c[3])) + c[4] * pow(ps, 0.1984);
		}
		else
		{
			return 6.09 + alpha * (12.608 + alpha * 0.4959);
		}
	}
}

double CMoistAir::ftdb(double w, double ha)
{
	return ((ha - (2501.0 * w)) / (1.006 + (1.846 * w)));
}

double CMoistAir::ftpws(double pws)
{
	int num = 0;
	double tdb = 25.0;
	do
	{
		double num3 = fpws(tdb) - pws;
		if (fabs(num3) < 1E-09)
		{
			return tdb;
		}
		double num4 = fpws(tdb + 1E-10) - pws;
		tdb -= num3 / ((num4 - num3) / 1E-10);
		num++;
	} while (20 >= num);
	throw("Iteration Error");
}

double CMoistAir::ftsat(double hs, double patm)
{
	double tsat = -6.0055 + (hs * (0.6851 + (hs * (-0.0056978 + (hs * (3.5344E-05 + (hs * (-1.2891E-07 + (hs * 2.0165E-10)))))))));
	double num3 = fhsat(tsat, patm) - hs;
	double num4 = 0.0;
	if (fabs(num3) >= 0.001)
	{
		num4 = tsat - 5.0;
		for (int i = 0; i < 50; i++)
		{
			double num6 = fhsat(num4, patm);
			double num7 = num6 - hs;
			if (fabs(num7) > 0.001)
			{
				double num8 = 0.0;
				if (fabs((double)(num7 - num3)) > 1E-05)
				{
					num8 = tsat - ((num3 * (num4 - tsat)) / (num7 - num3));
				}
				tsat = num4;
				double num2 = num6;
				num3 = num7;
				num4 = num8;
			}
		}
	}
	return num4;
}

double CMoistAir::ftwb(double tdb, double w, double patm)
{
	double hs = fhair(tdb, w);
	double num2 = ftsat(hs, patm) - 5.0;
	double num4 = fwpw(fpws(num2), patm);
	double num5 = fhair(num2, num4) - ((4.187 * num2) * (num4 - w));
	double num6 = num5 - hs;
	double num7 = num2 + 5.0;
	for (int i = 0; i < 50; i++)
	{
		double num10 = fwpw(fpws(num7), patm);
		double num11 = fhair(num7, num10) - ((4.187 * num7) * (num10 - w));
		double num12 = num11 - hs;
		if (fabs(num12) > 0.001)
		{
			double num13;
			if (fabs((double)(num12 - num6)) > 1E-30)
			{
				num13 = num2 - ((num6 * (num7 - num2)) / (num12 - num6));
			}
			else
			{
				num13 = num2;
			}
			num2 = num7;
			num5 = num11;
			num6 = num12;
			num7 = num13;
		}
	}
	return num7;
}

double CMoistAir::fwha(double tdb, double ha)
{
	return ((ha - (1.006 * tdb)) / ((1.846 * tdb) + 2501.0));
}

double CMoistAir::fwphi(double tdb, double phi, double patm)
{
	double num = fpws(tdb);
	double pw = (0.01 * phi) * num;
	return fwpw(pw, patm);
}

/// <summary>Wasserdampfpartialdruck[kPa] aus absoluter Feuchte[kg / kg] und atmosphärischem Druck[kPa] berechnen</summary>
/// <param name="w">Absolute humidity [kg/kg]</param>
/// <param name="patm">Atmospheric pressure [kPa]: 1 atm 101.325 [kPa]</param>
/// <returns>Wasserdampf Partialdruck [kPa]</returns>
double CMoistAir::fpww(double w, double patm)
{
	return patm * w / (0.621945 + w);
}

/// <summary>Obtain absolute humidity [kg / kg] from water vapor partial pressure [kPa] and atmospheric pressure [kPa]</summary>
/// <param name="pw">Absolute humidity [kg/kg]</param>
/// <param name="patm">Atmospheric pressure [kPa]: 1 atm 101.325 [kPa]</param>
/// <returns>Wasserdampf Partialdruck [kPa]</returns>
double CMoistAir::fwpw(double pw, double patm)
{
	return 0.621945 * pw / (patm - pw);
}

double CMoistAir::fwtwb(double tdb, double twb, double patm)
{
	return (((fwpw(fpws(twb), patm) * ((2501.0 + (1.846 * twb)) - fhc(twb))) - (1.006 * (tdb - twb))) / ((2501.0 + (1.846 * tdb)) - fhc(twb)));
}

double CMoistAir::fwwbdb(double absoluteHumid, double wetBulbTemp, double atm)
{
	double num2 = fwpw(fpws(wetBulbTemp), atm);
	return ((((1.006 * wetBulbTemp) + ((((1.846 * wetBulbTemp) + 2501.0) - fhc(wetBulbTemp)) * num2)) - ((2501.0 - fhc(wetBulbTemp)) * absoluteHumid)) / (1.006 + (1.846 * absoluteHumid)));
}
