#include "stdafx.h"

#include "Medium.h"
#include "Water.h"

CWater::CWater()
{
}


CWater::~CWater()
{
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
