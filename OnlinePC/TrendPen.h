#pragma once
#include "afxwin.h"


// CTrendPen ダイアログ

class CTrendPen : public CDialog
{
	DECLARE_DYNAMIC(CTrendPen)

public:
	CTrendPen(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTrendPen();

// member data
public:
	int		m_nPen;

// menber function
public:
	void SetupTrendPenDlg(void);
	void SetPenArgument(int pen, int sel);
	void UpdatePenArgSetting(void);
	int GetTcNo(LPCTSTR pc);

// dialog data
public:
	enum { IDD = IDD_TREND_PEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CButton m_chkPen01;
	CButton m_chkPen02;
	CButton m_chkPen03;
	CButton m_chkPen04;
	CButton m_chkPen05;
	CButton m_chkPen06;
	CButton m_chkPen07;
	CButton m_chkPen08;
	CButton m_chkPen09;
	CButton m_chkPen10;
	CButton m_chkPen11;
	CButton m_chkPen12;
	CButton m_chkPen13;
	CButton m_chkPen14;
	CButton m_chkPen15;
	CButton m_chkPen16;
	CEdit m_editPenName01;
	CEdit m_editPenName02;
	CEdit m_editPenName03;
	CEdit m_editPenName04;
	CEdit m_editPenName05;
	CEdit m_editPenName06;
	CEdit m_editPenName07;
	CEdit m_editPenName08;
	CEdit m_editPenName09;
	CEdit m_editPenName10;
	CEdit m_editPenName11;
	CEdit m_editPenName12;
	CEdit m_editPenName13;
	CEdit m_editPenName14;
	CEdit m_editPenName15;
	CEdit m_editPenName16;
	CListBox m_listDispArg;
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
protected:
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
