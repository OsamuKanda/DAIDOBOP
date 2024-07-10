// ==============================================================================================================================================
// 
//  ColorButton�N���X ( CColorButton )	
//	�� MFC�x�[�XCButton�N���X�̔h���N���X�ł��B
//  Version      :  2.0
//  FileName     :	ColorButton.h
//
//	History      :
//               :  Date       Ver.			Auther     Revision Item 
//				 : ----------- ---------	---------- ----------------------------- 
//				 : 2003/12/19  1.3.12.19	K.Ohnishi   �V�K
//				 : 2004/10/15  2.0			K.Ohnishi	�\�[�X����
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorButton �E�B���h�E
class CColorButton : public CButton
{
// �R���X�g���N�V����
public:
	//	�R���X�g���N�^
	CColorButton();
	//	�e�L�X�g�F�ݒ�
	virtual void SetTextColor(COLORREF dwTextColor);	
	//	�w�i�F�ݒ�
	virtual void SetBackColor(COLORREF dwBackColor);	
	//	�\���t�H���g�ݒ�
	virtual BOOL SetFontInfo(int nSize = 12,									
							 BOOL bBold = FALSE,
							 BOOL bItalic = FALSE,
							 LPCTSTR pFontName = _T("�l�r �S�V�b�N") );
	//	�e�L�X�g�_�ŏ��� �J�n
	virtual BOOL BlinkOn( UINT uTime,			
						  COLORREF dwColor1,
						  COLORREF dwColor2 );
	//	�e�L�X�g�_�ŏ��� ��~
	virtual void BlinkOff();
	//	�I�[�i�[�`��̈�ݒ�
	virtual void SetDrawMargin(CRect& mRc);
	//	�f�X�g���N�^
	virtual ~CColorButton();
protected:
	//	�����o�ϐ�
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
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CColorButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CColorButton)
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

