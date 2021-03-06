// ConfigurationDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FeuchteLuft.h"
#include "ConfigurationDlg.h"
#include "afxdialogex.h"

// CConfigurationDlg-Dialog

IMPLEMENT_DYNAMIC(CConfigurationDlg, CDialogEx)

CConfigurationDlg::CConfigurationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFDLG, pParent)
{
	m_fAtmosphericPressure = _T("1013.25");
	SetDefaultValues();
}

CConfigurationDlg::~CConfigurationDlg()
{
}

void CConfigurationDlg::SetDefaultValues()
{
	m_fTemperatureMin = -12.0;
	m_fTemperatureMax = 40.0;
	m_fAbsHumidityMin = 0.0;
	m_fAbsHumidityMax = 20.0;
}

void CConfigurationDlg::SetTemperatureRange(double minValue, double maxValue)
{

}

void CConfigurationDlg::SetAbsoluteHumidityRange(double minValue, double maxValue)
{

}

bool CConfigurationDlg::CheckValues()
{
	return true;
}

void CConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fAtmosphericPressure);
	DDX_Control(pDX, IDC_EDIT2, m_cTemperatureMin);
	DDX_Control(pDX, IDC_EDIT3, m_cTemperatureMax);
	DDX_Control(pDX, IDC_EDIT4, m_cAbsHumidityMin);
	DDX_Control(pDX, IDC_EDIT5, m_cAbsHumidityMax);
	DDX_Control(pDX, IDC_SPIN1, m_cTempMinSpinCtrl);
	DDX_Control(pDX, IDC_SPIN2, m_cTempMaxSpinCtrl);
	DDX_Control(pDX, IDC_SPIN3, m_cAbsHumMinSpinCtrl);
	DDX_Control(pDX, IDC_SPIN4, m_cAbsHumMaxSpinCtrl);
}

BOOL CConfigurationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;

	str.Format(L"%.2f", m_fTemperatureMin);
	m_cTemperatureMin.SetWindowText(str);
	str.Format(L"%.2f", m_fTemperatureMax);
	m_cTemperatureMax.SetWindowText(str);
	str.Format(L"%.2f", m_fAbsHumidityMin);
	m_cAbsHumidityMin.SetWindowText(str);
	str.Format(L"%.2f", m_fAbsHumidityMax);
	m_cAbsHumidityMax.SetWindowText(str);

	m_cTempMinSpinCtrl.SetRange(-2000, 0);
	m_cTempMinSpinCtrl.SetPos(-1200);

	m_cTempMaxSpinCtrl.SetRange(0, 8000);
	m_cTempMaxSpinCtrl.SetPos(4000);

	m_cAbsHumMinSpinCtrl.SetRange(0, 1000);
	m_cAbsHumMinSpinCtrl.SetPos(0);

	m_cAbsHumMaxSpinCtrl.SetRange(1000, 4000);
	m_cAbsHumMaxSpinCtrl.SetPos(2000);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

BEGIN_MESSAGE_MAP(CConfigurationDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CConfigurationDlg::OnTempMinSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CConfigurationDlg::OnTempMaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CConfigurationDlg::OnHumidityMinSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CConfigurationDlg::OnHumidityMaxSpin)
END_MESSAGE_MAP()

// CConfigurationDlg-Meldungshandler

void CConfigurationDlg::OnTempMinSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nPos = pNMUpDown->iPos;
	CString str;
	str.Format(L"%.2f", nPos / 100.0);
	m_cTemperatureMin.SetWindowText(str);

	*pResult = 0;
}


void CConfigurationDlg::OnTempMaxSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nPos = pNMUpDown->iPos;
	CString str;
	str.Format(L"%.2f", nPos / 100.0);
	m_cTemperatureMax.SetWindowText(str);

	*pResult = 0;
}


void CConfigurationDlg::OnHumidityMinSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.

	int nPos = pNMUpDown->iPos;
	CString str;
	str.Format(L"%.2f", nPos / 100.0);
	m_cAbsHumidityMin.SetWindowText(str);

	*pResult = 0;
}


void CConfigurationDlg::OnHumidityMaxSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	int nPos = pNMUpDown->iPos;
	CString str;
	str.Format(L"%.2f", nPos / 100.0);
	m_cAbsHumidityMax.SetWindowText(str);

	*pResult = 0;
}


void CConfigurationDlg::OnOK()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialogEx::OnOK();
}


void CConfigurationDlg::OnCancel()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialogEx::OnCancel();
}
