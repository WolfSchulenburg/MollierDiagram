// LayerDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FeuchteLuft.h"
#include "LayerDlg.h"
#include "afxdialogex.h"


// CLayerDlg-Dialog

IMPLEMENT_DYNAMIC(CLayerDlg, CDialogEx)

CLayerDlg::CLayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAYERDLG, pParent)
{

}

CLayerDlg::~CLayerDlg()
{
}

void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cmbLayerSelect);
	DDX_Control(pDX, IDC_CHECK1, btnLayerToggle);
	DDX_Control(pDX, IDC_COMBO2, cmbLayerPenIdx);
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLayerDlg::OnBnClickedSave)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLayerDlg::OnSelChangeLayer)
END_MESSAGE_MAP()


// CLayerDlg-Meldungshandler


BOOL CLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cmbLayerSelect.AddString(_T("Linien gesättiger Feuchte"));	// SaturatedLines
	cmbLayerSelect.AddString(_T("Linien der Feuchtkugeltemperaturen"));	// WetBulbTemperatureLine
	cmbLayerSelect.AddString(_T("Linien der Enthalpien"));	// EnthalpyLine
	cmbLayerSelect.AddString(_T("Linien absoluter Feuchte"));	// AbsoluteHumidityLine
	cmbLayerSelect.AddString(_T("Linien der Trockenkugeltemperaturen"));	// DryBulbTemperatureLine
	cmbLayerSelect.AddString(_T("Linien relativer Feuchte"));	// RelativeHumidityLine
	cmbLayerSelect.AddString(_T("Linien spezifischer Feuchte"));	// SpecificVolumeLine
	cmbLayerSelect.SetCurSel(0);
	nLastSelected = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}


void CLayerDlg::OnOK()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialogEx::OnOK();
}


void CLayerDlg::OnCancel()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialogEx::OnCancel();
}


void CLayerDlg::OnBnClickedSave()	// Übernehmen
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
}


void CLayerDlg::OnSelChangeLayer()
{
	int nIndex = cmbLayerSelect.GetCurSel();
	if (nIndex != nLastSelected)
		nLastSelected = nIndex;
}
