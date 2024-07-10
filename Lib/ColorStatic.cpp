// ==============================================================================================================================================
// 
//  ColorStatic�N���X ( CColorStatic )	
//	�� MFC�x�[�XCStatic�N���X�̔h���N���X�ł��B
//  Version      :  2.0
//  FileName     :	ColorStatic.cpp
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2003/10/16  1.0			K.Ohnishi	�V�K
//				 : 2004/10/15  2.0			K.Ohnishi	�\�[�X����
// ==============================================================================================================================================

#include <afxwin.h>
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic
CColorStatic::CColorStatic()
{
	//	������
	m_clrText		= RGB(0, 0, 0);
	m_clrBkgnd		= ::GetSysColor( COLOR_BTNFACE );
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
	m_uTimerID		= 0;
	m_bBlinkFlag	= FALSE;
	m_clrBlinkColor1 = ::GetSysColor( COLOR_BTNFACE );
	m_clrBlinkColor2 = ::GetSysColor( COLOR_BTNFACE );
}

CColorStatic::~CColorStatic()
{

}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic ���b�Z�[�W �n���h��

// ============================================================================
//     �֐���: CtlColor
//       ���e: �`�揈��
//		 ����: 
//     �߂�l: �w�i�u���V			
// ============================================================================
HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// �e�L�X�g
	pDC->SetTextColor( m_clrText );					
	// �e�L�X�g�̔w�i
	pDC->SetBkColor( m_clrBkgnd );					
	// �R���g���[���̔w�i
	return m_brBkgnd;	
}

// ============================================================================
//     �֐���: OnTimer
//       ���e: �^�C�}����
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorStatic::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_uTimerID )	
	{
		if( m_bBlinkFlag )
		{
			//	�_�ŐF1
			SetBackColor( m_clrBlinkColor1, TRUE );
			m_bBlinkFlag = FALSE;
		}
		else
		{
			//	�_�ŐF2
			SetBackColor( m_clrBlinkColor2, TRUE );
			m_bBlinkFlag = TRUE;
		}
	}
	CStatic::OnTimer(nIDEvent);
}

// ============================================================================
//     �֐���: OnDestroy
//       ���e: 
//		 ����: 
//     �߂�l: BOOL			
// ============================================================================
void CColorStatic::OnDestroy()
{
	if( m_uTimerID != 0 )
		KillTimer( m_uTimerID );
	CStatic::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CColorStatic �ǉ��֐�

// ============================================================================
//     �֐���: SetBackColor
//       ���e: �w�i�F�ݒ�
//		 ����: COLORREF		�J���[�R�[�h	��FRGB(255,255,255)
//			 : BOOL			�ĕ`��			TRUE - ����	FALSE - ���Ȃ�  
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
COLORREF CColorStatic::SetBackColor(COLORREF clrBackColor, BOOL bRedraw)
{
	COLORREF color = m_clrBkgnd;

	m_clrBkgnd = clrBackColor;
	m_clrOldBkColor = m_clrBkgnd;
	if (bRedraw)
	{
		m_brBkgnd.DeleteObject();
		//	�u���V�쐬
		m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
		//	�ĕ`��
		Invalidate();								
	}
	//	�ύX����O�̔w�i�F��Ԃ�
	return color;									
}
// ============================================================================
//     �֐���: SetTextColor
//       ���e: �e�L�X�g�F�ݒ�
//		 ����: COLORREF		�J���[�R�[�h	��FRGB(255,255,255)
//			 : BOOL			�ĕ`��			TRUE - ����	FALSE - ���Ȃ�  
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
COLORREF CColorStatic::SetTextColor(COLORREF clrTextColor, BOOL bRedraw)
{
	COLORREF color = m_clrText;

	m_clrText = clrTextColor;
	if (bRedraw)
		//	�ĕ`��
		Invalidate();								
	//	�ύX����O�̑O�i�F��Ԃ�
	return color;									
}
// ============================================================================
//     �֐���: SetFontInfo
//       ���e: �t�H���g�쐬
//		 ����: int			�t�H���g����
//			 : BOOL			�t�H���g����	TRUE - �{�[���h	FALSE - �ʏ�  
//			 : BOOL			�t�H���g�Α�	TRUE - �{�[���h FALSE - �ʏ�
//			 : LPCTSTR		�t�H���g��	
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorStatic::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
{
	int		nWeight;

	//	�I�u�W�F�N�g�폜
	m_Font.DeleteObject();
	//	����
	if(bBold)
		nWeight = FW_BOLD;
	else
		nWeight = FW_NORMAL;
	//	�t�H���g�쐬
	if(!m_Font.CreateFont( nSize,					// �t�H���g�̍����i�O�̎��́A�����I�ɒ��߂����j
						0,							// �t�H���g�̕��i�O�̎��́A�����I�ɒ��߂����j
						0,							// �X�Ε����ƃf�B�X�v���C�ʂ� x ���Ƃ̊p�x 
						0,							// �����̊���� x ���Ƃ̊p�x
						nWeight,					// �����̑���(�W��)
						bItalic,					// �C�^���b�N
						FALSE,						// ����
						FALSE,						// ��������
						DEFAULT_CHARSET,			// �����Z�b�g
						OUT_DEFAULT_PRECIS,			// �o�͐��x
						CLIP_DEFAULT_PRECIS,		// �N���b�s���O���x
						ANTIALIASED_QUALITY,		// �o�͕i��
						DEFAULT_PITCH | FF_SWISS,	// �s�b�`�ƃt�@�~��
						pFontName					// �^�C�v�t�F�C�X��
						) ) 
		return FALSE;
	//	�t�H���g�ݒ�
	SetFont(&m_Font);		
	return TRUE;
}
// ============================================================================
//     �֐���: BlinkOn
//       ���e: �e�L�X�g�_�ŏ���	�J�n
//		 ����: UINT			�_�ŊԊu(msec�w��)	1000msec = 1sec  
//			 : COLORREF		�_�ŐF�P	��jRGB(255,255,255) - ��
//			 : COLORREF		�_�ŐF�Q	��jRGB(255,0,0) - ��
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorStatic::BlinkOn(UINT uTime, COLORREF dwColor1, COLORREF dwColor2)
{
	//	�w��F����
	if( dwColor1 < RGB(0,0,0) || dwColor1 > RGB(255,255,255) )
		return FALSE;
	if( dwColor2 < RGB(0,0,0) || dwColor2 > RGB(255,255,255) )
		return FALSE;

	m_clrBlinkColor1 = dwColor1;
	m_clrBlinkColor2 = dwColor2;
	//	���ݔw�i�F�ێ�
	m_clrOldBkColor = m_clrBkgnd;
	BlinkOff();
	//	�^�C�}�J�n
	if( 0 == (m_uTimerID = SetTimer( 1, uTime, NULL ) ) )		
		return FALSE;

	return TRUE;
}
// ============================================================================
//     �֐���: BlinkOff
//       ���e: �e�L�X�g�_�ŏ��� ��~
//		 ����: 
//     �߂�l: 
// ============================================================================
void CColorStatic::BlinkOff()
{
	//	�^�C�}��~
	KillTimer( m_uTimerID );
	//	�w�i�F�߂�
	SetBackColor( m_clrOldBkColor, TRUE );
}