#include "stdafx.h"

#include <math.h>

#include "Medium.h"


#define NELEMS(x) (sizeof(x) / sizeof(double)

const double CMedium::fGasConstant = 8.314; // univ. gas constant in J/K/mol
const double CMedium::fAbsoluteTemperature = 273.15;

#ifdef USE_HORNER
double CMedium::Horner(const double *a, int n, double x)
{
	double result = 0.0;
	for (int i = n - 1; i >= 0; --i)
		result = result * x + a[i];
	return result;
}
#else
#include <gsl/gsl_poly.h>
#endif

CMedium::CMedium() {
	_molarmass = 18.01528;  // molar mass water in g/mol
	_pressure = 101300.0;   // ambient pressure in Pa
}

CMedium::CMedium(const CMedium& orig) {
}

CMedium::~CMedium() {
}

void CMedium::setTemperature(double aTemp)
{
	_temperature = aTemp;
}

void CMedium::setPressure(double aPressure)
{
	this->_pressure = aPressure;
}

void CMedium::mixTemperature(CMedium& medium)
{
	this->_temperature = (this->_temperature + medium.getTemperature()) * 0.5;
}

double CMedium::getAbsTemp()
{
	return fAbsoluteTemperature + _temperature;
}

