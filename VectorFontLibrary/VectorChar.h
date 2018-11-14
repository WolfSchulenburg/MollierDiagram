#pragma once

#include <array>

typedef struct stroke_t {
	double x1, y1;
	double x2, y2;
} Stroke;
const size_t stroke_s = sizeof(struct stroke_t);

class CVectorChar : public CObject
{
private:
	int m_iASCII;
	float m_fWidth;
	int m_iOverShot, m_iUnderShot;

	CArray<Stroke, Stroke> m_arrStrokes;

public:
	CVectorChar(int iASCII, float fWidth, int iOverShot, int iUnderShot);
	~CVectorChar();

	int GetASCIICode() { return m_iASCII; }

	void AddStroke(float x1, float y1, float x2, float y2);
	int StrokeCount();

	// void DrawString(const CDC *pDC);
};

