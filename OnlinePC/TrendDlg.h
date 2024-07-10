#pragma once
#include "afxwin.h"
#include "TrendGraph.h"
#include "LogSel2.h"
#include "TrendPen.h"
#include "TrendAxis.h"
#include "DrvAcc.h"
#include "TcVwDlg.h"

// CTrendDlg ダイアログ

class CTrendDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrendDlg)

public:
	CTrendDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTrendDlg();

// member data
public:
	BOOL		m_isSelFile;
	int			m_nSelectIdx;
	short		m_nPenData[NumTrendChannel];
	time_t		m_tScooter;
	CTcVwDlg	*m_pDlgTcView;

	BOOL		m_bFilterSwitch;
	float		m_fFilterFacter;
	float		m_fFilterFacterSn;
	float		m_fFilterFacterLv;
	float		m_fFilterFacterPr;

// member function
public:
	void DrawYScale(CDC &dc);
	void DrawPenList(CDC &dc);
	void DispLoggedData(CLogSel2& dlg);
	void GetScooterData(int nPos);
	void BoConditionManage(WPARAM wParam, int cmd);


// dialog data
public:
	enum { IDD = IDD_TREND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	CTrendGraph m_graph;
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLoggedData();
	afx_msg void OnAxisset();
	afx_msg void OnPenSelect();
	CStatic	m_textFileName;
	CStatic m_textRemark;
	afx_msg void OnPaint();
	CStatic m_textPenList;
	CStatic m_textYScale;
	CStatic	m_textScDate;
	CStatic	m_textScTime;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDrive();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_scrScooter;
	afx_msg void OnTcPosition();
	afx_msg void OnClickedFilterSw();
	CButton m_chkFilter;
	CEdit m_editTempFilter;
	CEdit m_editSnFilter;
	CEdit m_editLevelFilter;
	CEdit m_editVcFilter;
	afx_msg void OnBnClickedDisplay();
};
