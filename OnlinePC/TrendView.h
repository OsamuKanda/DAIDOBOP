#pragma once
#include "afxwin.h"


// TrendView �_�C�A���O

class CTrendView : public CDialog
{
	DECLARE_DYNAMIC(CTrendView)

public:
	CTrendView(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CTrendView();

// member data
public:
	TrendSelect	m_stTdSel[NumADChannel];

// member function
public:
	void SetTrendSelect(TrendSelect *data);
	void GetTrendSelect(TrendSelect *data);
	void SetupTrendViewDlg(void);
	void StoreTrendViewDlg(void);

// dialog data
	enum { IDD = IDD_TREND_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	CListBox m_listData;
};
