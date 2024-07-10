#pragma once
#include "afxwin.h"


// CTrendSel ダイアログ

class CTrendSel : public CDialog
{
	DECLARE_DYNAMIC(CTrendSel)

public:
	CTrendSel(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTrendSel();

// member function
public:
	void SetTrendSelect(TrendSelect *data);
	void GetTrendSelect(TrendSelect *data);
	void SetupTrendSelectDlg(void);
	void ChangeIndication(void);

// member data
public:
	TrendSelect m_stTdSel[NumADChannel];
	int			m_argTdList[NumADChannel];

// dialog data
	enum { IDD = IDD_TREND_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	CListBox m_listData;
	virtual BOOL OnInitDialog();
};
