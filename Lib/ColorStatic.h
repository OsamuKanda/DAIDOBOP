// ==============================================================================================================================================
// 
//  ColorStatic�N���X ( CColorStatic )	
//	�� MFC�x�[�XCStatic�N���X�̔h���N���X�ł��B
//  Version      :  2.0
//  FileName     :	ColorStatic.h
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2003/10/16  1.3.10.16	K.Ohnishi	�V�K
//				 : 2004/10/15  2.0			K.Ohnishi	�\�[�X����
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorStatic �E�B���h�E
class CColorStatic : public CStatic
{
// �R���X�g���N�V����
public:
	//	�R���X�g���N�^
	CColorStatic();	
	//	�e�L�X�g�F�ݒ�
	virtual COLORREF SetTextColor(COLORREF dwTextColor, BOOL bRedraw = TRUE);	
	//	�w�i�F�ݒ�
	virtual COLORREF SetBackColor(COLORREF dwBackColor, BOOL bRedraw = TRUE);	
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
	//	�f�X�g���N�^
	virtual ~CColorStatic();	
protected:
	//	�����o�ϐ�
	CBrush		m_brBkgnd;
	COLORREF	m_clrText;
	COLORREF	m_clrBkgnd;
	CFont		m_Font;
	UINT		m_uTimerID;
	BOOL		m_bBlinkFlag;
	COLORREF	m_clrOldBkColor;
	COLORREF	m_clrBlinkColor1;
	COLORREF	m_clrBlinkColor2;
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CColorStatic)
protected:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

