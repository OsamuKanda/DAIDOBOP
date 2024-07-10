#pragma once
#include "afxwin.h"
#include "TcVwDlg.h"
#include "../lib/ColorStatic.h"

#define	WM_CHARTDATA		(WM_APP + 0)
#define	WM_ACTINFO			(WM_APP + 1)
#define MAXTIME		100	// 300sec(/1sec)

// CChart3dDlg ダイアログ

class CChart3dDlg : public CDialog
{
	DECLARE_DYNAMIC(CChart3dDlg)

public:
	CChart3dDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CChart3dDlg();

// メンバ
public:
	// Chart FX Windows
	IChartFXPtr m_pChartFX;
	CWnd		m_ChartFX;
	int			m_iData[MAXTIME][NumTCTap];
	BOOL		m_isHideCloseButton;
	CStatic		m_textCurTime;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class

	void SetAverageData(int *data);
	void HideCloseButton(BOOL bHide);
	void SetTcBackColor(void);

// ダイアログ データ
public:
	enum { IDD = IDD_3DCHART_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnCounter();
	afx_msg void OnSurface();
	afx_msg void OnButtonUP();
	afx_msg void OnButtonDOWN();
	afx_msg void OnButtonLEFT();
	afx_msg void OnButtonRIGHT();
	afx_msg void OnButtonS1();
	afx_msg void OnButtonS2();
	afx_msg void OnButtonS3();
	afx_msg void OnButtonN1();
	afx_msg void OnButtonN2();
	afx_msg void OnButtonN3();
	afx_msg	LRESULT OnAddone(WPARAM, LPARAM lParam);
	afx_msg	LRESULT OnActInfo(WPARAM, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton11();
	CButton m_btnClose;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CColorStatic m_stTC1;
	CColorStatic m_stTC2;
	CColorStatic m_stTC3;
	CColorStatic m_stTC4;
	CColorStatic m_stTC5;
	CColorStatic m_stTC6;
	CColorStatic m_stTC7;
	CColorStatic m_stTC8;
	CColorStatic m_stTC9;
	CColorStatic m_stTC10;
	CColorStatic m_stTC11;
	CColorStatic m_stTC12;
	CColorStatic m_stTC13;
	CColorStatic m_stTC14;
	CColorStatic m_stTC15;
	CColorStatic m_stTC16;

	CStatic m_stTCNo1;
	CStatic m_stTCNo2;
	CStatic m_stTCNo3;
	CStatic m_stTCNo4;
	CStatic m_stTCNo5;
	CStatic m_stTCNo6;
	CStatic m_stTCNo7;
	CStatic m_stTCNo8;
	CStatic m_stTCNo9;
	CStatic m_stTCNo10;
	CStatic m_stTCNo11;
	CStatic m_stTCNo12;
	CStatic m_stTCNo13;
	CStatic m_stTCNo14;
	CStatic m_stTCNo15;
	CStatic m_stTCNo16;

	CStatic	m_textCastVc;
	CStatic	m_textWidth;
	CStatic	m_textActLevel;
	CStatic	m_textCastLen;
	CStatic	m_textEMBr;
	CStatic	m_static[10];

	afx_msg void OnTcposition();
};
