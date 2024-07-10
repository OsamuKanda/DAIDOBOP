#pragma once
#include "stdafx2.h"
#include "TcVwDlg.h"

#define	WM_CHARTDATA		(WM_APP + 0)

// CTMapDlg ダイアログ
class CTMapDlg : public CWinFormsDialog<TMap::TMapControl>
{
	DECLARE_DYNAMIC(CTMapDlg)

public:
	CTMapDlg(CWnd* pParent, BOOL isRec);   // 標準コンストラクタ
	virtual ~CTMapDlg();

	void SetAverageData(int *data, int *datam);
	void GetAverageData(int *pData);
	void CalcAverageData(int f, int *pData);
	void DispCurrentMap(void);
	void DispCurrentTime(void);
	LPCTSTR GetTimeStr(time_t tm);

public:
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class
	CDialog		*m_pDlgRec;
	int			m_iData[36][11][11];
	BOOL		m_isRecord;
	BOOL		m_bBatchStop;
	BOOL		m_bAutoRun;
	StLogInfo	m_stLog;
	int			m_nRecCur;
	int			m_nRecCnt;
	int			m_nStartTime;
	int			m_nOffsetTime;
	CBoBatch	*m_pBatch;

protected:
	CMenu	m_menu;

public:
// ダイアログ データ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcposition();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg	LRESULT OnAddone(WPARAM, LPARAM lParam);
	afx_msg	LRESULT OnRecStart(WPARAM, LPARAM lParam);
	afx_msg	LRESULT OnRecStop(WPARAM, LPARAM lParam);
	afx_msg	LRESULT OnBatchStop(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnTMapScroll(WPARAM, LPARAM lParam);
};
