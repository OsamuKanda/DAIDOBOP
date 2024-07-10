#pragma once
#include "afxwin.h"

#include "ConfName.h"
#include "AdTable.h"
#include "AdChSign.h"
#include "TcName.h"
#include "CombiSign.h"
#include "NnTable.h"
#include "SkAssign.h"
#include "PassDlg.h"
#include "../lib/ColorButton.h"
#include "../lib/ColorEdit.h"

// CConfigDlg ダイアログ

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CConfigDlg();

// member data
public:
	CButton		*m_pTcButton[NumInput];
	CAdTable	*m_pDlgAdTable;
	CNnTable	*m_pDlgNnTable;
	int			m_nAsnArg;
	BOOL		m_bADAssign;
	BOOL		m_bNetDef;
	int			m_nNetDefStep;		
	int			m_nAsnPoint[8];	
	BOOL		m_bPass;
	CString		m_strAutoLoadName[NumSteelKind];
	CString		m_strAutoLoadNameD[NumSteelKind];
	BOOL		m_bAutoLoad;

// member funciton
public:
	void DrawCanvas(CDC &dc);
	void DrawMold(CDC&);
	void DrawMoldModel(CDC&);
	void DrawTCCombination(CDC&);
	void DrawLine(CDC &dc, int ps, int width, COLORREF col, int x1, int y1, int x2, int y2);
	void DrawInputPoints(CDC&);
	void SetupInputPoints(void);
	//void SetupTCPos(void);
	//BOOL StoreTCPos(void);
	void DrawTCCombiLine(CDC &dc, TCCombination *cp);
	BOOL GetADAssignPoint(int x, int y, int *arg, int *xp, int *yp);
	BOOL GetTCNameAssignPoint(int x, int y, int *arg, int *xp, int *yp);
	void DrawADAssignPoint(CDC &dc, COLORREF col, int x, int y, int size);
	BOOL ConfReset(void);
	void BopReset(LPCTSTR pBuf);
	void ManualCutoffTC(int nTcNo, BOOL bCut);
	void EnableControl(BOOL bEnable);
	void SetAutoLoad(int nPtnNo, LPCTSTR name);
	void SetAutoLoadD(int nPtnNo, LPCTSTR name);
	void LoadConfig();
	void GetTcName(void);

// dialog data
public:
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	CStatic m_frameFIX;
	CStatic m_frameLOOSE;
	CStatic m_frameWEST;
	CStatic m_frameEAST;
	CStatic m_textFIX;
	CStatic m_textLOOSE;
	CStatic m_textWEST;
	CStatic m_textEAST;
	CStatic m_textFIX2;
	CStatic m_textLOOSE2;
	CStatic m_textWEST2;
	CStatic m_textEAST2;
	CStatic m_frameMOLD;
	CStatic m_moldFIX;
	CStatic m_moldLOOSE;
	CStatic m_moldWEST;
	CStatic m_moldEAST;
	CStatic m_moldRTNO;
	CStatic m_moldLTNO;
	CStatic m_moldLBNO;
	CStatic m_moldRBNO;
	CStatic m_moldWTNO;
	CStatic m_moldETNO;
	CStatic m_moldEye;
	CButton m_tc01;
	CButton m_tc02;
	CButton m_tc03;
	CButton m_tc04;
	CButton m_tc05;
	CButton m_tc06;
	CButton m_tc07;
	CButton m_tc08;
	CButton m_tc09;
	CButton m_tc10;
	CButton m_tc11;
	CButton m_tc12;
	CButton m_tc13;
	CButton m_tc14;
	CButton m_tc15;
	CButton m_tc16;
	CButton m_tc17;
	CButton m_tc18;
	CButton m_tc19;
	CButton m_tc20;
	CButton m_tc21;
	CButton m_tc22;
	CButton m_tc23;
	CButton m_tc24;
	CButton m_tc25;
	CButton m_tc26;
	CButton m_tc27;
	CButton m_tc28;
	CButton m_sn;
	CButton m_level;
	CButton m_vc;
	CButton m_width;
//	CButton m_widthL;
//	CButton m_widthR;
	CButton m_length;
	CButton m_tdweight;
	CButton m_mdwtA;
	CButton m_mdwtC;
	CButton m_mdwtD;
	CButton m_mdtmpD;
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdchAssign();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNetDefine();
	afx_msg void OnConfigName();
	CStatic m_textConfName;
	CColorEdit m_tp01;
	CColorEdit m_tp02;
	CColorEdit m_tp03;
	CColorEdit m_tp04;
	CColorEdit m_tp05;
	CColorEdit m_tp06;
	CColorEdit m_tp07;
	CColorEdit m_tp08;
	CColorEdit m_tp09;
	CColorEdit m_tp10;
	CColorEdit m_tp11;
	CColorEdit m_tp12;
	CColorEdit m_tp13;
	CColorEdit m_tp14;
	CColorEdit m_tp15;
	CColorEdit m_tp16;
	CColorEdit m_tp17;
	CColorEdit m_tp18;
	CColorEdit m_tp19;
	CColorEdit m_tp20;
	CColorEdit m_tp21;
	CColorEdit m_tp22;
	CColorEdit m_tp23;
	CColorEdit m_tp24;
	CColorEdit m_tp25;
	CColorEdit m_tp26;
	CColorEdit m_tp27;
	CColorEdit m_tp28;
	CColorEdit m_tp29;
	CColorEdit m_tp30;
	CColorEdit m_tp31;
	CColorEdit m_tp32;
	afx_msg void OnConfigLoad();
	afx_msg void OnConfigSave();
	//afx_msg void OnRelation();
	CStatic m_textSelConfName;
	afx_msg void OnUpdateConfigName(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigLoad(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdchAssign(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNetDefine(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateRelation(CCmdUI *pCmdUI);
	CColorButton m_btnPass;
	afx_msg void OnClickedPasswordButton();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
