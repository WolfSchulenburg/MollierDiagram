#pragma once

// CLayerDlg-Dialog

class CLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CLayerDlg();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();

	virtual void OnOK();
	virtual void OnCancel();

private:
	CButton btnLayerToggle;
	CComboBox cmbLayerSelect;
	CComboBox cmbLayerPenIdx;
	int nLastSelected;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedSave();
	afx_msg void OnSelChangeLayer();
};
