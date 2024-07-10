// ==============================================================================================================================================
// 
//  ColorComboBox�N���X ( CColorComboBox )	
//	�� MFC�x�[�XCComboBox�N���X�̔h���N���X�ł��B
//  Version      :  1.0
//  FileName     :	ColorComboBox.h
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/10/19  1.0			K.Ohnishi	�V�K
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _TEXT_MARGIN
#define	_TEXT_MARGIN	0
#endif
/////////////////////////////////////////////////////////////////////////////
// CColorComboBox �E�B���h�E
class CColorComboBox : public CComboBox
{
// �R���X�g���N�V����
public:
	//	�R���X�g���N�^
	CColorComboBox();	
	//	�e�L�X�g�F�ݒ�
	virtual BOOL SetTextColor(int nIndex, COLORREF dwTextColor, BOOL bRedraw = TRUE);	
	//	�w�i�F�ݒ�
	virtual BOOL SetBackColor(int nIndex, COLORREF dwBackColor, BOOL bRedraw = TRUE);	
	//	�\���t�H���g�ݒ�
	virtual BOOL SetFontInfo(int nSize = 12,									
							 BOOL bBold = FALSE,
							 BOOL bItalic = FALSE,
							 LPCTSTR pFontName = _T("MS Shell Dlg") );
	//	�ŏI�s�֒ǉ�
	virtual int AddItem(LPCTSTR lpszItem,
						COLORREF clrBack,
						COLORREF clrText,
						UINT uFlag = DT_LEFT | DT_VCENTER,
						DWORD dwData = NULL	);
	//	�w��s�֒ǉ�
	virtual int InsertItem(int	nIndex,	
						   LPCTSTR lpszItem,						  			
						   COLORREF clrBack,
						   COLORREF clrText,
						   UINT uFlag = DT_LEFT | DT_VCENTER,
						   DWORD dwData = NULL	);
	//	�w��s�폜 
	BOOL DeleteString(int nIndex);
	//	�S�s�폜
	void ResetContent();
	//	�I���s�n�C���C�g�\���@�L��/����
	void EnableHilight(BOOL nEnable = TRUE);
	//	�f�X�g���N�^
	virtual ~CColorComboBox();	
	//	�s�����w��
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
	//	�����o�ϐ�
	int			m_iItemHeight;
	CFont		m_Font;
	BOOL		m_bHilight;
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CColorComboBox)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

