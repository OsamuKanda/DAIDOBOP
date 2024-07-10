// OnlinePCDlg.h : header file
//
#pragma once

// include files
#include "BoViewDlg.h"
#include "PredicDlg.h"
#include "TMap2If.h"
#include "Chart3D2If.h"
#include "BpnetDlg.h"
#include "SimDlg.h"
#include "BatchSimDlg.h"
#include "TrendDlg.h"
#include "ConfigDlg.h"
#include "BosysDlg.h"
//#include "CastInfoDlg.h"
//#include "InfoOut.h"
#include "MtiTime.h"
#include "PlcComm.h"
#include "../lib/ColorStatic.h"
#include "afxwin.h"
#include <atlimage.h>

// COnlinePCDlg dialog
class COnlinePCDlg : public CDialog
{
// user data
public:
	CDialog			*m_pDlgActive;			// active screen
	CBoViewDlg		*m_pDlgBoView;			// Bo view screen
	CPredicDlg		*m_pDlgPredic;			// Bo prediction screen
	CDialog			*m_pDlgTMap;			// thermal mapping screen
	CDialog			*m_pDlgChart3d;			// 3d chart screen
	CBpnetDlg		*m_pDlgBpnet;			// learning screen
	CSimDlg			*m_pDlgSim;				// simulation screen
	CBatchSimDlg	*m_pDlgBatchSim;		// batch simulation screen
	CTrendDlg		*m_pDlgTrend;			// trend screen
	CConfigDlg		*m_pDlgConfig;			// config setting screen
	CBosysDlg		*m_pDlgBosys;			// bo system setting screen
//	CCastInfoDlg	*m_pDlgCastInfo;		// casting information screen
//	CInfoOut		*m_pInfoOut;			// information output screen
	CPlcComm		m_plccomm;

	UINT			m_nTimer1, m_nTimer2;
	BOOL			m_bInfoOutBk;
	BOOL			m_isAppEnd;

	// ロゴ関連
	CImage			m_imgPaint;
	CDC				m_dcPaint;


// Construction
public:
	COnlinePCDlg(CWnd* pParent = NULL);	// standard constructor

// member function
public:
	void DispStrandLamp(void);				// ストランドランプ表示
	BOOL OpenSubWindows(void);				// open sub screen (first time)
	template<class T>
	void OpenScreen(T *pScreen);			// open screen (each screen)
	void OpenScreen3D(CDialog *pScreen);	// open screen (for 3D)
	void SetAllButtonOff(void);				// set all button off
	void ChgButtonHighlight(void);			// change button highlight

// Dialog Data
public:
	enum { IDD = IDD_MAIN_MENU };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnClickedPredicBtn();
	afx_msg void OnClickedTmapBtn();
	afx_msg void OnClicked3dchartBtn();
	afx_msg void OnClickedBoviewBtn();
	afx_msg void OnClickedBpnetBtn();
	afx_msg void OnClickedBatchsimBtn();
	afx_msg void OnClickedTrendBtn();
	afx_msg void OnBnClickedConfigBtn();
	afx_msg void OnClickedBosysBtn();
	afx_msg void OnTimer(UINT nIDEvent);
	CButton			m_btnInfoOut;
	CColorStatic	m_txtStrand;
	CColorStatic	m_txtMsgOut;
	CButton m_btnQuit;
	afx_msg LRESULT OnTMSQuit(WPARAM, LPARAM);
	afx_msg void OnBnClickedInfoOutBtn();
	afx_msg void OnBnClickedCastinfoBtn();
	CStatic m_picLogo;
};
