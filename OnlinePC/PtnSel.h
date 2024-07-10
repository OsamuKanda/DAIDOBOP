#pragma once
#include "config.h"
#include "afxwin.h"

// CPtnSel dialog
class CPtnSel : public CDialog
{
	DECLARE_DYNAMIC(CPtnSel)

public:
	CPtnSel(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPtnSel();

// member data
	int		m_nPtnNo;
	int		m_nMoldSize;
	BOOL	m_bHide;

// member function
	// for predic
	void SetParam(CConfig &config, int pno, int nMoldSize);
	void GetParam(int *pno, int *pMoldSize);
	// for batchsim
	void SetParam(CConfig &config, int pno);
	void GetParam(int *pno);

// dialog data
public:
	enum { IDD = IDD_PTN_SEL2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedRadio1();
	afx_msg void OnClickedRadio2();
	afx_msg void OnClickedRadio3();
	afx_msg void OnClickedRadio4();
	afx_msg void OnClickedRadio5();
	afx_msg void OnClickedRadio6();
	afx_msg void OnClickedRadio7();
	afx_msg void OnClickedRadio8();
	afx_msg void OnClickedRadio9();
	afx_msg void OnClickedOk();
	CEdit m_editMdSize;
	CStatic m_textSG[NumSteelKind];
	CButton m_rdoChk[NumSteelKind];
	CStatic m_text1;
	CStatic m_text2;
};
