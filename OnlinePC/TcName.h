#pragma once
#include "../Include/Bodef.h"
#include "afxwin.h"

// CTcName ダイアログ

class CTcName : public CDialog
{
	DECLARE_DYNAMIC(CTcName)

public:
	CTcName(int nAnsArg, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTcName();

// ダイアログ データ
	enum { IDD = IDD_TC_NAME };

// member function
public:
	int		m_nAsnArg;
	TcName	m_stTcNm;

	_TCHAR	m_cRootDir[_MAX_PATH];

public:
	BOOL ReadTcNameFile(TcName *tcnm);
	BOOL WriteTcNameFile(TcName *tcnm);
	void SetDefaultTCName(TcName *tcnm);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_editTcName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
