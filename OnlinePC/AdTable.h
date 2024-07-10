#pragma once
#include "afxwin.h"


// CAdTable �_�C�A���O

class CAdTable : public CDialog
{
	DECLARE_DYNAMIC(CAdTable)

public:
	CAdTable(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CAdTable();

// user function
public:
	void DrawADAssign(CDC &dc);
	LPCTSTR GetADAssignedName(int ch);

// �_�C�A���O �f�[�^
public:
	enum { IDD = IDD_ADTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedClose();
	afx_msg void OnPaint();
	CStatic m_frameAD;
};
