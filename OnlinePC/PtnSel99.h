#pragma once
#include "afxcmn.h"
#include "config.h"


// CPtnSel99 ダイアログ

class CPtnSel99 : public CDialog
{
	DECLARE_DYNAMIC(CPtnSel99)

public:
	CPtnSel99(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPtnSel99();

	void SetParam(CConfig &config, int pno);
	void GetParam(int *pno);
	int GetCurSel(CListCtrl& list);

// メンバ変数
public:
	int				m_nPtnNo;
	CImageList		m_image;					// 高さを調整する為のイメージリスト
		
public:
// ダイアログ データ
	enum { IDD = IDD_PTN_SEL99 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_listGC;
	virtual BOOL OnInitDialog();
};
