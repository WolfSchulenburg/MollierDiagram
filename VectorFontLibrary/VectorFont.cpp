#include "stdafx.h"
#include "VectorChar.h"
#include "VectorFont.h"

CVectorFont::CVectorFont()
{

}


CVectorFont::~CVectorFont()
{
	POSITION pos(listChars.GetStartPosition());
	CObject* pObject = NULL;
	WORD iKey;

	while (pos) {
		listChars.GetNextAssoc(pos, iKey, pObject);
		ASSERT_VALID(pObject);

		delete pObject;
		pObject = NULL;
	}

	listChars.RemoveAll();
}

void CVectorFont::AddVectorChar(CVectorChar *pVectorChar)
{
	int iCode = pVectorChar->GetASCIICode();
	listChars.SetAt(iCode, pVectorChar);
}


void CVectorFont::LoadFont(CString aFileName)
{
	CStdioFile fontFile;
	CString strLine;
	CVectorChar *pVectorChar;
	int ident = 0, stage = 0, error = 0;

	CFileException e;
	if (!fontFile.Open(aFileName, CFile::modeRead, &e))
	{
		if (e.m_cause == CFileException::fileNotFound)
			AfxMessageBox(_T("VectorFont : Font-File not found !"));
	}
	else {
		TRACE("VectorFont: loading and parsing font file.");
		while (fontFile.ReadString(strLine))
		{
			try {
				int j = 0;
				switch (strLine.GetAt(0)) {
					case '#': break;		// remark
					case 's': {
						if (ident == 0) {
							strLine.Delete(0, 2);
							m_FontInfo.m_sName = strLine;
							ident++;
							break;
						}
						if (ident == 1) {
							strLine.Delete(0, 2);
							m_FontInfo.m_sDescription = strLine;
							ident = 0;
						}
						break;
					}
					case 'z': {
						if (stage == 0) {	// header
							strLine.Delete(0, 2);
							switch (ident) {
							case 0: 
								m_FontInfo.m_iHeight = _ttoi(strLine);
								break;
							case 1: 
								m_FontInfo.m_fScale = _ttof(strLine);
								break;
							case 2:
								m_FontInfo.m_fSmallCharHeight = _ttof(strLine);
								break;
							case 3:
								m_FontInfo.m_fOverShot = _ttof(strLine);
								break;
							case 4:
								m_FontInfo.m_fUnderShot = _ttof(strLine);
								break;
							case 5:
								m_FontInfo.m_fDistChar = _ttof(strLine);
								break;
							case 6:
								m_FontInfo.m_fDistWord = _ttof(strLine);
								break;
							case 7:
								m_FontInfo.m_fDistLines = _ttof(strLine);
								break;
							case 8:
								m_FontInfo.m_fDistLinesOverUnderShot = _ttof(strLine);
								break;
							case 9:
								m_FontInfo.m_fCharWidth = _ttof(strLine);
								break;
							case 10:
								j = swscanf_s(strLine, L"%f%f",
									&m_FontInfo.m_fMinSlant, &m_FontInfo.m_fMaxSlant);
								if (j < 2) error++;
								break;
							case 11:
								j = swscanf_s(strLine, L"%d%d%d%d",
									&m_FontInfo.clipBox.left, &m_FontInfo.clipBox.top, 
									&m_FontInfo.clipBox.right, &m_FontInfo.clipBox.bottom);
								if (j < 4) error++;
								stage++;	// now we are leaving header description
								break;
							}
							ident++;
						}
						else {				// char description
							float fWidth;
							int iASCII, iOverShot, iUnderShot;
							strLine.Delete(0, 2);
							j = swscanf_s(strLine, L"%d%f%d%d",
								&iASCII, &fWidth, &iOverShot, &iUnderShot);
							pVectorChar = new CVectorChar(iASCII, fWidth, iOverShot, iUnderShot);
						}
						break;
					}
					case 'g': {
						float x1, y1, x2, y2;
						strLine.Delete(0, 2);
						j = swscanf_s(strLine.GetBuffer(), L"%f%f%f%f", &x1, &y1, &x2, &y2);
						if(pVectorChar != nullptr)
							pVectorChar->AddStroke(x1, y2, x2, y2);
						break;			// stroke
					}
					case '.': {				// end of char
						TRACE("VectorFont: Add new char.");
						this->AddVectorChar(pVectorChar);
						break;
					}
				}
			}
			catch (CFileException *exp)
			{
				TCHAR szCause[255];
				exp->GetErrorMessage(szCause, 255);
				AfxMessageBox(szCause);
			}
		}
		fontFile.Close();
	}
}

void SetTransformMatrix(double e0, double e1, double e2, double e3)
{

}

void CVectorFont::DrawString(const CDC *pDC, const CString aString)
{
	CRect clientRect;
	pDC->GetClipBox(clientRect);

}