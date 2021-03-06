#pragma once

// CConfigurationDlg-Dialog

class CConfigurationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigurationDlg)

public:
	CConfigurationDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CConfigurationDlg();

	void SetDefaultValues();
	
	void SetTemperatureRange(double minValue, double maxValue);
	void SetAbsoluteHumidityRange(double minValue, double maxValue);
	
	double GetTemperatureMin() { return m_fTemperatureMin; }
	double GetTemperatureMax() { return m_fTemperatureMax; }
	double GetAbsoluteHumidityMin() { return m_fAbsHumidityMin; }
	double GetAbsoluteHumidityMax() { return m_fAbsHumidityMax; }

	bool CheckValues();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();

	virtual void OnOK();
	virtual void OnCancel();

private:
	double m_fTemperatureMin, m_fTemperatureMax;
	double m_fAbsHumidityMin, m_fAbsHumidityMax;

	CString m_fAtmosphericPressure;

	CEdit m_cTemperatureMin;
	CSpinButtonCtrl m_cTempMinSpinCtrl;
	CEdit m_cTemperatureMax;
	CSpinButtonCtrl m_cTempMaxSpinCtrl;
	CEdit m_cAbsHumidityMin;
	CSpinButtonCtrl m_cAbsHumMinSpinCtrl;
	CEdit m_cAbsHumidityMax;
	CSpinButtonCtrl m_cAbsHumMaxSpinCtrl;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTempMaxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTempMinSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHumidityMinSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHumidityMaxSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
