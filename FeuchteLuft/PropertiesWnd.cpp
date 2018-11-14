
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "FeuchteLuft.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar-Meldungshandler

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Kombinationsfeld erstellen:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Fehler beim Erstellen des Eigenschaftenkombinationsfelds. \n");
		return -1;      // Fehler beim Erstellen
	}

	m_wndObjectCombo.AddString(_T("Anwendung"));
	m_wndObjectCombo.AddString(_T("Eigenschaftenfenster"));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Fehler beim Erstellen des Eigenschaftenrasters. \n");
		return -1;      // Fehler beim Erstellen
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Ist gesperrt */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Gesperrt */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// Alle Befehle werden über dieses Steuerelement geleitet, nicht über den übergeordneten Rahmen:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Behandlungscode ein.
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Behandlungscode ein.
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Luftzustand 1"));

	CMFCPropertyGridProperty *pProp = new CMFCPropertyGridProperty(_T("Trockenkugel-Temp. [°C]"), (_variant_t)250l, _T("Trockenkugeltemperatur"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Feuchtkugel-Temp. [°C]"), (_variant_t)250l, _T("Feuchtkugeltemperatur"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Rel. Feuchtigkeit [%]"), (_variant_t)250l, _T("Rel. Feuchtigkeit"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Abs. Feuchtigkeit [g/kg]"), (_variant_t)250l, _T("Absolute Feuchtigkeit"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Enthalpie [kJ/kg]"), (_variant_t)250l, _T("Enthalpie"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Taupunkt [°C]"), (_variant_t)250l, _T("Taupunkt"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Spez. Volumen [m³/kg]"), (_variant_t)250l, _T("Spezifisches Volumen"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Luftzustand 2"));

	pProp = new CMFCPropertyGridProperty(_T("Trockenkugel-Temp. [°C]"), (_variant_t)250l, _T("Trockenkugeltemperatur"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Feuchtkugel-Temp. [°C]"), (_variant_t)250l, _T("Feuchtkugeltemperatur"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Rel. Feuchtigkeit [%]"), (_variant_t)250l, _T("Rel. Feuchtigkeit"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Abs. Feuchtigkeit [g/kg]"), (_variant_t)250l, _T("Absolute Feuchtigkeit"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Enthalpie [kJ/kg]"), (_variant_t)250l, _T("Enthalpie"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Taupunkt [°C]"), (_variant_t)250l, _T("Taupunkt"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Spez. Volumen [m³/kg]"), (_variant_t)250l, _T("Spezifisches Volumen"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Sonstiges"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Anwendung"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Fensterfarbe"), RGB(210, 192, 254), NULL, _T("Gibt die Standardfarbe des Fensters an."));
	pColorProp->EnableOtherButton(_T("Andere..."));
	pColorProp->EnableAutomaticButton(_T("Standard"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Symboldateien(*.ico)|*.ico|Alle Dateien(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Symbol"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Gibt das Fenstersymbol an.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Ordner"), _T("c:\\")));

	pGroup3->Expand(0);

	m_wndPropList.AddProperty(pGroup3);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}
