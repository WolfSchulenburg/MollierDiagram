#pragma once


// CStatePointDlg-Dialog

class CStatePointDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatePointDlg)

public:
	CStatePointDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CStatePointDlg();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATEPOINTDLG };
#endif
private:
	CListBox m_lbStatePoints;
	CEdit m_txtTemperature;
	CEdit m_txtHumidity;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
