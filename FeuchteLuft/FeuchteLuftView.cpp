
// FeuchteLuftView.cpp: Implementierung der Klasse CFeuchteLuftView
//

#include "stdafx.h"
#include <math.h>

// SHARED_HANDLERS k�nnen in einem ATL-Projekt definiert werden, in dem Vorschau-, Miniaturansichts- 
// und Suchfilterhandler implementiert werden, und die Freigabe von Dokumentcode f�r das Projekt wird erm�glicht.
#ifndef SHARED_HANDLERS
#include "FeuchteLuft.h"
#endif

#include "Color.h"

#include "MainFrm.h"
#include "FeuchteLuftDoc.h"
#include "FeuchteLuftView.h"

#include "ConfigurationDlg.h"
#include "LayerDlg.h"

#include "MoistAir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFeuchteLuftView

IMPLEMENT_DYNCREATE(CFeuchteLuftView, CView)

BEGIN_MESSAGE_MAP(CFeuchteLuftView, CView)
	// Standarddruckbefehle
	//{{AFX_MSG_MAP(CFeuchteLuftView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFeuchteLuftView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	// Update status bar indicators
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_X, &CFeuchteLuftView::OnUpdateIndicatorX)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_Y, &CFeuchteLuftView::OnUpdateIndicatorY)

	ON_COMMAND(ID_EXTRAS_UMGEBUNG, &CFeuchteLuftView::OnExtrasEnvironment)
	ON_UPDATE_COMMAND_UI(ID_EXTRAS_UMGEBUNG, &CFeuchteLuftView::OnUpdateExtrasEnvironment)
	ON_COMMAND(ID_EXTRAS_LAYER, &CFeuchteLuftView::OnExtrasLayer)
	ON_UPDATE_COMMAND_UI(ID_EXTRAS_LAYER, &CFeuchteLuftView::OnUpdateExtrasLayer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CFeuchteLuftView-Erstellung/Zerst�rung

CFeuchteLuftView::CFeuchteLuftView()
{
	// lineProperties[0].DrawingPen = new CPen(1, 1, RGB(10, 10, 10));
	CMainFrame *pMainWnd = (CMainFrame *) AfxGetMainWnd();
	
	pDiagram = new CPsychometricChart();
	CString sFontName = theApp.GetMainDir() + _T("iso.fnt");

	pDiagram->LoadFont(sFontName);
	pDiagram->SelectFont(0);
}

CFeuchteLuftView::~CFeuchteLuftView()
{
	delete pDiagram;
}

BOOL CFeuchteLuftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ndern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

// CFeuchteLuftView-Zeichnung

void CFeuchteLuftView::SetTemperatureRange(double min, double max)
{
	cfgData.tmin = min;
	cfgData.tmax = max;
}

void CFeuchteLuftView::SetSaturationRange(double min, double max)
{
	cfgData.xmin = min;
	cfgData.xmax = max;
}

void CFeuchteLuftView::OnDraw(CDC* pDC)
{
	CWaitCursor waitCursor;
	// CMemDC memDC(pDC);
	CContext context((CView*)this, pDC);

	CFeuchteLuftDoc *pDoc = (CFeuchteLuftDoc *) GetDocument();
	ASSERT_VALID(pDoc);
	
	pDiagram->Draw(&context);
	
	// Draw all defined state points
	POSITION pos = pDoc->listStatePoints.GetHeadPosition();
	for (int i = 0; i < pDoc->listStatePoints.GetCount(); i++)
	{
		CStatePoint* pStatePoint = pDoc->listStatePoints.GetNext(pos);
		if(pStatePoint != nullptr)
			pDiagram->DrawStatePoint(&context, pStatePoint->GetTemperature(), pStatePoint->GetAbsHumidity());
	}
}


// CFeuchteLuftView-Diagnose

#ifdef _DEBUG
void CFeuchteLuftView::AssertValid() const
{
	CView::AssertValid();
}

void CFeuchteLuftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFeuchteLuftDoc* CFeuchteLuftView::GetDocument() const // Nichtdebugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFeuchteLuftDoc)));
	return (CFeuchteLuftDoc*)m_pDocument;
}
#endif //_DEBUG


// CFeuchteLuftView drucken

void CFeuchteLuftView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CFeuchteLuftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CFeuchteLuftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Zus�tzliche Initialisierung vor dem Drucken hier einf�gen
}

void CFeuchteLuftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Bereinigung nach dem Drucken einf�gen
}


void CFeuchteLuftView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: F�gen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.
}


// CFeuchteLuftView-Meldungshandler

void CFeuchteLuftView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_ptLast = point;

	CView::OnMouseMove(nFlags, point);
}


BOOL CFeuchteLuftView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: F�gen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CFeuchteLuftView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: F�gen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.

	CView::OnLButtonDown(nFlags, point);
}


void CFeuchteLuftView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: F�gen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.

	CView::OnLButtonUp(nFlags, point);
}


void CFeuchteLuftView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}


void CFeuchteLuftView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CFeuchteLuftView::OnUpdateIndicatorX(CCmdUI* pCmdUI)
{
	CString s;
	s.Format(_T("x=%d"), m_ptLast.x);
	pCmdUI->SetText(s);

	pCmdUI->SetCheck(m_ptLast.x > 200);

	pCmdUI->Enable(TRUE);
}

void CFeuchteLuftView::OnUpdateIndicatorY(CCmdUI* pCmdUI)
{
	CString s;
	s.Format(_T("y=%d"), m_ptLast.y);
	pCmdUI->SetText(s);

	pCmdUI->SetCheck(m_ptLast.y > 200);

	pCmdUI->Enable(TRUE);
}

void CFeuchteLuftView::OnExtrasEnvironment()
{
	// TODO: F�gen Sie hier Ihren Befehlshandlercode ein.
	CConfigurationDlg dlg;
	dlg.DoModal();
}


void CFeuchteLuftView::OnUpdateExtrasEnvironment(CCmdUI *pCmdUI)
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}


void CFeuchteLuftView::OnExtrasLayer()
{
	// TODO: F�gen Sie hier Ihren Befehlshandlercode ein.
	CLayerDlg dlg;
	dlg.DoModal();
}


void CFeuchteLuftView::OnUpdateExtrasLayer(CCmdUI *pCmdUI)
{
	// TODO: F�gen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}
