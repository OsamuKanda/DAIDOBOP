#pragma once
#include "afxwin.h"
#include "CondFile.h"


// CCondSet ダイアログ

class CCondSet : public CDialog
{
	DECLARE_DYNAMIC(CCondSet)

public:
	CCondSet(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCondSet();

// member valiable
public:
	int			m_nNo;
	StCondItem	m_stItem;

// member function
public:
	void SetupCondition(void);
	BOOL StoreCondition(void);
	void SetupEdit( void );
	void SetupComboBox( void );
	void SetData( int nNo, StCondItem &stItem );
	void GetData( StCondItem &stItem );
	NmCondElemType GetIdxToElemTemp( int nSel );
	NmCondElemType GetIdxToElemCalc( int nSel );
	NmCondElemType GetIdxToElemLRO( int nSel );
	NmCondLogicType GetIdxToElemLogic( int nSel );
	CString GetFormulaString( void );
	
	static CString GetCondString( StCondItem &stItem );
	static CString GetTypeString( NmCondElemType nmType );
	static CString	GetLogicString( NmCondLogicType nmType );



public:
// ダイアログ データ
	enum { IDD = IDD_COND_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedF1SetBtn();
	afx_msg void OnBnClickedF4SetBtn();
	afx_msg void OnBnClickedL1SetBtn();
	afx_msg void OnBnClickedCondClrBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_cmbF1;
	CComboBox m_cmbF2;
	CComboBox m_cmbF3;
	CComboBox m_cmbF4;
	CComboBox m_cmbF5;
	CComboBox m_cmbF6;
	CComboBox m_cmbF7;
	CComboBox m_cmbF8;
	CComboBox m_cmbF9;
	CComboBox m_cmbL1;
	CButton m_rdoF1;
	CButton m_rdoF2;
	CButton m_rdoF3;
	CButton m_rdoF4;
	CButton m_rdoF5;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCondBackBtn();
	CEdit m_editCond;
	CEdit m_editFormula;
	CEdit m_editTime;
	CEdit m_editRemark;
	CEdit m_editInVal;
	CStatic m_txtCondNo;
	afx_msg void OnCbnSelchangeCmboF1();
	afx_msg void OnCbnSelchangeCmboF2();
	afx_msg void OnCbnSelchangeCmboF3();
	afx_msg void OnCbnSelchangeCmboF5();
	afx_msg void OnCbnSelchangeCmboF6();
	afx_msg void OnCbnSelchangeCmboF7();
	afx_msg void OnCbnSelchangeCmboF8();
	afx_msg void OnBnClickedRdoF4();
	afx_msg void OnBnClickedRdoF5();
	afx_msg void OnEnChangeEditInval();
};
