
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList-Fenster

class COutputList : public CListBox
{
// Konstruktion
public:
	COutputList();

// Implementierung
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// Konstruktion
public:
	COutputWnd();

	void UpdateFonts();

// Attribute
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputCalculation;
	COutputList m_wndOutputProcess;

protected:
	void FillBuildWindow();
	void FillDebugWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementierung
public:
	virtual ~COutputWnd();

	void LogCalculationWindow(CString aString);
	void LogProcessWindow(CString aString);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

