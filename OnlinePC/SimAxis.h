#pragma once


// CSimAxis ダイアログ

class CSimAxis : public CDialog
{
	DECLARE_DYNAMIC(CSimAxis)

public:
	CSimAxis(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSimAxis();

// member function
public:
	void SetupSimAxisDlg(void);
	BOOL UpdateAxisSetting(void);

// dialog data
public:
	enum { IDD = IDD_SIM_AXIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	CComboBox m_cmbTmScale;
	CEdit m_editTempScale;
	CEdit m_editTempZero;
	virtual BOOL OnInitDialog();
};
