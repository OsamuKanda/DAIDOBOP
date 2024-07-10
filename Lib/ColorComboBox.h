// ==============================================================================================================================================
// 
//  ColorComboBoxクラス ( CColorComboBox )	
//	※ MFCベースCComboBoxクラスの派生クラスです。
//  Version      :  1.0
//  FileName     :	ColorComboBox.h
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/10/19  1.0			K.Ohnishi	新規
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _TEXT_MARGIN
#define	_TEXT_MARGIN	0
#endif
/////////////////////////////////////////////////////////////////////////////
// CColorComboBox ウィンドウ
class CColorComboBox : public CComboBox
{
// コンストラクション
public:
	//	コンストラクタ
	CColorComboBox();	
	//	テキスト色設定
	virtual BOOL SetTextColor(int nIndex, COLORREF dwTextColor, BOOL bRedraw = TRUE);	
	//	背景色設定
	virtual BOOL SetBackColor(int nIndex, COLORREF dwBackColor, BOOL bRedraw = TRUE);	
	//	表示フォント設定
	virtual BOOL SetFontInfo(int nSize = 12,									
							 BOOL bBold = FALSE,
							 BOOL bItalic = FALSE,
							 LPCTSTR pFontName = _T("MS Shell Dlg") );
	//	最終行へ追加
	virtual int AddItem(LPCTSTR lpszItem,
						COLORREF clrBack,
						COLORREF clrText,
						UINT uFlag = DT_LEFT | DT_VCENTER,
						DWORD dwData = NULL	);
	//	指定行へ追加
	virtual int InsertItem(int	nIndex,	
						   LPCTSTR lpszItem,						  			
						   COLORREF clrBack,
						   COLORREF clrText,
						   UINT uFlag = DT_LEFT | DT_VCENTER,
						   DWORD dwData = NULL	);
	//	指定行削除 
	BOOL DeleteString(int nIndex);
	//	全行削除
	void ResetContent();
	//	選択行ハイライト表示　有効/無効
	void EnableHilight(BOOL nEnable = TRUE);
	//	デストラクタ
	virtual ~CColorComboBox();	
	//	行高さ指定
	int SetItemHeight(int h)
	{
		int oldH = m_iItemHeight;
		m_iItemHeight = h;
		return oldH;
	}
	//{{AFX_VIRTUAL(CColorComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	struct LItem
	{
		COLORREF	cBg;
		COLORREF	cFg;
		UINT		uFlag;
		DWORD		dwData;
	};

	int		Ix(int ix);
	LItem*	Item(int ix);
	//	メンバ変数
	int			m_iItemHeight;
	CFont		m_Font;
	BOOL		m_bHilight;
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CColorComboBox)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

