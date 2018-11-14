#pragma once

#define USE_HORNER

//class AFX_EXT_CLASS COutOfRangeException : public CException;

class AFX_EXT_CLASS CMedium : public CObject
{
	DECLARE_SERIAL(CMedium);

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

	void Serialize(CArchive& ar);

#ifdef USE_HORNER
protected:
	static double Horner(const double *a, int n, double x);
#endif 

protected:
	int	_version;

	double _temperature;
	double _molarmass;
	double _pressure;

	const static double fGasConstant;
	const static double fAbsoluteTemperature;

	const static double ATM;
	const static double CP_AIR;
	const static double CP_VAPOR;
	const static double CP_WATER;
	const static double HFG;
	const static double TCONV;

};

