#pragma once
#include "afxwin.h"
#include "../Lib/ColorStatic.h"

// CTestDlg ダイアログ

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTestDlg();

	void BoConditionManage(WPARAM wParam, int cmd);

public:
// dialog data
	enum { IDD = IDD_TEST_DLG };
	UINT	m_nTimerID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnClickedPrOn();
	afx_msg void OnClickedChargeChange();
	afx_msg void OnClickedAutoLevel();
	afx_msg void OnClickedTdChange();
	afx_msg void OnClickedL1Error();
	afx_msg void OnBnClickedEnabsim();
	CButton m_rdoPrOn;
	CButton m_rdoChargeChange;
	CButton m_rdoAutoLevel;
	CButton	m_rdoTdChg;
	CButton	m_rdoL1Err;
	CButton	m_rdoBopCond;
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBocout();
	afx_msg void OnBnClickedBoout();
	afx_msg void OnBnClickedBorun();
	CButton m_chkBocOut;
	CButton m_chkBoOut;
	CButton m_chkBoRun;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	CColorStatic m_txtPlcErr1;
	CColorStatic m_txtPlcErr2;
	CColorStatic m_txtPlcErr3;
	CColorStatic m_txtPlcErr4;
	CColorStatic m_txtPlcErr5;
	CColorStatic m_txtPlcErr6;
	CColorStatic m_txtPlcErr7;
	CColorStatic m_txtPlcErr8;
	CColorStatic m_txtPlcErr9;
	CColorStatic m_txtPlcErr10;
	CColorStatic m_txtPlcErr11;
	CColorStatic m_txtPlcErr12;
	CColorStatic m_txtPlcErr13;
	CColorStatic m_txtPlcErr14;
	CColorStatic m_txtPlcErr15;
	CColorStatic m_txtPlcErr16;
	CColorStatic m_txtPlcErr17;
	CColorStatic m_txtPlcErr18;
	CColorStatic m_txtPlcErr19;
};
