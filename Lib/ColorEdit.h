// ==============================================================================================================================================
// 
//  ColorEdit�N���X ( CColorEdit )	
//	�� MFC�x�[�XCEdit�N���X�̔h���N���X�ł��B
//  Version      :  2.0
//  FileName     :	ColorEdit.h
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/06/01  1.0			K.Ohnishi	�V�K
//				 : 2004/10/15  2.0			K.Ohnishi	�\�[�X����
// ==============================================================================================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorEdit �E�B���h�E
class CColorEdit : public CEdit
{
// �R���X�g���N�V����
public:
	//	�R���X�g���N�^
	CColorEdit();
	//	�e�L�X�g�F�ݒ�
	virtual COLORREF SetTextColor(COLORREF dwTextColor, BOOL bRedraw = TRUE);
	//	�w�i�F�ݒ�
	virtual COLORREF SetBackColor(COLORREF dwBackColor, BOOL bRedraw = TRUE);	
	//	�\���t�H���g�ݒ�
	virtual BOOL SetFontInfo(int nSize = 12,									
							 BOOL bBold = FALSE,
							 BOOL bItalic = FALSE,
							 LPCTSTR pFontName = _T("MS UI Gothic") );
	//	�e�L�X�g�_�ŏ��� �J�n
	virtual BOOL BlinkOn( UINT uTime,			
						  COLORREF dwColor1,
						  COLORREF dwColor2 );
	//	�e�L�X�g�_�ŏ��� ��~
	virtual void BlinkOff();
	//	�f�X�g���N�^
	virtual ~CColorEdit();	
protected:
	//	�����o�ϐ�
	CBrush		m_brBkgnd;
	COLORREF	m_clrText;
	COLORREF	m_clrBkgnd;
	CFont		m_Font;
	UINT		m_uTimerID;
	BOOL		m_bBlinkFlag;
	COLORREF	m_dwOldBkColor;
	COLORREF	m_clrBlinkColor1;
	COLORREF	m_clrBlinkColor2;
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

