#pragma once

#define USE_HORNER

class CMedium
{
public:
	CMedium();
	CMedium(const CMedium& orig);
	virtual ~CMedium();

	void setTemperature(double aTemp);  // set Temperature in °C
	double getTemperature() { return this->_temperature; }

	void setPressure(double aPressure);
	double getPressure() { return this->_pressure; }

	void mixTemperature(CMedium& medium);

	double getAbsTemp();

	bool isSteam() { return false; }

#ifdef USE_HORNER
protected:
	static double Horner(const double *a, int n, double x);
#endif 

protected:
	double _temperature;
	double _molarmass;
	double _pressure;

	const static double fGasConstant;
	const static double fAbsoluteTemperature;
};

