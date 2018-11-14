#pragma once
#include <afx.h>
#include "VectorChar.h"

typedef struct fontinfo_t {
	// Fontname - Kuerzel(<= 6 Zeichen)
	CString m_sName;
	// Fontbeschreibung
	CString m_sDescription; 
	// Fonthoehe h eines Grossbuchstaben
	int m_iHeight;
	// FontSkalierung
	float m_fScale;
	// Hoehe Kleinbuchstaben bezogen auf h
	float m_fSmallCharHeight;
	// Ueberstand Oberlaenge bezogen auf h
	float m_fUnderShot;
	// Ueberstand Unterlaenge bezogen auf h
	float m_fOverShot;
	// Abstand zweier Zeichen(Faktor bezogen auf h)
	float m_fDistChar;
	// Abstand zweier Woerter(Faktor bezogen auf h)
	float m_fDistWord;
	// Abstand zweier Grundlinien(Faktor bezogen auf h)
	float m_fDistLines;
	// Abstand zweier Grundlinien bei Ober - / Unterlaengen(Faktor bezogen auf h)
	float m_fDistLinesOverUnderShot;
	// Faktor Zeichenbreite
	float m_fCharWidth;
	// Bereichsangabe Textneigung(von bis) im Bereich - 89 Grad bis + 89 Grad
	float m_fMinSlant, m_fMaxSlant;
	// ClipBox
	CRect clipBox;
} FontInfo;

class AFX_EXT_API CVectorFont : public CObject
{
private:
	FontInfo m_FontInfo;
	CMapWordToOb listChars;

	void AddVectorChar(CVectorChar* pVectorChar);

public:
	CVectorFont();
	~CVectorFont();

	void LoadFont(CString aFileName);
	
	void SetTransformMatrix(double e0, double e1, double e2, double e3);
	void DrawString(const CDC *pDC, const CString aString);
};

