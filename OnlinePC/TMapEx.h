#pragma once
#include "afxwin.h"

#define	WM_RECSTART			(WM_APP + 0x10)
#define	WM_RECSTOP			(WM_APP + 0x11)
#define	WM_BATCHSTOP		(WM_APP + 0x12)
#define	WM_TMAP_SCR			(WM_APP + 0x13)

CScrollBar *GetScr(CDialog *pThis);
CStatic *GetText(CDialog *pThis);

// CTMapEx �_�C�A���O
class CTMapEx : public CDialog
{
	DECLARE_DYNAMIC(CTMapEx)

public:
	CTMapEx(CDialog* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CTMapEx();

protected:
	CDialog		*m_pParent;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_TMAP_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_scr;
	CStatic m_txtCurTime;
};
