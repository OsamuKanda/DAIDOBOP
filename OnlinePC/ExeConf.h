#pragma once
#include "afxwin.h"


// CExeConf dialog

class CExeConf : public CDialog
{
	DECLARE_DYNAMIC(CExeConf)

public:
	CExeConf(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CExeConf();

// member data
public:
	CButton		*m_pTcButton[NumInput];
	int			m_nAsnArg;

// member funciton
public:
	void DrawCanvas(CDC &dc);
	void DrawMold(CDC&);
	void DrawMoldModel(CDC&);
	void DrawTCCombination(CDC&);
	void DrawLine(CDC &dc, int ps, int width, COLORREF col, int x1, int y1, int x2, int y2);
	void DrawInputPoints(CDC&);
	void SetupInputPoints(void);
	void SetupTCPos(void);
	BOOL StoreTCPos(void);
	void DrawTCCombiLine(CDC &dc, TCCombination *cp);
	BOOL GetADAssignPoint(int x, int y, int *arg, int *xp, int *yp);
	void DrawADAssignPoint(CDC &dc, COLORREF col, int x, int y, int size);

// dialog data
public:
	enum { IDD = IDD_EXE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	CStatic m_frameFIX;
	CStatic m_frameLOOSE;
	CStatic m_frameWEST;
	CStatic m_frameEAST;
	CStatic m_textFIX;
	CStatic m_textLOOSE;
	CStatic m_textWEST;
	CStatic m_textEAST;
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
	CButton m_tc29;
	CButton m_tc30;
	CButton m_tc31;
	CButton m_tc32;
	CButton m_tc33;
	CButton m_tc34;
	CButton m_tc35;
	CButton m_tc36;
	CButton m_tc37;
	CButton m_tc38;
	CButton m_tc39;
	CButton m_tc40;
	CButton m_tc41;
	CButton m_tc42;
	CButton m_tc43;
	CButton m_tc44;
	CButton m_tc45;
	CButton m_tc46;
	CButton m_tc47;
	CButton m_tc48;
	CButton m_tc49;
	CButton m_tc50;
	CButton m_tc51;
	CButton m_tc52;
	CButton m_tc53;
	CButton m_tc54;
	CButton m_tc55;
	CButton m_tc56;
	CButton m_tc57;
	CButton m_tc58;
	CButton m_tc59;
	CButton m_tc60;
	CButton m_tc61;
	CButton m_tc62;
	CButton m_tc63;
	CButton m_tc64;
	CButton m_tc65;
	CButton m_tc66;
	CButton m_tc67;
	CButton m_tc68;
	CButton m_tc69;
	CButton m_tc70;
	CButton m_tc71;
	CButton m_tc72;
	CButton m_tc73;
	CButton m_tc74;
	CButton m_tc75;
	CButton m_tc76;
	CButton m_tc77;
	CButton m_tc78;
	CButton m_tc79;
	CButton m_tc80;
	CButton m_tc81;
	CButton m_tc82;
	CButton m_tc83;
	CButton m_tc84;
	CButton m_tc85;
	CButton m_tc86;
	CButton m_tc87;
	CButton m_tc88;
	CButton m_tc89;
	CButton m_tc90;
	CButton m_tc91;
	CButton m_tc92;
	CButton m_tc93;
	CButton m_tc94;
	CButton m_tc95;
	CButton m_tc96;
	CStatic m_textConfName;
	CEdit m_tp01;
	CEdit m_tp02;
	CEdit m_tp03;
	CEdit m_tp04;
	CEdit m_tp05;
	CEdit m_tp06;
	CEdit m_tp07;
	CEdit m_tp08;
	CEdit m_tp09;
	CEdit m_tp10;
	CEdit m_tp11;
	CEdit m_tp12;
	CEdit m_tp13;
	CEdit m_tp14;
	CEdit m_tp15;
	CEdit m_tp16;
	CEdit m_tp17;
	CEdit m_tp18;
	CEdit m_tp19;
	CEdit m_tp20;
	CEdit m_tp21;
	CEdit m_tp22;
	CEdit m_tp23;
	CEdit m_tp24;
	CEdit m_tp25;
	CEdit m_tp26;
	CEdit m_tp27;
	CEdit m_tp28;
	CEdit m_tp29;
	CEdit m_tp30;
	CEdit m_tp31;
	CEdit m_tp32;
	afx_msg void OnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton m_btn2;
};
