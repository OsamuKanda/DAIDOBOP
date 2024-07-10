#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "PenSetting.h"
#include "BoviewGraph.h"
#include "TcGraph.h"
#include "ExeConf.h"
#include "BovwAxis.h"
#include "TcVwDlg.h"
#include "../lib/ColorButton.h"
#include "../lib/ColorEdit.h"
#include "../lib/ColorStatic.h"


#define	WM_SMPLDATA		(WM_APP + 0)

// CBoViewDlg dialog
class CBoViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CBoViewDlg)

public:
	CBoViewDlg(CWnd* pParent = NULL);   // standard constract
	virtual ~CBoViewDlg();

// member data
public:
	StPenSetInfo	m_ps;
	int				m_nScPosX;
	BOOL			m_bScDrag;
	CRect			m_rcGraph;
	CRect			m_rcYScale;
	ChartAxis		m_stTempAxis2;
	CTcVwDlg		*m_pDlgTcView;				// TC View screen class
	CImageList		m_image;					// 高さを調整する為のイメージリスト
	int				m_nCurView;

// member function
public:
	void ResetBoviewGraph(BOOL bFirst);
	void DrawYScale(CDC &dc);
	void DrawScooter(CDC &dc);
	void SetButtonLamp(int no);
	void MakeScooterPoints(int &x, POINT *pt);
	void SetScooterX(int x);
	void ResetTcGraph(void);
	void DoOffButton(int no);
	void DrawTcScale1(CDC &dc, CStatic &cs);
	void DrawTcScale2(CDC &dc, CStatic &cs);
	BOOL ConfDataSetUp(void);
	BOOL ConfDataUpDate(void);

// dialog data
public:
	enum { IDD = IDD_BOVIEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg LRESULT OnAddone(WPARAM, LPARAM) ;
	afx_msg void OnClickedButton1();
	CListCtrl m_listActData;
	virtual BOOL OnInitDialog();
	CBoviewGraph	m_graph;
	CStatic			m_textYScale;
	CColorButton	m_btnFrz;
	CColorButton	m_btnMinute5;
	CColorButton	m_btnMinute15;
	CColorButton	m_btnMinute30;
	CColorButton	m_btnMinute60;
	CTcGraph		*m_tcg[TC_NO_MAX];
	afx_msg void OnPaint();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	afx_msg void OnClickedButton5();
	afx_msg void OnClickedButton6();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CEdit m_textScVal[PEN_SET_NUM];
	//afx_msg void OnClickedOffButton1();
	//afx_msg void OnClickedOffButton2();
	//afx_msg void OnClickedOffButton3();
	//afx_msg void OnClickedOffButton4();
	//afx_msg void OnClickedOffButton5();
	//afx_msg void OnClickedOffButton6();
	//afx_msg void OnClickedOffButton7();
	//afx_msg void OnClickedOffButton8();
	//afx_msg void OnClickedOffButton9();
	//afx_msg void OnClickedOffButton10();
	//afx_msg void OnClickedOffButton11();
	//afx_msg void OnClickedOffButton12();
	//afx_msg void OnClickedOffButton13();
	//afx_msg void OnClickedOffButton14();
	//afx_msg void OnClickedOffButton15();
	//afx_msg void OnClickedOffButton16();
	//afx_msg void OnClickedOffButton17();
	//afx_msg void OnClickedOffButton18();
	//afx_msg void OnClickedOffButton19();
	//afx_msg void OnClickedOffButton20();
	//afx_msg void OnClickedOffButton21();
	//afx_msg void OnClickedOffButton22();
	//afx_msg void OnClickedOffButton23();
	//afx_msg void OnClickedOffButton24();
	//afx_msg void OnClickedOffButton25();
	//afx_msg void OnClickedOffButton26();
	//afx_msg void OnClickedOffButton27();
	//afx_msg void OnClickedOffButton28();
	//afx_msg void OnClickedOffButton29();
	//afx_msg void OnClickedOffButton30();
	//afx_msg void OnClickedOffButton31();
	//afx_msg void OnClickedOffButton32();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnAxisset();
	CStatic m_textTcScale1;
	CStatic m_textTcScale2;
	CStatic m_textTcScale3;
	CStatic m_textTcScale4;
	CStatic m_textTcScale5;
	CStatic m_textTcScale6;
	CColorEdit m_editScVal[PEN_SET_NUM];
	CColorButton m_btnOFF[12];
	CColorStatic m_stHigh;
	CColorStatic m_stMiddle;
	CColorStatic m_stLow;
	afx_msg void OnTcposition();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
};

