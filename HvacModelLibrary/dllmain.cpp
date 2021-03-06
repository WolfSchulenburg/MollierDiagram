// dllmain.cpp : Definiert die Initialisierungsroutinen für die DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE HvacModelLibraryDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden.
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("HvacModelLibrary.DLL wird initialisiert.\n");

		// Einmalige Initialisierung der Extension-DLL
		if (!AfxInitExtensionModule(HvacModelLibraryDLL, hInstance))
			return 0;

		// Diese DLL in Ressourcenkette einfügen
		// HINWEIS: Wird diese Extension-DLL implizit durch
		//  eine reguläre MFC-DLL (z.B. ein ActiveX-Steuerelement)
		//  anstelle einer MFC-Anwendung verknüpft, dann sollten Sie
		//  folgende Zeilen aus DllMain entfernen, und diese in eine separate
		//  Funktion einfügen, die aus der Extension-DLL exportiert wurde.  Die reguläre DLL,
		//  die diese Extension-DLL verwendet, sollte dann explizit die
		//  Funktion aufrufen, um die Extension-DLL zu initialisieren.  Andernfalls
		//  wird das CDynLinkLibrary-Objekt nicht mit der Ressourcenkette der
		//  regulären DLL verbunden, was zu ernsthaften Problemen
		//  führen kann.

		new CDynLinkLibrary(HvacModelLibraryDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("HvacModelLibrary.DLL wird beendet.\n");

		// Bibliothek vor dem Aufruf der Destruktoren schließen.
		AfxTermExtensionModule(HvacModelLibraryDLL);
	}
	return 1;   // OK
}
