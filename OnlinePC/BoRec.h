#pragma once
#include "afxwin.h"


// CBoRec ダイアログ

class CBoRec : public CDialog
{
	DECLARE_DYNAMIC(CBoRec)

public:
	CBoRec(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBoRec();

// member data
public:
	BoSysInfo	m_stBsInfo;
	int			m_nSelectInfo;

// member function
public:
	void SetInfo(int no, BoSysInfo *bs);
	void GetInfo(BoSysInfo *bs);
	void DispSteelGroupInfo(void);
	void SetupConfigNameBox(void);
	void CheckRegistration(void);
	void DeleteBoRecognition(void);
	//void CheckPowder(void);
	//void DeletePowder(void);
	BOOL CheckBosysInfo(void);

// dialog data
public:
	enum { IDD = IDD_BO_RECOGNITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_textPtnNo;
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	CEdit m_editComment;
	CListBox m_listBoRec;
	CListBox m_listPowder;
	CEdit m_editThreshold;
	CEdit m_editNumTimes;
	CEdit m_editWatchTime;
	CEdit m_editAlmCombi;
	CEdit m_editAlmCtlCombi;
	CEdit m_editPowder;
	//afx_msg void OnBnClickedButton5();
	//afx_msg void OnBnClickedButton6();
	CComboBox m_cmbConfName;
	CComboBox m_cmbConfName2;
	afx_msg void OnBnClickedButton11();
};
