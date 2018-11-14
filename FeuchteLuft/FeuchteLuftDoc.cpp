
// FeuchteLuftDoc.cpp: Implementierung der Klasse CFeuchteLuftDoc
//

#include "stdafx.h"

// SHARED_HANDLERS k�nnen in einem ATL-Projekt definiert werden, in dem Vorschau-, Miniaturansichts- 
// und Suchfilterhandler implementiert werden, und die Freigabe von Dokumentcode f�r das Projekt wird erm�glicht.
#ifndef SHARED_HANDLERS
#include "FeuchteLuft.h"
#endif

#include "StatePointDlg.h"
#include "FeuchteLuftDoc.h"

#include "FeuchteLuftRegistry.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFeuchteLuftDoc

IMPLEMENT_DYNCREATE(CFeuchteLuftDoc, CDocument)

BEGIN_MESSAGE_MAP(CFeuchteLuftDoc, CDocument)
	ON_COMMAND(ID_PROCESS_AHU, &CFeuchteLuftDoc::OnProcessUnit)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_AHU, &CFeuchteLuftDoc::OnUpdateProcessUnit)
	ON_COMMAND(ID_PROCESS_POINT, &CFeuchteLuftDoc::OnStatePoint)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_POINT, &CFeuchteLuftDoc::OnUpdateStatePoint)
	ON_COMMAND_RANGE(ID_PROCESS_HEATER, ID_PROCESS_REGENERATION, &CFeuchteLuftDoc::OnProcessComponent)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PROCESS_HEATER, ID_PROCESS_REGENERATION, &CFeuchteLuftDoc::OnUpdateProcessComponent)
END_MESSAGE_MAP()

// CFeuchteLuftDoc-Erstellung/Zerst�rung

CFeuchteLuftDoc::CFeuchteLuftDoc()
{
	// TODO: Hier Code f�r One-Time-Konstruktion einf�gen
}

CFeuchteLuftDoc::~CFeuchteLuftDoc()
{
	//if (pAHU)
	//	delete pAHU;
}

BOOL CFeuchteLuftDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: Hier Code zur Reinitialisierung einf�gen
	// (SDI-Dokumente verwenden dieses Dokument)
	if (!pAHU)
		pAHU = new CAirHandlingUnit();

	return TRUE;
}

// CFeuchteLuftDoc-Serialisierung

void CFeuchteLuftDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		listStatePoints.Serialize(ar);
		pAHU->Serialize(ar);
	}
	else
	{
		listStatePoints.Serialize(ar); 
		pAHU->Serialize(ar);
	}
}

#ifdef SHARED_HANDLERS

// Unterst�tzung f�r Miniaturansichten
void CFeuchteLuftDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �ndern Sie diesen Code, um die Dokumentdaten zu zeichnen.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Unterst�tzung f�r Suchhandler
void CFeuchteLuftDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Suchinhalte aus Dokumentdaten festlegen. 
	// Die Inhaltsteile sollten durch ";" getrennt werden.

	// Beispiel:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CFeuchteLuftDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFeuchteLuftDoc-Diagnose

#ifdef _DEBUG
void CFeuchteLuftDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFeuchteLuftDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFeuchteLuftDoc-Befehle


void CFeuchteLuftDoc::OnProcessUnit()
{
	// TODO: F�gen Sie hier Ihren Befehlshandlercode ein.
}


void CFeuchteLuftDoc::OnUpdateProcessUnit(CCmdUI *pCmdUI)
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	pCmdUI->Enable(false);
}


void CFeuchteLuftDoc::OnStatePoint()
{
	// TODO: F�gen Sie hier Ihren Befehlshandlercode ein.
	CStatePointDlg dlg;
	dlg.DoModal();
}


void CFeuchteLuftDoc::OnUpdateStatePoint(CCmdUI *pCmdUI)
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	pCmdUI->Enable(true);
}


void CFeuchteLuftDoc::OnProcessComponent(UINT nID)
{
	// TODO: F�gen Sie hier Ihren Befehlshandlercode ein.
}


void CFeuchteLuftDoc::OnUpdateProcessComponent(CCmdUI *pCmdUI)
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}


// CFeuchteLuftDoc-Implementierung


void CFeuchteLuftDoc::AddComponent(CComponent* pComponent)
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}

CComponent* CFeuchteLuftDoc::CreateHeater()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}

CComponent* CFeuchteLuftDoc::CreateCooler()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}

CComponent* CFeuchteLuftDoc::CreateMixer()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}

CComponent* CFeuchteLuftDoc::CreateHumidifier()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}

CComponent* CFeuchteLuftDoc::CreateDehumidifier()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}

CComponent* CFeuchteLuftDoc::CreateRecuperator()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}

CComponent* CFeuchteLuftDoc::CreateRegenerator()
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
	return nullptr;
}
