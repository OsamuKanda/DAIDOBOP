#pragma once
#include "SimGraph.h"
#include "LogSel2.h"
#include "NetSel.h"
#include "NetCalc.h"
#include "SimAxis.h"
#include "SimExe.h"
#include "TcVwDlg.h"
#include "afxwin.h"


///////////////////////////////////////////////////////////////////////////////
// CSimDlg class
#define	SIM_LOOP_CNT	60


///////////////////////////////////////////////////////////////////////////////
// CSimDlg class
class CSimDlg : public CDialog
{
	DECLARE_DYNAMIC(CSimDlg)

public:
	CSimDlg(CWnd* pParent = NULL);
	virtual ~CSimDlg();

// member data
public:
	int			m_nNetIdx;
	int			m_nCombiIdx;
	int			m_nSelectIdx;
	BOOL		m_isSelFile;
	CSimExe		m_exe;
	CNetCalc	*m_pDlgCalc;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class

	BOOL		m_bFilterSwitch;
	float		m_fFilterFacter;

// member function
public:
	// draw Y scale
	void DrawYScale(CDC &dc);
	void DrawNetList(CDC &dc);
	int	 DispSimExeErrMsg(int ret);
	BOOL AlarmSave(LPCTSTR dir, DWORD startNo, DWORD dataCnt, float fFilter);
	BOOL OrdinarySave(LPCTSTR dir, DWORD startNo, DWORD dataCnt, float fFilter);
	void RefreshSimOut( void );
	void DispLoggedData(CLogSel2& dlg);
	void BoConditionManage(WPARAM wParam, int cmd);
	void ChgFilterSet(void);

// dialog data
public:
	enum { IDD = IDD_SIM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnDataSelect();
	afx_msg void OnNetSelect();
	CSimGraph m_graph;
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CStatic m_textFileName;
	CStatic m_textRemark;
	afx_msg void OnDoDisplay();
	afx_msg void OnAxisConfig2();
	CStatic m_textYScale;
	afx_msg void OnPaint();
	CStatic m_textConfName;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBackProp();
	CStatic m_textDataList;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChartSelect();
	afx_msg void OnSelectAlarm();
	afx_msg void OnSelectOrdinary();
	afx_msg void OnMove(int x, int y);
	afx_msg LRESULT OnSimChg(WPARAM, LPARAM);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnTcposition();
	afx_msg void OnClickedFilterSw();
	CButton m_chkFilter;
	CEdit m_editTempFilter;
	afx_msg void OnBnClickedDisplay();
};

