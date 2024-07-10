#pragma once
#include "afxwin.h"
#include "NewPass.h"


// CPassDlg dialog

class CPassDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPassDlg();

// public static member function.
public:
	// setting pass valid time (sec)
	static void SetPassTime(int nSec);
	// setting unlock time
	static void SetUnlockTime(time_t tETime);
	// password valid check
	static BOOL CheckValidPass(void);
	// static valiable
	static time_t	tUnlock;	// unlock elapse time
	static int		nTernSec;	// unlock term (sec)
	static BOOL		isUnlock;	// unlock status

// ダイアログ データ
	enum { IDD = IDD_PASS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedNewPassword();
	afx_msg void OnClickedCancel();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_editPass;
	virtual BOOL OnInitDialog();
};
