// TcVwDlg.h : TC View Screen header file
//
#if !defined(_TCVIEWDLG_H_)
#define _SPEANADLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// TC View Screen

//----------------
// 定数
//----------------


//----------------
// クラス定義
//----------------
class CTcVwDlg : public CDialog
{
// コンストラクション
public:
	CTcVwDlg(CWnd* pParent = NULL);			// 標準のコンストラクタ

// メンバ変数（外部非公開）
protected:
	CFont		m_fontPointInfo;
	CDC			m_mdc;
	CBitmap		m_bitmap;
	BOOL		m_bFirst;

// メソッド（外部公開）
public:
	void SetData(int ch, double data1, double data2);			
												// setting data
	void createTcViewFrame(CDC &dc);			// create TC view frame
	void drawSermo(CDC &dc);					// draw sermo view

// ダイアログ データ
public:
	//{{AFX_DATA(CTcVwDlg)
	enum { IDD = IDD_TC_VIEW_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTcVwDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTcVwDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(_SPEANADLG_H_)
