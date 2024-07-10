#pragma once
#include "afxwin.h"
#include "TcVwDlg.h"

#define	WM_MAPDATA		(WM_APP + 0)

// CTMapDlg ダイアログ

class CTMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMapDlg)

public:
	CTMapDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTMapDlg();

// メンバ
public:
	IChartFXPtr m_pChartFIX;
	IChartFXPtr m_pChartLOOSE;
	CWnd		m_ChartFIX;
	CWnd		m_ChartLOOSE;
	BOOL		m_isHideCloseButton;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class

	void SetAverageData(int *data);
	void GetAverageData(int *pData);
	void CalcAverageData(int f, int *pData);
	void HideCloseButton(BOOL bHide);


// dialog data
public:
	enum { IDD = IDD_TMAP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();
    virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCancel();
	afx_msg LRESULT OnAddone(WPARAM, LPARAM) ;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton1();
	CButton m_btnClose;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	void PostPaintChartfix(short w, short h, long lPaint, short* nRes);
	void PostPaintChartloose(short w, short h, long lPaint, short* nRes);
	afx_msg void OnTcposition();
};
