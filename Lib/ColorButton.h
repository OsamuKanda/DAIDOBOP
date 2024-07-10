// ==============================================================================================================================================
// 
//  ColorButtonクラス ( CColorButton )	
//	※ MFCベースCButtonクラスの派生クラスです。
//  Version      :  2.0
//  FileName     :	ColorButton.h
//
//	History      :
//               :  Date       Ver.			Auther     Revision Item 
//				 : ----------- ---------	---------- ----------------------------- 
//				 : 2003/12/19  1.3.12.19	K.Ohnishi   新規
//				 : 2004/10/15  2.0			K.Ohnishi	ソース整理
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorButton ウィンドウ
class CColorButton : public CButton
{
// コンストラクション
public:
	//	コンストラクタ
	CColorButton();
	//	テキスト色設定
	virtual void SetTextColor(COLORREF dwTextColor);	
	//	背景色設定
	virtual void SetBackColor(COLORREF dwBackColor);	
	//	表示フォント設定
	virtual BOOL SetFontInfo(int nSize = 12,									
							 BOOL bBold = FALSE,
							 BOOL bItalic = FALSE,
							 LPCTSTR pFontName = _T("ＭＳ ゴシック") );
	//	テキスト点滅処理 開始
	virtual BOOL BlinkOn( UINT uTime,			
						  COLORREF dwColor1,
						  COLORREF dwColor2 );
	//	テキスト点滅処理 停止
	virtual void BlinkOff();
	//	オーナー描画領域設定
	virtual void SetDrawMargin(CRect& mRc);
	//	デストラクタ
	virtual ~CColorButton();
protected:
	//	メンバ変数
	COLORREF	m_clrBkgnd;
	COLORREF	m_clrText;
    CRect		m_innerRc; 
	CRect		m_rcMargin;
	CFont		m_Font;
	UINT		m_uTimerID;
	BOOL		m_bBlinkFlag;
	COLORREF	m_dwOldBkColor;
	COLORREF	m_dwBlinkColor1;
	COLORREF	m_dwBlinkColor2;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CColorButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CColorButton)
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

