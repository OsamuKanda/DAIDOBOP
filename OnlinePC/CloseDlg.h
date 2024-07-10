#pragma once


// CCloseDlg ダイアログ

class CCloseDlg : public CDialog
{
	DECLARE_DYNAMIC(CCloseDlg)

public:
	CCloseDlg(CDialog* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCloseDlg();

protected:
	CDialog		*m_pParent;

public:
// ダイアログ データ
	enum { IDD = IDD_CLOSE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButton1();
};
