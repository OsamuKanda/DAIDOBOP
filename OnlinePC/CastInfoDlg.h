#pragma once
#include "afxwin.h"
#include "TcVwDlg.h"
#include "CondSet.h"
#include "CondFile.h"
#include "InfoSet.h"
#include "InfoFile.h"
#include "../lib/ColorEdit.h"

#define	WM_CHARTDATA2		(WM_APP + 0)
#define	WM_ACTINFO2			(WM_APP + 1)

typedef enum
{
	ID_TempA = 0,
	ID_TempB = 1,
	ID_TempC = 2,
	ID_TempD = 3,
	ID_TempE = 4,
	ID_TempF = 5,
	ID_TempNum = 6,
}NmTempAve;

// CCastInfoDlg ダイアログ

class CCastInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CCastInfoDlg)

public:
	CCastInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCastInfoDlg();

// メンバ
public:
	// Chart FX Windows
	IChartFXPtr m_pChartFX1;
	CWnd		m_ChartFX1;
	IChartFXPtr m_pChartFX2;
	CWnd		m_ChartFX2;
	IChartFXPtr m_pChartFX3;
	CWnd		m_ChartFX3;

	int			m_iData[NumTCTap];
	int			m_nAngleX;
	int			m_nAngleY;
	int			m_nTempMax;
	int			m_nTempMin;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class

	StCondItem	m_cond[COND_ITEM_MAX];	
	int			m_numCond;

	StInfoItem	m_info[INFO_ITEM_MAX];	
	int			m_numInfo;
	BOOL		m_bInfoMsg;

	void SetupChartFX( IChartFXPtr axChartFX );
	void SetupPalleteChartFX( IChartFXPtr pChartFX );
	void UpdateChart( IChartFXPtr pChartFX, int iPos );
	void SetAverageData(int *data);
	void ChangeContourStep( IChartFXPtr pChartFX, int nMin, int nMax );
	void DrawColorAxis(CDC &dc);
	void DrawMold(CDC &dc);
	void UpdateChartAll(void);
	void AddCondList( StCondItem &stItem );
	void UpdateCondList( int nSel, StCondItem &stItem );
	void DeleteCondItem(int nSel);
	void RestructCondItem(void);
	void SetupCondition(void);
	void AddInfoList( StInfoItem &stItem );
	void UpdateInfoList( int nSel, StInfoItem &stItem );
	void DeleteInfoItem(int nSel);
	void RestructInfoItem(void);
	void SetupInformation(void);
	int  GetListCurSel(CListCtrl &list);
	void CreateCondID( StCondItem &stItem );
	BOOL CheckInfoUsed( int nID );
	void SetAverageTemp(StCondInput &stInput);


// ダイアログ データ
	enum { IDD = IDD_CASTINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg	LRESULT OnAddone(WPARAM, LPARAM lParam);
	afx_msg	LRESULT OnActInfo(WPARAM, LPARAM lParam);
	afx_msg void OnSpinAngleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinAngleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinTempMax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinTempMin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdateLoadData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSaveData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRestart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTcposition(CCmdUI *pCmdUI);
	afx_msg void OnLoadData();
	afx_msg void OnSaveData();
	afx_msg void OnRestart();
	afx_msg void OnInfoOut();
	afx_msg void OnTcposition();
	afx_msg void OnClickedCondAdd();
	afx_msg void OnClickedCondUpdate();
	afx_msg void OnClickedCondDelete();
	afx_msg void OnClickedInfoAdd();
	afx_msg void OnClickedInfoUpdate();
	afx_msg void OnClickedInfoDelete();
	CStatic				m_lbColAxis;
	CStatic				m_lbMold;
	CSpinButtonCtrl		m_spnAngleX;
	CSpinButtonCtrl		m_spnAngleY;
	CSpinButtonCtrl		m_spnTempMax;
	CSpinButtonCtrl		m_spnTempMin;
	CColorEdit			m_editAngleX;
	CColorEdit			m_editAngleY;
	CColorEdit			m_editTempMax;
	CColorEdit			m_editTempMin;
	CColorEdit			m_editTempA;
	CColorEdit			m_editTempB;
	CColorEdit			m_editTempC;
	CColorEdit			m_editTempD;
	CColorEdit			m_editTempE;
	CColorEdit			m_editTempF;
	CListCtrl			m_listCondSet;
	CListCtrl			m_listInfoSet;
	CStatic				m_textCastVc;
	CStatic				m_textWidth;
	CStatic				m_textActLevel;
	CStatic				m_textCastLen;
	CStatic				m_textEMBr;
};

