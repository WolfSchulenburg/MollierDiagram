#include "stdafx.h"
#include "FeuchteLuftRegistry.h"

const CString CFeuchteLuftRegistry::M_SKEY = _T("Software\\IBS\\FeuchteLuft\\1x20");
const CString CFeuchteLuftRegistry::M_SPAGESIZE = _T("PageSize");
const CString CFeuchteLuftRegistry::M_SPRINTSCALE = _T("PrintScaleFactor");
const CString CFeuchteLuftRegistry::M_SPRINTBANDW = _T("PrintBandW");
const CString CFeuchteLuftRegistry::M_SMDIMAXIMIZE = _T("MdiMaximize");
const CString CFeuchteLuftRegistry::M_SMAXIMIZE = _T("Maximize");

//=========================================================================
//== ctor/dtor/initializing                                              ==
//=========================================================================

//-------------------------------------------------------------------------
//-- Attach this object to the registry
CFeuchteLuftRegistry::CFeuchteLuftRegistry() :
	CRegistry()
{
	m_oKey.Create(HKEY_CURRENT_USER, M_SKEY);

	// Does the registry information exist - if not create it
	if (!keyExists(M_SPAGESIZE))
	{
		CreateDefaultEntries();
	}
}
//-------------------------------------------------------------------------
CFeuchteLuftRegistry::~CFeuchteLuftRegistry()
{
}
//-------------------------------------------------------------------------
//-- Write the initial data to the registry
void CFeuchteLuftRegistry::CreateDefaultEntries()
{
	TRACE("Virgin install detected:  CFeuchteLuftRegistry::CreateDefaultEntries() is creating the very first registry entries for FeuchteLuft\n");

	// Save the page size setup
	// SetPageSize(CSize(297 * PIXELSPERMM, 210 * PIXELSPERMM));
	SetMaximize(false);

	// Finally place the association of this file and the design type
	// super::Associate(".dsn", "TinyCAD design", "TinyCAD");
}
//-------------------------------------------------------------------------

//=========================================================================
//== Accessor for application settings                                   ==
//=========================================================================

//-------------------------------------------------------------------------
//-- Returns the page size
CSize CFeuchteLuftRegistry::GetPageSize()
{
	CSize szReturn = super::GetSize(M_SPAGESIZE, CSize());

	return szReturn;
}
//-------------------------------------------------------------------------
//-- Returns the print scale
double CFeuchteLuftRegistry::GetPrintScale()
{
	double nReturn = super::GetDouble(M_SPRINTSCALE, 100);

	return nReturn;
}
//-------------------------------------------------------------------------
//-- Returns black and white print
bool CFeuchteLuftRegistry::GetPrintBandW()
{
	bool bReturn = super::GetBool(M_SPRINTBANDW, false);

	return bReturn;
}
//-------------------------------------------------------------------------
//-- Returns MDI child window state
bool CFeuchteLuftRegistry::GetMDIMaximize()
{
	bool bReturn = super::GetBool(M_SMDIMAXIMIZE, false);

	return bReturn;
}
//-------------------------------------------------------------------------
//-- Returns main window state
bool CFeuchteLuftRegistry::GetMaximize()
{
	bool bReturn = super::GetBool(M_SMAXIMIZE, false);

	return bReturn;
}

//=========================================================================
//== Mutator for application settings                                    ==
//=========================================================================

//-------------------------------------------------------------------------
//-- Changes the page size
void CFeuchteLuftRegistry::SetPageSize(CSize szPage)
{
	super::Set(M_SPAGESIZE, szPage);
}
//-------------------------------------------------------------------------
//-- Changes the print scale
void CFeuchteLuftRegistry::SetPrintScale(double nPrintScale)
{
	super::Set(M_SPRINTSCALE, nPrintScale);
}
//-------------------------------------------------------------------------
//-- Changes black and white print
void CFeuchteLuftRegistry::SetPrintBandW(bool bPrintBandW)
{
	CFeuchteLuftRegistry::Set(M_SPRINTBANDW, bPrintBandW);
}
//-------------------------------------------------------------------------
//-- Changes MDI child window state
void CFeuchteLuftRegistry::SetMDIMaximize(bool bMDIMaximize)
{
	CFeuchteLuftRegistry::Set(M_SMDIMAXIMIZE, bMDIMaximize);
}
//-------------------------------------------------------------------------
//-- Changes Changes main window state
void CFeuchteLuftRegistry::SetMaximize(bool bMaximize)
{
	CFeuchteLuftRegistry::Set(M_SMAXIMIZE, bMaximize);
}
//-------------------------------------------------------------------------
