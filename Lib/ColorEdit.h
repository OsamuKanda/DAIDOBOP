// ==============================================================================================================================================
// 
//  ColorEditクラス ( CColorEdit )	
//	※ MFCベースCEditクラスの派生クラスです。
//  Version      :  2.0
//  FileName     :	ColorEdit.h
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/06/01  1.0			K.Ohnishi	新規
//				 : 2004/10/15  2.0			K.Ohnishi	ソース整理
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorEdit ウィンドウ
class CColorEdit : public CEdit
{
// コンストラクション
public:
	//	コンストラクタ
	CColorEdit();
	//	テキスト色設定
	virtual COLORREF SetTextColor(COLORREF dwTextColor, BOOL bRedraw = TRUE);
	//	背景色設定
	virtual COLORREF SetBackColor(COLORREF dwBackColor, BOOL bRedraw = TRUE);	
	//	表示フォント設定
	virtual BOOL SetFontInfo(int nSize = 12,									
							 BOOL bBold = FALSE,
							 BOOL bItalic = FALSE,
							 LPCTSTR pFontName = _T("MS UI Gothic") );
	//	テキスト点滅処理 開始
	virtual BOOL BlinkOn( UINT uTime,			
						  COLORREF dwColor1,
						  COLORREF dwColor2 );
	//	テキスト点滅処理 停止
	virtual void BlinkOff();
	//	デストラクタ
	virtual ~CColorEdit();	
protected:
	//	メンバ変数
	CBrush		m_brBkgnd;
	COLORREF	m_clrText;
	COLORREF	m_clrBkgnd;
	CFont		m_Font;
	UINT		m_uTimerID;
	BOOL		m_bBlinkFlag;
	COLORREF	m_dwOldBkColor;
	COLORREF	m_clrBlinkColor1;
	COLORREF	m_clrBlinkColor2;
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CColorEdit)
protected:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

