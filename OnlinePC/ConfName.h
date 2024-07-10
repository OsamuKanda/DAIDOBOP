#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CConfName class
class CConfName : public CDialog
{
	DECLARE_DYNAMIC(CConfName)

public:
	CConfName(CWnd* pParent = NULL); 
	virtual ~CConfName();

// member data
public:
	CString	m_szSelectName;

// member function
public:
	void	SetupConfigNameDlg(void);
	void	StoreConfigName(void);
	void	DeleteConfigName(void);

// dialog data
public:
	enum { IDD = IDD_CONFIG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit		m_editName;
	CListBox	m_listConf;
	afx_msg void OnClickedOk();
	afx_msg void OnClickedDelete();
	afx_msg void OnClickedCancel();
	afx_msg void OnSelchangeConfigList();
};
