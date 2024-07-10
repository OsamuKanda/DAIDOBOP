#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyList.h"


// CLogSel2 ダイアログ

class CLogSel2 : public CDialog
{
	DECLARE_DYNAMIC(CLogSel2)

public:
	CLogSel2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CLogSel2();

// member data
public:
	TrendDirectory	trendDir[NumTrendFile*2];
	DataFileHeader	m_header;
	int				m_nSelectNum;
	int				m_nTrendNum;
    
// member funciton
public:
	void SetupTrendDirDlg(void);
	void SetSelectedTrendEntry(int sel);
	void PutSelectedTrendEntry(int sel);
	void DeleteTrendFile(int sel);
	void BackupFileRemove(void);
	void BackupNameSave(LPCTSTR lpszName);
	BOOL NoDialogSetup(int sel);
	int GetListCurSel(CListCtrl &list);
	void SetTrendDataFromDrv(LPCTSTR lpszDir, int nHead);
	void SetTrendDataFromDrv2(LPCTSTR lpszDir, int nPos);
	void SetTrendEntryInfo(int sel, LPCTSTR lpszDir, LPCTSTR lpszName);

// dialog gata
public:
	enum { IDD = IDD_LOG_SEL2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_textStTime;
	CStatic m_textTotalMin;
	CEdit m_editRemark;
	afx_msg void OnClickedOk();
	afx_msg void OnClickedRecord();
	afx_msg void OnClickedDelete();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	CMyList m_listLog2;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
