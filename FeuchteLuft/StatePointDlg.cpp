// StatePointDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FeuchteLuft.h"
#include "StatePointDlg.h"
#include "afxdialogex.h"


// CStatePointDlg-Dialog

IMPLEMENT_DYNAMIC(CStatePointDlg, CDialogEx)

CStatePointDlg::CStatePointDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATEPOINTDLG, pParent)
{
}

CStatePointDlg::~CStatePointDlg()
{
}

void CStatePointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbStatePoints);
	DDX_Control(pDX, IDC_EDIT1, m_txtTemperature);
	DDX_Control(pDX, IDC_EDIT2, m_txtHumidity);
}


BEGIN_MESSAGE_MAP(CStatePointDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CStatePointDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &CStatePointDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CStatePointDlg-Meldungshandler


void CStatePointDlg::OnBnClickedAdd()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
}


void CStatePointDlg::OnBnClickedDelete()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
}


BOOL CStatePointDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;

	m_lbStatePoints.AddString(_T("01 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("02 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("01 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("02 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("01 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("02 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("01 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("02 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("01 - Zustand Test 01 - +12.0 °C / 40 % r.F."));
	m_lbStatePoints.AddString(_T("02 - Zustand Test 01 - +12.0 °C / 40 % r.F."));

	str.Format(L"%.2f", 12.0);
	m_txtTemperature.SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}


void CStatePointDlg::OnOK()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialogEx::OnOK();
}


void CStatePointDlg::OnCancel()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialogEx::OnCancel();
}
