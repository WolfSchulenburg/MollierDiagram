#include "stdafx.h"

#include "Medium.h"
#include "Water.h"

IMPLEMENT_SERIAL(CWater, CMedium, 0);

CWater::CWater()
{
}


CWater::~CWater()
{
}

void CWater::Serialize(CArchive & ar)
{
	CMedium::Serialize(ar);
	// 
}

/*
 *  calculate thermal heat of water in kJ/kgK
 *  range 30 - 200 °C
 *  source Bernd Glueck
*/
double CWater::getThermalHeat()
{
	const double coeff[4] = { 4.177375,-2.144614E-6,-3.165823E-7,4.134309E-8 };

#ifdef USE_HORNER
	return Horner(coeff, 4, _temperature);
#else
	return gsl_poly_eval(coeff, 4, _temperature);
#endif
}

/*
 *  calculate kinematic viscosity of water Pa * s
 *  range 20 - 200 °C
 *  source Bernd Glueck
*/
double CWater::getViscosity()
{
	const double coeff[4] = { 554.7781,19.726936,0.1282308,-2.686914E-4 };

#ifdef USE_HORNER
	return 1 / Horner(coeff, 4, _temperature);
#else
	return 1 / gsl_poly_eval(coeff, 4, _temperature);
#endif
}

/*
 *  calculate density of water or steam in kg/cbm
 *  range 20 - 200 °C
 *  source Bernd Glueck
*/
double CWater::getDensity()
{
	if (isSteam())
		return _pressure * _molarmass / 8314.0 / (_temperature + 273.15);
	/*
	 *  simpler version error approx. 0,15 %
		FloatType coeff[3] = {1006.0,-0.26,-0.0022};
		return Horner(coeff, 3, fTemperature);
	*/

	/* approx. 0.08 % mean error */
	const double coeff[4] = { 1.002045E3,-1.029905E-1,-3.698162E-3,3.991053E-6 };

#ifdef USE_HORNER
	return Horner(coeff, 4, _temperature);
#else
	return gsl_poly_eval(coeff, 4, _temperature);
#endif

}


double CWater::Cps(double t)
{
	double num = t + 273.15;
	if ((num < 300.0) || (num > 3500.0))
	{
		throw("FUNCTION WaterProperty.Cps: T OUT OF RANGE");
	}
	double x = num / 100.0;
	return ((((143.05 + (-183.54 * pow(x, 0.25))) + (82.751 * pow(x, 0.5))) + (-3.6989 * x)) / 18.015);
}

double CWater::Cvs(double v, double t)
{
	double d = ((1.8 * t) + 32.0) + 459.67;
	double x = (v - 0.0063101) / 0.062428;
	return ((((0.99204818 + (-33.137211 / sqrt(d))) + (416.29663 / d)) - (((((0.185053 * pow(5.475, 2.0)) * d) / pow(1165.11, 2.0)) * exp((-5.475 * d) / 1165.11)) * ((-2590.5815 / x) + (113.95968 / pow(x, 2.0))))) * 4.1868);
}

double CWater::Hfg(double tc)
{
	if (tc < 0.0)
	{
		tc = 0.0;
	}
	double x = ((647.3 - tc) - 273.15) / 647.3;
	if (x < 0.0)
	{
		return 0.0;
	}
	double num2 = ((0.779221 * pow(x, 0.33333333333333331)) + (4.62668 * pow(x, 0.83333333333333337))) + (-1.07931 * pow(x, 0.875));
	num2 += x * (-3.87446 + (x * (2.94553 + (x * (-8.06395 + (x * (11.5633 + (x * -6.02884))))))));
	return (num2 * 2500.9);
}

double CWater::Hs(double pkpa, double tc)
{
	double d = pkpa * 0.001;
	double num2 = tc + 273.15;
	double num3 = 42.6776 + (-3892.7 / (log(d) + -9.48654));
	if (d >= 12.33)
	{
		num3 = -387.592 + (-12587.5 / (log(d) + -15.2578));
	}
	double num4 = 2041.21 + (d * (-40.4002 + (d * -0.48095)));
	double num5 = 1.610693 + (d * (0.05472051 + (d * 0.0007517537)));
	double num6 = 0.0003383117 + (d * (-1.975736E-05 + (d * -2.87409E-07)));
	double num7 = 1707.82 + (num3 * (-16.99419 + (num3 * (0.062746295 + (num3 * (-0.00010284259 + (num3 * 6.4561298E-08)))))));
	return ((num4 + (num2 * (num5 + (num2 * num6)))) - (num7 * exp((num3 - num2) / 45.0)));
}

double CWater::Hsats(double tc)
{
	double x = ((647.3 - tc) - 273.15) / 647.3;
	double num2 = ((1.0 + (0.457874342 * pow(x, 0.33333333333333331))) + (5.08441288 * pow(x, 0.83333333333333337))) + (-1.48513244 * pow(x, 0.875));
	num2 += x * (-4.81351884 + (x * (2.69411792 + (x * (-7.39064542 + (x * (10.4961689 + (x * -5.46840036))))))));
	return (num2 * 2099.3);
}

double CWater::Hsatw(double tc)
{
	double num3;
	double num = tc + 273.15;
	double x = (647.3 - num) / 647.3;
	if (num < 300.0)
	{
		num3 = x * (624.698837 + (x * (-2343.85369 + (x * (-9508.12101 + (x * (71628.7928 + (x * (-163535.221 + (x * (166531.093 + (x * -64785.4585))))))))))));
	}
	else if (num < 600.0)
	{
		num3 = (x * (-2.67172935 + (x * (6.22640035 + (x * (-13.1789573 + (x * (-1.91322436 + (x * (68.793763 + (x * (-124.819906 + (x * 72.1435404))))))))))))) + 0.8839230108;
	}
	else
	{
		num3 = (((1.0 + (-0.441057805 * pow(x, 0.33333333333333331))) + (-5.52255517 * pow(x, 0.83333333333333337))) + (6.43994847 * pow(x, 0.875))) + (x * (-1.64578795 + (x * -1.30574143)));
	}
	return (num3 * 2099.3);
}

double CWater::Psats(double tc)
{
	double num = tc + 273.15;
	double d = (10.4592 + (num * (-0.0040489 + (num * (-4.1752E-05 + (num * (3.6851E-07 + (num * (-1.0152E-09 + (num * (8.6531E-13 + (num * (9.03668E-16 + (num * (-1.9969E-18 + (num * (7.79287E-22 + (num * 1.91482E-25)))))))))))))))))) + (-3968.06 / (num - 39.5735));
	return (1000.0 * exp(d));
}

double CWater::Ss(double pkpa, double tc)
{
	double d = pkpa * 0.001;
	double num2 = tc + 273.15;
	double num3 = 42.6776 + (-3892.7 / (log(d) + -9.48654));
	if (d >= 12.33)
	{
		num3 = -387.592 + (-12587.5 / (log(d) + -15.2578));
	}
	return (((4.6162961 + (num2 * (0.01039008 + (num2 * (-9.873085E-06 + (num2 * (5.4311E-09 + (num2 * -1.170465E-12)))))))) + (-0.4650306 * log(0.001 + (d * 10.0)))) - (exp((num3 - num2) / 85.0) * (1.777804 + (num3 * (-0.01802468 + (num3 * (6.854459E-05 + (num3 * (-1.184434E-07 + (num3 * 8.142201E-11))))))))));
}

double CWater::Ssats(double tc)
{
	double x = ((647.3 - tc) - 273.15) / 647.3;
	double num2 = ((1.0 + (0.377391 * pow(x, 0.33333333333333331))) + (-2.78368 * pow(x, 0.83333333333333337))) + (6.93135 * pow(x, 0.875));
	num2 += x * (-4.34839 + (x * (1.34672 + (x * (1.75261 + (x * (-6.22295 + (x * 9.99004))))))));
	return (num2 * 4.4289);
}

double CWater::Ssatw(double tc)
{
	double num3;
	double num = tc + 273.15;
	double x = (647.3 - num) / 647.3;
	if (num < 300.0)
	{
		num3 = x * (-1836.92956 + (x * (14706.6352 + (x * (-43146.6046 + (x * (48606.6733 + (x * (7997.5096 + (x * (-58333.9887 + (x * 33140.0718))))))))))));
	}
	else if (num < 600.0)
	{
		num3 = (x * (-1.75702956 + (x * (1.68754095 + (x * (5.82215341 + (x * (-63.3354786 + (x * (188.076546 + (x * (-252.344531 + (x * 128.058531))))))))))))) + 0.912762917;
	}
	else
	{
		num3 = (((1.0 + (-0.32481765 * pow(x, 0.33333333333333331))) + (-2.990556709 * pow(x, 0.83333333333333337))) + (3.23419 * pow(x, 0.875))) + (x * (-0.678067859 + (x * -1.91910364)));
	}
	return (num3 * 4.4289);
}

double CWater::Steamk(double t)
{
	double x = (t * 1.8) + 32.0;
	double num2 = ((0.824272 + (0.00254627 * x)) + (9.848539E-08 * pow(x, 2.0))) * 0.01;
	return (num2 * 0.0017308);
}

double CWater::Tpss(double p, double s)
{
	double tc = -230.4724 + (-3892.7 / (log(p * 0.001) + -9.48654));
	if (p >= 12330.0)
	{
		tc = -660.742 + (-12587.5 / (log(p * 0.001) + -15.2578));
	}
	double num2 = Ssats(tc);
	if (num2 >= s)
	{
		return tc;
	}
	double num3 = ((tc + 273.15) * (1.0 + ((s - num2) / Cps(tc)))) - 273.15;
	double num4 = Ss(p, num3);
	double num5 = 0.0;
	for (int i = 0; i < 10; i++)
	{
		num5 = num3 + (((tc - num3) * (s - num4)) / (num2 - num4));
		if (fabs((double)(num5 - num3)) < 0.05)
		{
			return num5;
		}
		tc = num3;
		num2 = num4;
		num3 = num5;
		num4 = Ss(p, num3);
		if (i == 9)
		{
			throw("FUNCTION WaterProperty. Tpss FAILS TO CONVERGE");
		}
	}
	return num5;
}

double CWater::Tsats(double pkpa)
{
	double d = pkpa * 0.001;
	if (d < 12.33)
	{
		return (-230.4724 + (-3892.7 / (log(d) + -9.48654)));
	}
	return (-660.742 + (-12587.5 / (log(d) + -15.2578)));
}

double CWater::Vissph(double t)
{
	double x = (t * 1.8) + 32.0;
	double num2 = ((0.0183161 + (5.7067E-05 * x)) + (-1.42253E-08 * pow(x, 2.0))) + (7.241555E-12 * pow(x, 3.0));
	return (num2 * 0.00041338);
}

double CWater::Vissv(double p)
{
	double x = p / 6.894757;
	double num2 = ((0.0314 + (2.9675E-05 * x)) + (-1.60583E-08 * pow(x, 2.0))) + (3.768986E-12 * pow(x, 3.0));
	return (num2 * 0.00041338);
}

double CWater::Vs(double pkpa, double tc)
{
	double d = pkpa * 0.001;
	double num2 = tc + 273.15;
	double num3 = 42.6776 + (-3892.7 / (log(d) + -9.48654));
	if (d >= 12.33)
	{
		num3 = -387.592 + (-12587.5 / (log(d) + -15.2578));
	}
	return ((((0.000461631 * num2) / d) - (0.0527993 * exp(-0.00375928 * num2))) + (((0.022 - exp(-3.741378 + (num3 * (-0.0047838281 + (num3 * 1.5923434E-05))))) / (10.0 * d)) * exp((num3 - num2) / 40.0)));
}

double CWater::Vsats(double pkpa, double tc)
{
	double x = ((647.3 - tc) - 23.15) / 647.3;
	double num2 = ((1.0 + (1.6351057 * pow(x, 0.33333333333333331))) + (52.584599 * pow(x, 0.83333333333333337))) + (-44.694653 * pow(x, 0.875));
	num2 += x * (-8.9751114 + (x * (-0.4384553 + (x * (-19.179576 + (x * (36.765319 + (x * -19.462437))))))));
	return (((num2 * 22.089) * 0.003155) / (pkpa * 0.001));
}

double CWater::Vsatw(double tc)
{
	double x = ((647.3 - tc) - 273.15) / 647.3;
	double num2 = ((1.0 + (-1.9153882 * pow(x, 0.33333333333333331))) + (12.015186 * pow(x, 0.83333333333333337))) + (-7.8464025 * pow(x, 0.875));
	num2 += x * (-3.8886414 + (x * (2.0582238 + (x * (-2.0829991 + (x * (0.82180004 + (x * 0.47549742))))))));
	return (num2 * 0.003155);
}

double CWater::Wcp(double tw)
{
	if (tw > 100.0)
	{
		return (2.9735 + (tw * (0.023049 + (tw * (-0.00013953 + (tw * 3.092474E-07))))));
	}
	return (4.21534 + (tw * (-0.00287819 + (tw * (7.4729E-05 + (tw * (-7.79624E-07 + (tw * 3.220424E-09))))))));
}

double CWater::Wk(double tw)
{
	return (0.001 * (0.560101 + (tw * (0.00211703 + (tw * (-1.05172E-05 + (tw * (1.497323E-08 + (tw * -1.48553E-11)))))))));
}

double CWater::Wmu(double tw)
{
	double num;
	if (tw < 20.0)
	{
		num = pow(10.0, -3.30233 + (1301.0 / (998.333 + ((tw - 20.0) * (8.1855 + (0.00585 * (tw - 20.0))))))) * 100.0;
	}
	else if (tw > 100.0)
	{
		num = 0.68714 + (tw * (-0.0059231 + (tw * (2.1249E-05 + (tw * -2.69575E-08)))));
	}
	else
	{
		num = 1.002 * pow(10.0, ((tw - 20.0) * (-1.3272 + ((tw - 20.0) * -0.001053))) / (tw + 105.0));
	}
	return (0.001 * num);
}

double CWater::Wrho(double tw)
{
	return ((999.83952 + (tw * (16.945176 + (tw * (-0.0079870401 + (tw * (-4.6170461E-05 + (tw * (1.0556302E-07 + (tw * -2.8054253E-10)))))))))) / (1.0 + (0.01687985 * tw)));
}
