
// FeuchteLuftDoc.h: Schnittstelle der Klasse CFeuchteLuftDoc
//
#pragma once

#include "StatePoint.h"
#include "AirHandlingUnit.h"

class CFeuchteLuftDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CFeuchteLuftDoc();
	DECLARE_DYNCREATE(CFeuchteLuftDoc)

// Attribute
public:
	CList<CStatePoint*> listStatePoints;

private:
	CAirHandlingUnit *pAHU;

// Vorgänge
public:
	void AddComponent(CComponent* pComponent);
	
	CComponent* CreateHeater();
	CComponent* CreateCooler();
	CComponent* CreateMixer();
	CComponent* CreateHumidifier();
	CComponent* CreateDehumidifier();
	CComponent* CreateRecuperator();
	CComponent* CreateRegenerator();

// Überschreibungen
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementierung
public:
	virtual ~CFeuchteLuftDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Hilfsfunktion, die Suchinhalt für Suchhandler festlegt
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnStatePoint();
	afx_msg void OnUpdateStatePoint(CCmdUI *pCmdUI);
	afx_msg void OnProcessUnit();
	afx_msg void OnUpdateProcessUnit(CCmdUI *pCmdUI);
	afx_msg void OnProcessComponent(UINT nID);
	afx_msg void OnUpdateProcessComponent(CCmdUI *pCmdUI);
};
