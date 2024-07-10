#pragma once
#include "afxwin.h"
#include "InfoFile.h"


// CInfoSet ダイアログ

class CInfoSet : public CDialog
{
	DECLARE_DYNAMIC(CInfoSet)

public:
	CInfoSet(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CInfoSet();

// member valiable
public:
	int			m_nNo;
	StInfoItem	m_stItem;
	int			m_nNumList;
	int			m_nIDs[INFO_ID_MAX];

// member function
public:
	void SetupCondition(void);
	BOOL StoreCondition(void);
	void SetupEdit( void );
	void SetupComboBox( void );
	void SetData( int nNo, StInfoItem &stItem );
	void GetData( StInfoItem &stItem );
	NmInfoLogicType GetIdxToElemLogic( int nSel );
	void SetIDList( int *pID, int num );

	static CString GetCondString( StInfoItem &stItem );
	static CString	GetLogicString( NmInfoLogicType nmType );

public:
// ダイアログ データ
	enum { IDD = IDD_INFO_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedC1SetBtn();
	afx_msg void OnBnClickedL1SetBtn();
	afx_msg void OnBnClickedCondClrBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCondBackBtn();
	CComboBox m_cmbC1;
	CComboBox m_cmbL1;
	CEdit m_editCond;
	CEdit m_editInfo;
	CStatic m_txtInfoNo;
};
