#pragma once
#include "afxcmn.h"
#include "InfoFile.h"


#define	WM_INFODATA		(WM_APP + 0)


// CInfoOut �_�C�A���O

class CInfoOut : public CDialog
{
	DECLARE_DYNAMIC(CInfoOut)

public:
	CInfoOut(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CInfoOut();

// member
public:
	StInfoOutput	m_out[INFO_INFO_MAX];
	int				m_nDispCnt;
	BOOL			m_bReset;

// member
public:
	void SetupList(void);
	void SendInfoMessage(LPCTSTR pStr);

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_INFO_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnAddone(WPARAM, LPARAM) ;
	CListCtrl m_listInfoOut;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
