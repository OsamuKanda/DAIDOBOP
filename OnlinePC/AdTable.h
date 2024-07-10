#pragma once
#include "afxwin.h"


// CAdTable ダイアログ

class CAdTable : public CDialog
{
	DECLARE_DYNAMIC(CAdTable)

public:
	CAdTable(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAdTable();

// user function
public:
	void DrawADAssign(CDC &dc);
	LPCTSTR GetADAssignedName(int ch);

// ダイアログ データ
public:
	enum { IDD = IDD_ADTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedClose();
	afx_msg void OnPaint();
	CStatic m_frameAD;
};
