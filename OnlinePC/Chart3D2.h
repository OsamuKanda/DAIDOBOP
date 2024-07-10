#pragma once
#include "stdafx2.h"
#include "TcVwDlg.h"

#define	WM_CHARTDATA		(WM_APP + 0)

// CChart3DDlg �_�C�A���O
class CChart3DDlg : public CWinFormsDialog<Chart3D::Chart3DControl>
{
	DECLARE_DYNAMIC(CChart3DDlg)

public:
	CChart3DDlg(CWnd* pParent, BOOL isCloseBtn);   // �W���R���X�g���N�^
	virtual ~CChart3DDlg();

public:
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class
	CDialog		*m_pDlgClose;
	int			m_iData[3][100][12];
	BOOL		m_isCloseBtn;

protected:
	CMenu	m_menu;

public:
// �_�C�A���O �f�[�^

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcposition();
	afx_msg	LRESULT OnAddone(WPARAM, LPARAM lParam);
};
