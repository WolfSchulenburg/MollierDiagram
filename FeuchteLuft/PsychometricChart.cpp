#include "stdafx.h"

#include "MoistAir.h"
#include "WorldPoint.h"
#include "PsychometricChart.h"

#include <limits>

int int_cast(double x)
{
	return (int)(x * (1.0 + std::numeric_limits<double>::epsilon()));
}

// ======================================================================================

CLineProperty::CLineProperty(double minValue, double maxValue, double spacing, CPen *pen, bool drawLine)
{
	this->m_fMinimumValue = minValue;
	this->m_fMaximumValue = maxValue;
	this->m_fSpacing = spacing;
	this->m_pDrawingPen = pen;
	this->m_bDrawLine = drawLine;
}


CLineProperty::~CLineProperty()
{
	if(m_pDrawingPen != nullptr)
		delete m_pDrawingPen;
}

// ======================================================================================

CDiagramAxis::CDiagramAxis(AxisOrientation aOrientation)
{
	this->axisOrientation = aOrientation;

	if (axisOrientation == axisHorizontal)
		sTitle = _T("X-Axis");
	else
		sTitle = _T("Y-Axis");
}


CDiagramAxis::~CDiagramAxis()
{
}


void CDiagramAxis::SetPosition(int sx, int sy, int ex, int ey)
{
	startPt.x = sx, startPt.y = sy;
	endPt.x = ex, endPt.y = ey;
}


void CDiagramAxis::SetRange(double minValue, double maxValue)
{
	this->minValue = min(maxValue, minValue);
	this->maxValue = max(maxValue, minValue);
	this->valueRange = maxValue - minValue;
}


void CDiagramAxis::SetTicks(int ticks)
{
	this->ticks = ticks;
}


void CDiagramAxis::Draw(CContext* pContext)
{
	double xLL, yHH;

	if (axisOrientation == axisHorizontal) {
		length = endPt.x - startPt.x;
		xLL = length / (maxValue - minValue);     // Pixel/Länge
	}
	else {
		length = endPt.y - startPt.y;
		yHH = length / (maxValue - minValue);    // Pixel/Länge
	}

	pContext->MoveTo(startPt.x, startPt.y);
	pContext->LineTo(endPt.x, endPt.y);

	int deltaTicks = int_cast(length / ticks);
	double deltaValueTicks = fabs(maxValue - minValue) / ticks;

	CString strScale;
	for (int i = 0; i <= ticks; i++)
	{
		strScale.Format(_T("%.1f"), int(10 * (valueRange) / 10.0f));

		double delta = (maxValue - minValue) / ticks;
		if (axisOrientation == axisHorizontal)
		{
			int x = int_cast(delta * i * xLL);
			pContext->MoveTo(x, startPt.y);
			pContext->LineTo(x, startPt.y + 10);
		}
		else
		{
			int y = int_cast(delta * i * yHH);
			pContext->MoveTo(startPt.x, y);
			pContext->LineTo(startPt.x - 10, y);
		}
	}

	if (axisOrientation == axisHorizontal)
	{
		CRect rectLabel;
		rectLabel.left = startPt.x;
		rectLabel.right = endPt.x;
		rectLabel.top = startPt.y;
		rectLabel.bottom = startPt.y + 25;

		pContext->HorCenterText(rectLabel, sTitle);
	}
	else
	{
		pContext->VertCenterText(startPt.x - 20, (startPt.y - endPt.y) / 2, sTitle);
	}

}

// ======================================================================================

CPsychometricChart::CPsychometricChart()
	: xAxis(AxisOrientation::axisHorizontal), yAxis(AxisOrientation::axisVertical)
{
	iMargin = 80;

	xmin = 0, xmax = 20;		// x 0 - 20 g/kg
	ymin = -10, ymax = 20;		// t -12 - 20 °C

	xDiv = 10;
	yDiv = 10;

	pressureAir = 101.3250;		// p atm in Pa 101.325 kPa / 1000 -> 101325 Pa

	lineProperties.SetAt((WORD) SaturatedLine, new CLineProperty(-10.0, 50.0, 1.0, new CPen(PS_SOLID, 1, RGB(127,127,127)), true));
	lineProperties.SetAt((WORD) WetBulbTemperatureLine, new CLineProperty(-5.0, 35.0, 1.0, new CPen(PS_SOLID, 1, RGB(127, 127, 127)), true));
	lineProperties.SetAt((WORD) EnthalpyLine, new CLineProperty(0.0, 125.0, 5.0, new CPen(PS_SOLID, 1, RGB(127, 127, 127)), true));
	lineProperties.SetAt((WORD) AbsoluteHumidityLine, new CLineProperty(0.0, 0.037, 0.001, new CPen(PS_SOLID, 1, RGB(127, 127, 127)), true));
	lineProperties.SetAt((WORD) DryBulbTemperatureLine, new CLineProperty(-10.0, 50.0, 1.0, new CPen(PS_SOLID, 1, RGB(127, 127, 127)), true));
	lineProperties.SetAt((WORD) RelativeHumidityLine, new CLineProperty(5.0, 95.0, 5.0, new CPen(PS_SOLID, 1, RGB(127, 127, 127)), true));
	lineProperties.SetAt((WORD) SpecificVolumeLine, new CLineProperty(0.75, 0.96, 0.01, new CPen(PS_SOLID, 1, RGB(127, 127, 127)), true));

	//double hmax = MoistAir.GetAirStateFromDBHR(50, 0, MoistAir.Property.Enthalpy);
	//double hmin = MoistAir.GetAirStateFromDBHR(-10, 0, MoistAir.Property.Enthalpy);
	//ax = Math.Tan(58d / 180d * Math.PI) * (hmax - hmin) / 0.037;
}


CPsychometricChart::~CPsychometricChart()
{
	POSITION pos(lineProperties.GetStartPosition());
	CObject* pObject = NULL;
	WORD iKey;

	while (pos) {
		lineProperties.GetNextAssoc(pos, iKey, pObject);
		ASSERT_VALID(pObject);

		delete pObject;
		pObject = NULL;
	}

	lineProperties.RemoveAll();
}


void CPsychometricChart::LoadFont(CString aFilename)
{
	m_rVectorFont.LoadFont(aFilename);
}


void CPsychometricChart::SelectFont(int i)
{

}


void CPsychometricChart::SetDivisionsHorizontal(int value)
{
	xDiv = value;
}


void CPsychometricChart::SetDivisionsVertical(int value)
{
	yDiv = value;
}


void CPsychometricChart::SetPressure(double aPressure)
{
	if ((aPressure > 80) && (aPressure < 200))
		pressureAir = aPressure;
	else
		pressureAir = 101.325;
}


void CPsychometricChart::SetAbsHumidityRange(double minValue, double maxValue)
{
	if (minValue < 0)
		xmin = 0.0;
	else
		xmin = minValue;
	xmax = maxValue;
}


void CPsychometricChart::SetTemperatureRange(double minValue, double maxValue)
{
	ymin = minValue;
	ymax = maxValue;
}

void CPsychometricChart::UpdateValues()
{
	CLineProperty *pLineProperty = (CLineProperty*)lineProperties[DryBulbTemperatureLine];
	minDB = pLineProperty->GetMinimumValue();
	maxDB = pLineProperty->GetMaximumValue();

	pLineProperty = (CLineProperty*)lineProperties[AbsoluteHumidityLine];
	minAH = pLineProperty->GetMinimumValue();
	maxAH = pLineProperty->GetMaximumValue();

	yRate = height / (maxAH - minAH);
	xRate = width / (maxDB - minDB);
}

CLineProperty* CPsychometricChart::GetLineProperty(ELineType aLineType)
{
	return (CLineProperty*) lineProperties[aLineType];
}

void CPsychometricChart::Draw(CContext* pContext)
{
	int pen = 1;
	double tmpX, tmpY;
	double x, y;
	
	height = pContext->Height() - 2 * iMargin;
	width = pContext->Width() - 2 * iMargin;

	xScale = width / (xmax - xmin);     // Pixel/Länge
	yScale = height / (ymax - ymin);    // Pixel/Länge
	yDelta = (ymax - ymin) / 5 / yDiv;
	xDelta = (xmax - xmin) / 5 / xDiv;

	xAxis.SetPosition(iMargin, pContext->Height() - iMargin, pContext->Width() - 2 * iMargin, pContext->Height() - iMargin);
	xAxis.SetRange(xmin, xmax);
	xAxis.SetTicks(xDiv);
	xAxis.setTitle(_T("absolute Feuchtigkeit in g/kg"));

	yAxis.SetPosition(iMargin, pContext->Height() - iMargin, iMargin, iMargin);
	yAxis.SetRange(ymin, ymax);
	yAxis.SetTicks(yDiv);
	yAxis.setTitle(_T("Temperatur in °C"));

	// xAxis.Draw(pContext);
	// yAxis.Draw(pContext);

	drawLine(pContext, pen, 0, 0, xmax, 0);
	drawLine(pContext, pen, 0, 0, 0, 1.01 * ymax - ymin);

	UpdateValues();

	CLineProperty *pLineProperty = (CLineProperty*)lineProperties[SaturatedLine];
	if (pLineProperty->GetDrawLine())
		drawSaturatedLine(pContext, xDelta, yDelta);

	pLineProperty = (CLineProperty*)lineProperties[DryBulbTemperatureLine];
	if (pLineProperty->GetDrawLine())
		drawDryBulbTemperatureLine(pContext, xDelta, yDelta);

	pLineProperty = (CLineProperty*)lineProperties[AbsoluteHumidityLine];
	if (pLineProperty->GetDrawLine())
		drawAbsoluteHumidityLine(pContext, xDelta, yDelta);

	pLineProperty = (CLineProperty*)lineProperties[EnthalpyLine];
	if (pLineProperty->GetDrawLine())
		drawEnthalpyLine(pContext, xDelta, yDelta);

	pLineProperty = (CLineProperty*)lineProperties[WetBulbTemperatureLine];
	if (pLineProperty->GetDrawLine())
		drawWetBulbTemperatureLine(pContext, xDelta, yDelta);

	pLineProperty = (CLineProperty*)lineProperties[SpecificVolumeLine];
	if (pLineProperty->GetDrawLine())
		drawSpecificVolumeLine(pContext, xDelta, yDelta);

	pLineProperty = (CLineProperty*)lineProperties[RelativeHumidityLine];
	if (pLineProperty->GetDrawLine())
		drawRelativeHumidityLine(pContext, xDelta, yDelta);

	// Gitter
	// y-Achsteilung (Enthalpie)
	for (int n = 1; n < (50 * yDiv); n++) {

		y = n * yDelta;
		if ((n % 5) == 0) {

			if ((n*yDelta) == -ymin)
				pen = 0;
			else
				pen = 1;

			tmpX = y / 2.501;
			tmpY = 0;
			if (tmpX > xmax) {
				tmpX = xmax;
				tmpY = y - ymax * 2.501;
			}

			if (n < (5 * yDiv)) {
				drawLine(pContext, pen, 0, y, tmpX, tmpY);
			}
			else if (tmpY < (ymax - ymin)) {
				double tmpXY = (y - ymax + ymin) / 2.501;
				drawLine(pContext, pen, tmpXY, ymax - ymin, tmpX, tmpY);
			}
		}
	}

	// x-Achsteilung (Wassergehalt)
	pen = 1;
	y = (ymax - ymin) / 100;		// tick length
	for (int n = 1; n <= (5 * xDiv); n++) {
		x = n * xDelta;
		if (n % 5 == 0) {
			drawLine(pContext, pen, x, 0, x, ymax - ymin);
		}
		else {
			drawLine(pContext, pen, x, 0, x, y);
		}
	}

	// Isothermen tt und Sättigungslinie Phi=1 (ymax und ymin stellen auch die Grenztemperaturen dar)

	double tt;
	double h0, h1, x1;
	double h1v = 0, x1v = 0;
	double p = pressureAir;
	double PSDiag;

	for (int n = 0; n <= (5 * yDiv); n++) {

		// Isotherme im ungesättigtem Gebiet
		pen = 2;
		tt = n * yDelta + ymin;
		PSDiag = CMoistAir::PSA(tt);
		x1 = 0.6222 / (p * 1000 / PSDiag - 1) * 1000;

		if ((p * 1000 / PSDiag - 1) < 0)
			throw(_T("Dampfdruck > Luftdruck, tmax kleiner!"));

		if (x1 > xmax)
			x1 = xmax;

		h0 = tt * 1.01;
		h1 = tt * 1.01 + x1 / 1000 * 1.86 * tt;

		if (n % 5 == 0)
			pen = 3;

		if ((tt > -0.1) && (tt < 0.1))
			pen = 4;

		drawLine(pContext, pen, 0, h0 - ymin, x1, h1 - ymin);

		// Sättigungslinie
		if ((n > 0) && (x1v < xmax)) {

			pen = 7; // 5

			if (x1 < xmax) {
				drawLine(pContext, pen, x1v, h1v - ymin, x1, h1 - ymin);
			}
			else
			{
				// Randkorrektur
				x1 = 0.6222 / (p * 1000 / PSDiag - 1) * 1000;
				h1 = tt * 1.01 + x1 / 1000 * 1.86 * tt;
				h1 = h1v + (h1 - h1v) * (xmax - x1v) / (x1 - x1v);
				drawLine(pContext, pen, x1v, h1v - ymin, xmax, h1 - ymin);
			}

		}

		h1v = h1;
		x1v = x1;

		// TODO : Probleme der Darstellung
		// Ausgewählte Isotherme im Flüssigkeitsnebel
		if ((x1 < xmax) && (tt >= 0) && ((n % 5) == 0)) {
			pen = 3;
			double hsaet = tt * 1.01 + x1 / 1000 * (2501 + 1.86 * tt);
			double dx = (hsaet - ymin) / 2.501 - x1;	// Näherung
			y = 0;
			tmpY = hsaet + dx / 1000 * 4.19 * tt;
			tmpX = (tmpY - ymin) / 2.5;
			// double xy0 = tmpX;
			if (tmpX > xmax)
				tmpX = xmax;
			drawLine(pContext, pen, x1, h1 - ymin, tmpX, y);
		}

		// TODO : Keine Linien sichtbar
		// Ausgewählte Isotherme im Eisnebel
		if ((x1 < xmax) && (tt == -10) || (tt == -5) || (tt = 0)) {
			pen = 3;
			double hsaet = tt * 1.01 + x1 / 1000 * (2501 + 1.86 * tt);
			double dx = (hsaet - ymin) / 2.501 - x1;		// Näherung
			y = 0;
			tmpY = hsaet + dx / 1000 * (-334 + 2.09 * tt);
			tmpX = (tmpY - ymin) / 2.5;
			drawLine(pContext, pen, x1, h1 - ymin, tmpX, y);
		}
	}

	// Linien Phi < 1 - relative Feuchten 
	for (int phiD = 1; phiD <= 9; phiD++) {

		if (phiD == 5)
			pen = 5;
		else
			pen = 6;

		for (int n = 0; n <= (5 * yDiv); n++) {

			tt = n * yDelta + ymin;
			PSDiag = CMoistAir::PSA(tt);
			x1 = 0.6222 / (p * 10000 / PSDiag / phiD - 1) * 1000;
			h1 = tt * 1.01 + x1 / 1000 * 1.86 * tt;

			if ((n > 0) && (x1v < xmax)) {
				if (x1 < xmax) {
					drawLine(pContext, pen, x1v, h1v - ymin, x1, h1 - ymin);
				}
				else {
					// Randkorrektur
					h1 = h1v + (h1 - h1v) * (xmax - x1v) / (x1 - x1v);
					drawLine(pContext, pen, x1v, h1v - ymin, xmax, h1 - ymin);
				}
			}

			h1v = h1;
			x1v = x1;
		}
	}
}

void CPsychometricChart::DrawStatePoint(CContext* pContext, double x, double y)
{

}

void CPsychometricChart::drawLine(CContext* pContext, int pen, double sx, double sy, double ex, double ey)
{
	int x0 = iMargin;
	int y0 = pContext->Height() - iMargin;

	int x1 = x0 + int_cast(sx * xScale);
	int y1 = y0 - int_cast(sy * yScale);
	int x2 = x0 + int_cast(ex * xScale);
	int y2 = y0 - int_cast(ey * yScale);

	pContext->SelectPen(pen);
	pContext->MoveTo(x1, y1);
	pContext->LineTo(x2, y2);
}


CWorldPoint CPsychometricChart::getPointFromDBandAH(double dbTemp, double absoluteHumidity, double xRate, double yRate)
{
	return CWorldPoint(10, 10);

//	double en = MoistAir::GetAirStateFromDBHR(dbTemp, absoluteHumidity, MoistAir.Property.Enthalpy);
//	double dbt = en - ax * absoluteHumidity;

	CLineProperty *pLineProperty = (CLineProperty*)lineProperties[DryBulbTemperatureLine];
	double minDB = pLineProperty->GetMinimumValue();
	pLineProperty = (CLineProperty*)lineProperties[AbsoluteHumidityLine];
	double minAH = pLineProperty->GetMinimumValue();

	// return new CWorldPoint((float)((dbt - minDB) * xRate), (float)((minAH - absoluteHumidity) * yRate));
}


void CPsychometricChart::drawSaturatedLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawDryBulbTemperatureLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawAbsoluteHumidityLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawEnthalpyLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawWetBulbTemperatureLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawSpecificVolumeLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawRelativeHumidityLine(CContext* pContext, double xRate, double yRate)
{
}

void CPsychometricChart::drawAxis(CContext* pContext, double xRate, double yRate)
{
}

