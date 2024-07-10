#pragma once
#include "afxwin.h"


// CLogSel ダイアログ

class CLogSel : public CDialog
{
	DECLARE_DYNAMIC(CLogSel)

public:
	CLogSel(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CLogSel();

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

// dialog gata
public:
	enum { IDD = IDD_LOG_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_textStTime;
	CStatic m_textTotalMin;
	CEdit m_editRemark;
	CListBox m_listLog;
	afx_msg void OnClickedOk();
	afx_msg void OnClickedRecord();
	afx_msg void OnClickedDelete();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLogList();
};
