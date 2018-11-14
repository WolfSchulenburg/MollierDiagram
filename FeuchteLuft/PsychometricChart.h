#pragma once
#include <afx.h>

#include "Color.h"
#include "Context.h"
#include "VectorFont.h"

#include "MoistAir.h"

enum AxisOrientation { axisHorizontal, axisVertical };

typedef enum tLineType
{
	SaturatedLine = 1,
	WetBulbTemperatureLine = 2,
	EnthalpyLine = 4,
	AbsoluteHumidityLine = 8,
	DryBulbTemperatureLine = 16,
	RelativeHumidityLine = 32,
	SpecificVolumeLine = 64,
	All = SaturatedLine | WetBulbTemperatureLine | EnthalpyLine | AbsoluteHumidityLine | DryBulbTemperatureLine | RelativeHumidityLine | SpecificVolumeLine
} ELineType;


class CLineProperty : public CObject
{
public:
	CLineProperty(double minValue, double maxValue, double spacing, CPen *pen, bool drawLine);
	~CLineProperty();

	bool GetDrawLine() { return m_bDrawLine; }
	
	double GetMinimumValue() { return m_fMinimumValue; }
	double GetMaximumValue() { return m_fMaximumValue; }
	
	CPen* GetPen() { return m_pDrawingPen; }

private:
	double m_fMinimumValue;
	double m_fMaximumValue;
	double m_fSpacing;
	CPen *m_pDrawingPen;
	bool m_bDrawLine;
};


class CDiagramAxis : public CObject
{
private:
	int length, ticks;
	CPoint startPt, endPt;
	AxisOrientation axisOrientation;
	double minValue, maxValue, valueRange;
	CString sTitle;

public:
	CDiagramAxis(AxisOrientation aOrientation);
	~CDiagramAxis();

	void setTitle(CString aTitle) { sTitle = aTitle; }

	void SetPosition(int sx, int sy, int ex, int ey);

	void SetRange(double minValue, double maxValue);
	void SetTicks(int ticks);

	void Draw(CContext* pContext);
};


class CPsychometricChart : public CObject
{
private:
	CMapWordToOb lineProperties;
	CDiagramAxis xAxis, yAxis;
	CVectorFont m_rVectorFont;

	double pressureAir;
	
	int height, width;

	double minDB;
	double maxDB;

	double minAH;
	double maxAH;

	double yRate;
	double xRate;

	int iMargin;

	double xmin, xmax, ymin, ymax;
	double xScale, yScale;				// Skalierung der Achsen - Pixel/Lämge
	double xDelta, yDelta;				// Divisions in float

	int xDiv, yDiv;						// x/y Teilung der Achse

	void UpdateValues();

	CLineProperty* GetLineProperty(ELineType aLineType);

	void drawLine(CContext* pContext, int pen, double sx, double sy, double ex, double ey);

	CWorldPoint getPointFromDBandAH(double dbTemp, double absoluteHumidity, double xRate, double yRate);

	void drawSaturatedLine(CContext* pContext, double xRate, double yRate);
	void drawDryBulbTemperatureLine(CContext* pContext, double xRate, double yRate);
	void drawAbsoluteHumidityLine(CContext* pContext, double xRate, double yRate);
	void drawEnthalpyLine(CContext* pContext, double xRate, double yRate);
	void drawWetBulbTemperatureLine(CContext* pContext, double xRate, double yRate);
	void drawSpecificVolumeLine(CContext* pContext, double xRate, double yRate);
	void drawRelativeHumidityLine(CContext* pContext, double xRate, double yRate);

	void drawAxis(CContext* pContext, double xRate, double yRate);

public:
	CPsychometricChart();
	~CPsychometricChart();

	void LoadFont(CString aFilename);
	void SelectFont(int i);

	void SetPressure(double aPressure);

	void SetAbsHumidityRange(double minValue, double maxValue);
	void SetTemperatureRange(double minValue, double maxValue);

	void SetDivisionsHorizontal(int value);
	void SetDivisionsVertical(int value);

	void SetMargin(int aMargin) { iMargin = aMargin; }

	void DrawStatePoint(CContext* pContext, double x, double y);
	void Draw(CContext* pContext);
};

