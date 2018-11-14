
// FeuchteLuft.h: Hauptheaderdatei für die FeuchteLuft-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole

// CFeuchteLuftApp:
// Siehe FeuchteLuft.cpp für die Implementierung dieser Klasse
//

class CFeuchteLuftApp : public CWinAppEx
{
public:
	CFeuchteLuftApp();
	~CFeuchteLuftApp();

	static CString GetVersion();
	static CString GetName();
	static CString GetMainDir();		// Gets the installed path location of the directory that contains the currently executing copy of FeuchteLuft

private:
	void ReadRegistry();

// Überschreibungen
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementierung
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnCanCalc();
	DECLARE_MESSAGE_MAP()
};

extern CFeuchteLuftApp theApp;
