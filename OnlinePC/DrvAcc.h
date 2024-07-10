#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyList.h"

// CDrvAcc dialog
class CDrvAcc : public CDialog
{
	DECLARE_DYNAMIC(CDrvAcc)

public:
	CDrvAcc(CWnd* pParent = NULL); 
	virtual ~CDrvAcc();
	
// member data.
protected:
	TrendDirectory	m_trendDir[NumTrendFile*2];
	int				m_numTrendFile;
	BOOL			m_bAcc;
	BOOL			m_isLocal;

// member function.
protected:
	void SetTrendDataFromDrv(LPCTSTR lpszDir, int nHead);
	void SetTrendDataFromDrv2(LPCTSTR lpszDir, int nPos);
	int GetListCurSel(CListCtrl &list);
	void SetTrendEntryInfo(int sel, LPCTSTR lpszDir, LPCTSTR lpszName);
	void CopySelectFile(int nWay);

// dialog data.
public:
	enum { IDD = IDD_DRIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedToLocal();
	afx_msg void OnBnClickedToDrive();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	virtual BOOL OnInitDialog();
	CMyList m_list;
	CComboBox m_cmb1;
	CButton m_rdo1;
	CButton m_rdo2;
	CButton m_btn1;
	CButton m_btn2;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnEndScrollList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};

