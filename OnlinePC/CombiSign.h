#pragma once
#include "afxwin.h"


// CCombiSign class

class CCombiSign : public CDialog
{
	DECLARE_DYNAMIC(CCombiSign)

public:
	CCombiSign(int nCombiUp, int nCombiLo, CWnd* pParent = NULL); 
	virtual ~CCombiSign();

// member data
public:
	int 	m_nCombiUp;
	int		m_nCombiLo;

// member function
public:
	void SetupCombinationDlg(int *upper, int *lower);
	void SortCombiPoint(int *pt0, int *pt1);
	void AppendCombination(int upper, int lower);
	void DeleteCombination(int upper, int lower);

// dialog data
public:
	enum { IDD = IDD_COMBI_SIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedCombiAppend();
	afx_msg void OnClickedCombiDel();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	CStatic m_textCombiUp;
	CStatic m_textCombiLo;
	CEdit m_editCombiNet;
};
