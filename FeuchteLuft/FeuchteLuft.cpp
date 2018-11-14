
// FeuchteLuft.cpp: Definiert das Klassenverhalten für die Anwendung.
//
#pragma comment(lib,"version.lib")

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "FeuchteLuft.h"
#include "MainFrm.h"

#include "ChildFrm.h"

#include "FeuchteLuftDoc.h"
#include "FeuchteLuftView.h"

#include "AboutDlg.h"

#include "FeuchteLuftRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFeuchteLuftRegistry *g_pRegistry = NULL;

// CFeuchteLuftApp

BEGIN_MESSAGE_MAP(CFeuchteLuftApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CFeuchteLuftApp::OnAppAbout)
	ON_COMMAND(ID_EXTRAS_CANCALC, &CFeuchteLuftApp::OnCanCalc)
	// Dateibasierte Standarddokumentbefehle
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standarddruckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

// CFeuchteLuftApp-Erstellung

CFeuchteLuftApp::CFeuchteLuftApp()
{
	m_bHiColorIcons = TRUE;

	// Neustart-Manager unterstützen
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

#ifdef _MANAGED
	// Wenn die Anwendung mit Common Language Runtime-Unterstützung (/clr) erstellt wurde:
	//     1) Diese zusätzliche Eigenschaft ist erforderlich, damit der Neustart-Manager ordnungsgemäß unterstützt wird.
	//     2) Für die Projekterstellung müssen Sie im Projekt einen Verweis auf System.Windows.Forms hinzufügen.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: Ersetzen Sie die Anwendungs-ID-Zeichenfolge unten durch eine eindeutige ID-Zeichenfolge; empfohlen
	// Das Format für die Zeichenfolge ist: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("IBS.FeuchteLuft.Mollier.10"));

	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

CFeuchteLuftApp::~CFeuchteLuftApp()
{
	delete g_pRegistry;
}

// Das einzige CFeuchteLuftApp-Objekt

CFeuchteLuftApp theApp;

// CFeuchteLuftApp-Initialisierung

BOOL CFeuchteLuftApp::InitInstance()
{
	// InitCommonControlsEx() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE-Bibliotheken initialisieren
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() ist für die Verwendung des RichEdit-Steuerelements erforderlich.	
	// AfxInitRichEdit2();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	//SetRegistryKey(_T("Software\\IBS\\FeuchteLuft"));
	SetRegistryKey(CFeuchteLuftApp::GetName());
	LoadStdProfileSettings(4);  // Standard INI-Dateioptionen laden (einschließlich MRU)
	
	// Read the registry
	ReadRegistry();

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MOLLIERDIA_TYPE,
		RUNTIME_CLASS(CFeuchteLuftDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CFeuchteLuftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Haupt-MDI-Rahmenfenster erstellen
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Rufen Sie DragAcceptFiles nur auf, wenn eine Suffix vorhanden ist.
	//  In einer MDI-Anwendung ist dies unmittelbar nach dem Festlegen von m_pMainWnd erforderlich
	// Öffnen mit Drag  Drop aktivieren
	m_pMainWnd->DragAcceptFiles();

	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// DDE-Execute-Open aktivieren
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Verteilung der in der Befehlszeile angegebenen Befehle. Gibt FALSE zurück, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CFeuchteLuftApp::ExitInstance()
{
	//TODO: Zusätzliche Ressourcen behandeln, die Sie möglicherweise hinzugefügt haben
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

//-------------------------------------------------------------------------
CString CFeuchteLuftApp::GetVersion()
{
	CString sReturn;
	TCHAR szModulePath[MAX_PATH];
	DWORD dwSize;
	DWORD dwZero;
	char* pBuffer;
	VS_FIXEDFILEINFO* pFixedInfo;
	UINT uVersionLen;

	GetModuleFileName(NULL, szModulePath, MAX_PATH - 1);
	TRACE("CFeuchteLuftApp::GetVersion() - szModulePath=\"%S\"\n", szModulePath);
	dwSize = GetFileVersionInfoSize(szModulePath, &dwZero);

	if (dwSize > 0)
	{
		pBuffer = new char[dwSize];

		GetFileVersionInfo(szModulePath, dwZero, dwSize, pBuffer);
		VerQueryValue(pBuffer, _T("\\"), (void**)&pFixedInfo, (UINT*)&uVersionLen);

		sReturn.Format(_T("Version %u.%02u.%02u"), HIWORD(pFixedInfo->dwProductVersionMS), LOWORD(pFixedInfo->dwProductVersionMS), HIWORD(pFixedInfo->dwProductVersionLS));
		//										LOWORD(pFixedInfo->dwProductVersionLS));
		delete[] pBuffer;
	}

	return sReturn;
}

//-------------------------------------------------------------------------
CString CFeuchteLuftApp::GetName()
{
	return _T("FeuchteLuft");
}

//-------------------------------------------------------------------------
CString CFeuchteLuftApp::GetMainDir()
{
	CString sReturn;
	TCHAR theBuffer[1024];
	DWORD theBytes = GetModuleFileName(NULL, theBuffer, (sizeof(theBuffer) - 1) / sizeof(TCHAR));
	TRACE("CFeuchteLuftApp::GetModuleFileName() returned \"%S\"\n", theBuffer);
	if (theBytes != 0)
	{
		TCHAR* thePtr = theBuffer + theBytes;

		while (thePtr >= theBuffer && *thePtr != _TCHAR('\\'))
		{
			thePtr--;
		}
		*thePtr = _TCHAR('\0');

		sReturn = CString(theBuffer) + _T("\\");
	}

	return sReturn;
}

//-------------------------------------------------------------------------
// Read the registry
void CFeuchteLuftApp::ReadRegistry()
{
	// create singleton registry
	g_pRegistry = new CFeuchteLuftRegistry();



}


// CFeuchteLuftApp-Meldungshandler

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CFeuchteLuftApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Anwendungsbefehl zum Ausführen des Dialogfelds
void CFeuchteLuftApp::OnCanCalc()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CFeuchteLuftApp Methoden zum Laden/Speichern von Anpassungen

void CFeuchteLuftApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CFeuchteLuftApp::LoadCustomState()
{
}

void CFeuchteLuftApp::SaveCustomState()
{
}

// CFeuchteLuftApp-Meldungshandler



