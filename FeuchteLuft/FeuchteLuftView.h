
// FeuchteLuftView.h: Schnittstelle der Klasse CFeuchteLuftView
//

#pragma once

#include "WorldPoint.h"

#include "Context.h"
#include "StatePoint.h"
#include "PsychometricChart.h"

typedef struct {
	double barometricPressure = 101.325;
	double hmax = 20.0, hmin = 0.0;
	double xmin = 0.0, xmax = 20.0;
	double tmin = -20.0, tmax = 40.0;
} tConfigurationData;

class CFeuchteLuftView : public CView
{
protected: // Nur aus Serialisierung erstellen
	CFeuchteLuftView();
	DECLARE_DYNCREATE(CFeuchteLuftView)

// Attribute
public:
	CFeuchteLuftDoc* GetDocument() const;

private:
	tConfigurationData cfgData;
	CPsychometricChart *pDiagram;
	CPoint m_ptLast;
	// CList<CLineProperty> linesProperties;
	//CMap<int, int, CLineProperty, CLineProperty> lineProperties;
	
// Vorgänge
public:
	void SetTemperatureRange(double min, double max);
	void SetSaturationRange(double min, double max);

// Überschreibungen
public:
	virtual void OnDraw(CDC* pDC);  // Überschrieben, um diese Ansicht darzustellen
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementierung
public:
	virtual ~CFeuchteLuftView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generierte Funktionen für die Meldungstabellen
protected:
	//{{AFX_MSG(CFeuchteLuftView)
	afx_msg void OnFilePrintPreview();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	// Update status bar indicators
	afx_msg void OnUpdateIndicatorX(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorY(CCmdUI* pCmdUI);

	afx_msg void OnExtrasEnvironment();
	afx_msg void OnUpdateExtrasEnvironment(CCmdUI *pCmdUI);
	afx_msg void OnExtrasLayer();
	afx_msg void OnUpdateExtrasLayer(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Debugversion in FeuchteLuftView.cpp
inline CFeuchteLuftDoc* CFeuchteLuftView::GetDocument() const
   { return reinterpret_cast<CFeuchteLuftDoc*>(m_pDocument); }
#endif

