#include "stdafx.h"

#include "Medium.h"
#include "MoistAir.h"

#define TCONV 273.15

CMoistAir::CMoistAir()
{
}

CMoistAir::~CMoistAir()
{
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
