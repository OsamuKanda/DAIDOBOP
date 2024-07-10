#pragma once
#include "afxwin.h"


// CTrendSel �_�C�A���O

class CTrendSel : public CDialog
{
	DECLARE_DYNAMIC(CTrendSel)

public:
	CTrendSel(CWnd* pParent = NULL);   // �W���R���X�g���N�^
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	CListBox m_listData;
	virtual BOOL OnInitDialog();
};
