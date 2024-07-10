#pragma once
#include "afxwin.h"
#include "BpOpe.h"
#include "BpGraph.h"
#include "SelGraph.h"
#include "PassDlg.h"
#include "TcVwDlg.h"
#include "WeiList.h"
#include "../Lib/ColorStatic.h"
#include "../Lib/ColorButton.h"


///////////////////////////////////////////////////////////////////////////////
// definition
#define	DEF_PATT_ORD	0.0
#define	DEF_PATT_ALM	1.0
#define	DEF_CORR_ORD	0.0
#define	DEF_CORR_ALM	1.0
#define	DEF_CONDITION	(CBpOpe::IsMask)
#define	DEF_EATHA		0.01
#define	DEF_ALPHA		0.005

#define	REPEAT_COUNT	20				// calc repeat count for WM_TIMER

///////////////////////////////////////////////////////////////////////////////
// CBpnetDlg class
class CBpnetDlg : public CDialog
{
	DECLARE_DYNAMIC(CBpnetDlg)

public:
	CBpnetDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBpnetDlg();

// member data
public:
	int			m_nNetIdx;
	int			m_nNetTab[NumNeuralNet];
	int			m_nCombiTab[NumCombination];
	int			m_nTeachCondition;
	double		m_dPattOrdTeachVal;
	double		m_dPattAlmTeachVal;
	double		m_dCorrOrdTeachVal;
	double		m_dCorrAlmTeachVal;
	double		m_dEatha;
	double		m_dAlpha;
	BOOL		m_isBPExecuting;
	BOOL		m_isLearnedPending;
	UINT		m_nTimerID;
	BOOL		m_bPass;
	BOOL		m_bBpnetSecPass;
	CString		m_strWkName;

	CBpOpe		m_bp;
	CBpGraph	m_graph;
	CSelGraph	m_ordGraph;
	CSelGraph	m_almGraph;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class

	int			m_nOrdArg1, m_nOrdArg2;
	int			m_nAlmArg1, m_nAlmArg2;

	BOOL		m_isCheckLearn;

// member function
public:
	void SetupCombinationList(int sel);
	void ChangeCombiSelect(CSelGraph &graph, int *idx, int sel);
	void ChangeNeuralnetFromFile(int nId);
	void SetupBpnetDlg(void);
	void SetupNetworkList(void);
	void SetupTargetValueColumn(void);
	void SetupParameterColumn(void);
	void GetTargetValue(void);
	void GetParameter(void);
	void GetTeachCondition(void);
	void EnableControl(BOOL blEnable);
	int  DispBpErrMsg(int ret);
	void OpenBpnetSecurityPass(void);
	void DispLearningTimes(void);
	void DispTeachTime(void);
	BOOL AlarmSave(void);
	BOOL OrdinarySave(void);
	BOOL CheckLearningFinish(void);
	void DispWeiLp(void);

// dialog data
public:
	enum { IDD = IDD_BPNET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnSimulate();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CStatic m_textConfName;
	CComboBox m_cmbNN;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	CColorStatic m_textPattOrd;
	CColorStatic m_textPattAlm;
	CColorStatic m_textCorrOrd;
	CColorStatic m_textCorrAlm;
	CButton m_btnPattOrd;
	CButton m_btnPattAlm;
	CButton m_btnCorrOrd;
	CButton m_btnCorrAlm;
	CButton m_btnOrdL;
	CButton m_btnOrdR;
	CButton m_btnAlmL;
	CButton m_btnAlmR;
	CEdit m_editPattOrd;
	CEdit m_editPattAlm;
	CEdit m_editCorrOrd;
	CEdit m_editCorrAlm;
	CEdit m_editEatha;
	CEdit m_editAlpha;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBpLoad();
	afx_msg void OnBpSave();
	afx_msg void OnBpInit();
	afx_msg void OnStartBp();
	afx_msg void OnStopBp();
	afx_msg void OnRecoverBp();
	CComboBox m_cmbOrd;
	CComboBox m_cmbAlm;
	CStatic m_textOrdDate;
	CStatic m_textOrdTime;
	CStatic m_textAlmDate;
	CStatic m_textAlmTime;
	CStatic m_textOrdShift;
	CStatic m_textAlmShift;
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnUpdateBpLoad(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBpSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBpInit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStartBp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopBp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRecoverBp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSimulate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTcposition(CCmdUI *pCmdUI);
	CColorButton m_btnPass;
	afx_msg void OnClickedPasswordButton();
	afx_msg void OnMove(int x, int y);
	afx_msg LRESULT OnBpnetChg(WPARAM, LPARAM);
	CEdit m_editOrdinary;
	CEdit m_editAlarm;
	CEdit m_editOrdinaryFilter;
	CEdit m_editAlarmFilter;
	afx_msg void OnTcposition();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CStatic m_labLearnTimes;
	afx_msg void OnBnClickedWeiokBtn();
	CColorStatic m_labWeiLp;
	CButton m_btnWeiOk;
};

