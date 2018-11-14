
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Registerkartenfenster erstellen:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Fehler beim Erstellen des Ausgaberegisterkarten-Fensters.\n");
		return -1;      // Fehler beim Erstellen
	}

	// Ausgabebereiche erstellen:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputCalculation.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputProcess.Create(dwStyle, rectDummy, &m_wndTabs, 3))
	{
		TRACE0("Fehler beim Erstellen der Ausgabefenster.\n");
		return -1;      // Fehler beim Erstellen
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// Listenfenster an Registerkarte anfügen:
	bNameValid = strTabName.LoadString(IDS_CALCULATION_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputCalculation, strTabName, (UINT)0);

	bNameValid = strTabName.LoadString(IDS_PROCESS_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputProcess, strTabName, (UINT)1);
	
	// Platzhaltertext in Ausgaberegisterkarten einfügen (Platzhalter)
	FillBuildWindow();
	FillDebugWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Das Registerkartensteuerelement sollte den gesamten Clientbereich abdecken:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
	m_wndOutputCalculation.AddString(_T("Ok."));
}

void COutputWnd::FillDebugWindow()
{
	m_wndOutputProcess.AddString(_T("Ok."));
}

void COutputWnd::LogCalculationWindow(CString aString)
{
	m_wndOutputCalculation.AddString(aString);
}

void COutputWnd::LogProcessWindow(CString aString)
{
	m_wndOutputProcess.AddString(aString);
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputCalculation.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputProcess.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList-Meldungshandler

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("Ausgabe kopieren"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("Ausgabe löschen"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
