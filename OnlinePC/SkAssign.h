#pragma once
#include "afxwin.h"


// CSkAssign dialog
class CSkAssign : public CDialog
{
	DECLARE_DYNAMIC(CSkAssign)

public:
	CSkAssign(CWnd* pParent = NULL);
	virtual ~CSkAssign();

// member data.
public:
	void SetupSteelKindDlg(void);
	BOOL StoreSteelKindDlg(void);

// dialog data.
public:
	enum { IDD = IDD_SK_ASSIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbSk01;
	CComboBox m_cmbSk02;
	CComboBox m_cmbSk03;
	CComboBox m_cmbSk04;
	CComboBox m_cmbSk05;
	CComboBox m_cmbSk06;
	CComboBox m_cmbSk07;
	CComboBox m_cmbSk08;
	CComboBox m_cmbSk09;
	CStatic m_textSG1;
	CStatic m_textSG2;
	CStatic m_textSG3;
	CStatic m_textSG4;
	CStatic m_textSG5;
	CStatic m_textSG6;
	CStatic m_textSG7;
	CStatic m_textSG8;
	CStatic m_textSG9;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
};
