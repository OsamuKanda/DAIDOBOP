#pragma once
#include "afxwin.h"
#include "BoRec.h"
#include "PassDlg.h"
#include "TcVwDlg.h"
#include "../lib/ColorButton.h"
#include "../lib/ColorEdit.h"
#include "afxcmn.h"


/////////////////////////////////////////////////////////////////////////////////////////
// definition
#define	NUM_PTN_NO		9
#define MAX_PAGE		11

// CBosysDlg dialog
class CBosysDlg : public CDialog
{
	DECLARE_DYNAMIC(CBosysDlg)

public:
	CBosysDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBosysDlg();

// member data
public:
	BoCondInfo	m_stCondInfo;
	BoNetInfo	m_stNetInfo;
	StTcExpSet	m_stTcExpInfo;
	BoSysInfo	m_stBsInfo[NumSteelKind];
	int			m_nSamplePeriod;
	BOOL		m_bPass;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class
	int			m_page;

// member function
public:
	void SetupBosysCondition(void);
	void DoPatternNo(int no);
	void DispBosysInfo(void);
	void DispBosysInfoMem(void);
	void StoreBosysInfo(int no);
	void ClearBosysInfo(int no);
	BOOL CheckBosysInfo(void);
	BOOL SetBosysInfo(int no);
	BOOL SetBosysInfo2(int no);
	void EnableControl(BOOL bEnable);

// dialog data
public:
	enum { IDD = IDD_BOSYS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLoadData();
	afx_msg void OnSaveData();
	CButton m_rdoSK01;
	CButton m_rdoSK02;
	CButton m_rdoSK03;
	CButton m_rdoSK04;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClickedRadio1();
	afx_msg void OnClickedRadio2();
	afx_msg void OnClickedRadio3();
	afx_msg void OnClickedRadio4();
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	CColorEdit m_textSamplePeriod;
	CColorEdit m_editComment[NUM_PTN_NO];
	CColorEdit m_editWatchTime[NUM_PTN_NO];
	CListBox m_listBoRec[NUM_PTN_NO];
	CListBox m_listBoPow[NUM_PTN_NO];
	CColorEdit m_editAlmCombi[NUM_PTN_NO];
	CColorEdit m_editAlmCtlCombi[NUM_PTN_NO];
	CColorEdit m_editConfName[NUM_PTN_NO];
	CColorEdit m_editConfNameD[NUM_PTN_NO];
	CButton m_btnGradeNo[NUM_PTN_NO];
	afx_msg void OnDoubleclickedButton1();
	afx_msg void OnDoubleclickedButton2();
	afx_msg void OnDoubleclickedButton3();
	afx_msg void OnDoubleclickedButton4();
	afx_msg void OnDoubleclickedButton5();
	afx_msg void OnDoubleclickedButton6();
	afx_msg void OnDoubleclickedButton7();
	afx_msg void OnDoubleclickedButton8();
	afx_msg void OnDoubleclickedButton9();
	afx_msg void OnClickedProtectButton();
	CColorButton m_btnPass;
	afx_msg void OnUpdateLoadData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSaveData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTcposition(CCmdUI *pCmdUI);
	afx_msg void OnMove(int x, int y);
	CEdit m_editCondLen;
	CEdit m_editCondVc;
	afx_msg void OnTcposition();
	CIPAddressCtrl m_ipPlc;
	CEdit m_editAutoTcPrm1;
	CEdit m_editAutoTcPrm2;
	CEdit m_editAutoTcPrm3;
	CEdit m_editAutoTcPrm4;
	CEdit m_editAutoTcPrm5;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton13();
	CStatic m_txtPage;
	CButton m_btnNext;
	CButton m_btnPrev;
};
