#pragma once

#include "Registry.h"

class CFeuchteLuftRegistry : public CRegistry
{
	typedef CRegistry super;

	//=====================================================================
	//== ctor/dtor/initializing                                          ==
	//=====================================================================
public:
	CFeuchteLuftRegistry();
	virtual ~CFeuchteLuftRegistry();

	//-- Write the initial data to the registry
private:
	static void CreateDefaultEntries();

	//=====================================================================
	//== Accessor for application settings                               ==
	//=====================================================================
	//-- Returns the page size
public:
	static CSize GetPageSize();

	//-- Returns the print scale
	static double GetPrintScale();

	//-- Returns black and white print
	static bool GetPrintBandW();

	//-- Returns MDI child window state
	static bool GetMDIMaximize();

	//-- Returns main window state
	static bool GetMaximize();

	//=====================================================================
	//== Mutator for application settings                               ==
	//=====================================================================
	//-- Changes the page size
	static void SetPageSize(CSize szPage);

	//-- Changes the print scale
	static void SetPrintScale(double nPrintScale);

	//-- Changes black and white print
	static void SetPrintBandW(bool nPrintBandW);

	//-- Changes MDI child window state
	static void SetMDIMaximize(bool bMDIMaximize);

	//-- Changes main window state
	static void SetMaximize(bool bWndMaximize);

	//=====================================================================
	//== class constants                                                 ==
	//=====================================================================
	//-- The revision number for storage in the registry database
private:
	static const CString M_SKEY;

	static const CString M_SPAGESIZE;
	static const CString M_SPRINTSCALE;
	static const CString M_SPRINTBANDW;
	static const CString M_SMDIMAXIMIZE;
	static const CString M_SMAXIMIZE;
};
//=========================================================================

