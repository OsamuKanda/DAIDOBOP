#pragma once
#include "afxwin.h"


// CBoexeAxis dialog

class CBoexeAxis : public CDialog
{
	DECLARE_DYNAMIC(CBoexeAxis)

public:
	CBoexeAxis(CWnd* pParent = NULL);
	virtual ~CBoexeAxis();

// member function
public:
	void SetupTrendAxisDlg(void);
	BOOL UpdateAxisSetting(void);

// dialog data
public:
	enum { IDD = IDD_TREND_AXIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	CComboBox m_cmbTmScale;
	CEdit m_editTempScale;
	CEdit m_editTempZero;
	CEdit m_editLevelScale;
	CEdit m_editLevelZero;
	CEdit m_editVcScale;
	CEdit m_editVcZero;
	CEdit m_editSNScale;
	CEdit m_editSNZero;
	CEdit m_editWidthScale;
	CEdit m_editWidthZero;
	CStatic m_textTmScale;
	virtual BOOL OnInitDialog();
};
