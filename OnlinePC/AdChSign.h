#pragma once
#include "afxwin.h"


// CAdChSign ダイアログ

class CAdChSign : public CDialog
{
	DECLARE_DYNAMIC(CAdChSign)

public:
	CAdChSign(int nAnsArg, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAdChSign();

// member function
public:
	int		m_nAsnArg;

// member function
	void SetupADAssignFactor(int arg);
	void StoreADAssignment(int arg);

// dialog data
public:
	enum { IDD = IDD_ADCH_SIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	CEdit m_editChNo;
	CEdit m_editMinVal;
	CEdit m_editMaxVal;
	CEdit m_editUnit;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDelete();
};
