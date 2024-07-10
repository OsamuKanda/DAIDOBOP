// MsgWnd.cpp : �C���v�������e�[�V���� �t�@�C��
//
#include "../include/MsgWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �t�@�C���^�C���̉��Z
FILETIME FT_Add(FILETIME ft1, FILETIME ft2);
// �H�����̃f�[�^�擾
FILETIME FT_Day(INT nCount);
// �t�@�C���^�C���̔�r
int FT_Comp(FILETIME ft1, FILETIME ft2);

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd

CMsgWnd::CMsgWnd()
{
	int	i;

	m_dcWin = NULL;
	m_bmpWin = NULL;
	m_nNumRow = NumRow;

	// ���b�Z�[�W�o�b�t�@�|�C���^�̏�����
	m_msgHeadLine	= m_msgTailLine	= m_msgDispLine	= 0;
	m_msgNumLine	= 0;
	m_msgColumn		= 0;

	// ���b�Z�[�W�o�b�t�@�̏�����
	for (i = 0; i < MsgMaxRow; i++){
		*m_msgBuf[i]	= '\0';
		m_msgCol[i] = RGB(0,0,0);
	}

	// �\���p�t�H���g�̍쐬
	m_cFont.CreateFont(	-9, 0, 0, 0, 0, 0, 0, 0,
						SHIFTJIS_CHARSET, 0, 0, 0, 0, "FixedSys");

	//-----------------
	// ���O�֌W������
	//-----------------
	memset(m_szLogPath, 0, sizeof(m_szLogPath));
	strcpy(m_szLogPath, "..\\Log");
	m_nDays = 0;
	m_bLog = false;
}

CMsgWnd::~CMsgWnd()
{
	// �I�����Ƀ��O��ۑ�
	SaveLogFile();
	// �t�H���g�̍폜
	m_cFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMsgWnd, CWnd)
	//{{AFX_MSG_MAP(CMsgWnd)
	ON_MESSAGE( WM_APPENDMSG, OnAppendMsg )
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMsgWnd ���b�Z�[�W �n���h��

int CMsgWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// �X�N���[���o�[�̃����W�ݒ�
	SetScrollRange(SB_VERT, 0, MsgMaxRow - m_nNumRow - 1, FALSE);

	return 0;
}

void CMsgWnd::PostNcDestroy() 
{
	if( m_bmpWin != NULL )
		delete m_bmpWin;
	if( m_dcWin != NULL )
		delete m_dcWin;

	CWnd::PostNcDestroy();
}

void CMsgWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode) 
	{
		case SB_LINEUP:
			// ��X�N���[���L�[�������ꂽ��
			ScrolldwnMsgWin();
			break;
		case SB_LINEDOWN:
			// ���X�N���[���L�[�������ꂽ��
			ScrollupMsgWin();
			break;
		case SB_PAGEUP:
		{
			// ��y�[�W�A�b�v�������ꂽ��
			for (int i = 0; i < 16; i++)
				ScrolldwnMsgWin();
			break;
		}
		case SB_PAGEDOWN:
		{
			// ���y�[�W�A�b�v�������ꂽ��
			for (int i = 0; i < 16; i++)
				ScrollupMsgWin();
			break;
		}
	}

	// �X�N���[���o�[�ʒu���Z�b�g
	SetScrollPos();	

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMsgWnd::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

	CRect	rc;
	GetClientRect(rc);

	if(m_dcWin == NULL)
	{
		m_dcWin = new CDC;
		m_dcWin->CreateCompatibleDC(&dc);
		m_bmpWin = new CBitmap;
		m_bmpWin->CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		m_dcWin->SelectObject(m_bmpWin);
	}
	
	// ���b�Z�[�W�E�B���h�E�̕`��
	DrawMsgWindow(m_dcWin);

	// ��ʓ\��t��
	dc.BitBlt(0,0,rc.Width(),rc.Height(),m_dcWin,0,0,SRCCOPY);
}

LRESULT CMsgWnd::OnAppendMsg(WPARAM wParam, LPARAM lParam)
{
	// ���b�Z�[�W��ǉ����\������
	AddMessage((LPSTR)lParam, (COLORREF)wParam);
	// �X�N���[���o�[�ʒu��ݒ肷��
	SetScrollPos();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ���[�U�[�֐�

void CMsgWnd::Append(LPSTR fmt, ...)
{
	_TCHAR		buf[256];
	va_list		pvar;

	// ���b�Z�[�W���o�b�t�@�ɃZ�b�g����
	va_start(pvar, fmt);
	_vstprintf(buf, fmt, pvar);
	va_end(pvar);

	// ���s���o�b�t�@�̍Ō�ɃZ�b�g����B
	_tcscat(buf, _T("\n"));

	// �o�b�t�@�̓��e����ʂɕ\������
	SendMessage(WM_APPENDMSG, (WPARAM)-1, (DWORD)buf);
	Sleep(1);
}

void CMsgWnd::Append(COLORREF col, LPSTR fmt, ...)
{
	_TCHAR		buf[256];
	va_list		pvar;

	// ���b�Z�[�W���o�b�t�@�ɃZ�b�g����
	va_start(pvar, fmt);
	_vstprintf(buf, fmt, pvar);
	va_end(pvar);

	// ���s���o�b�t�@�̍Ō�ɃZ�b�g����B
	_tcscat(buf, _T("\n"));

	// �o�b�t�@�̓��e����ʂɕ\������
	SendMessage(WM_APPENDMSG, (WPARAM)col, (DWORD)buf);
	Sleep(1);
}

void CMsgWnd::AddMessage(LPSTR msg, COLORREF col)
{
	char		*p;

	// set color
	m_msgCol[m_msgTailLine] = col;

	// �����݃o�b�t�@�|�C���^���擾
	p	= m_msgBuf[m_msgTailLine] + m_msgColumn;
	// ���b�Z�[�W�̏I���܂�
	for ( ; *msg; msg++) {
		// ���s�R�[�h�łȂ��ꍇ
		if (*msg != '\n') {
			// ���b�Z�[�W���o�b�t�@�ɒǉ�����
			*p++	= *msg;
			// ���������J�E���g
			m_msgColumn++;
		}
		// ���s���\���������̌��E�Ȃ�
		if (*msg == '\n' || m_msgColumn == MsgColumn) {
			// ���݂̃o�b�t�@�����
			*p	= '\0';
			// �o�b�t�@�̓��e�����ݍs�ɕ\������
			DispMsgLine();
			// ���b�Z�[�W�s�����C���N�������g����
			m_msgNumLine++;
			// �s���C�����ŏI�ɒB������
			if (++m_msgTailLine == MsgMaxRow) 
			{
				SaveLogFile();
				// �擪�ɖ߂�
				m_msgTailLine	= 0;
			}
			// ���b�Z�[�W�o�b�t�@�����^���ɂȂ�����
			if (m_msgNumLine >= MsgMaxRow) {
				// ���b�Z�[�W�s�����ő�l�Ɋۂ߂�
				m_msgNumLine	= MsgMaxRow;
				// �w�b�_�s���ő�s���̏ꍇ�A�w�b�_���C��������������
				if (++m_msgHeadLine == MsgMaxRow)
				{
					m_msgHeadLine	= 0;
				}
			}
			// ��������������
			m_msgColumn	= 0;

			// set color
			m_msgCol[m_msgTailLine] = col;

			// 
			p	= m_msgBuf[m_msgTailLine] + m_msgColumn;
		}
	}
	/* if the last character is not a new line code, */
	if (msg[-1] != '\n') {
		/* terminate current line. */
		*p = '\0';
		/* display a current line. */
		DispMsgLine();
	}
}

void CMsgWnd::DispMsgLine(void)
{
	int		n;

	// �\���s�����v�Z
	n	= (m_msgTailLine + MsgMaxRow - m_msgDispLine) % MsgMaxRow;

	// if the displaying space is left
	if (m_msgNumLine < MsgMaxRow && n < m_nNumRow) 
	{
	}
	// if the displaying position is the tail of screen.
	else if (n == m_nNumRow) 
	{
		// increase the display point.
		if (++m_msgDispLine == MsgMaxRow)
			m_msgDispLine	= 0;
	}
	// if the displaying space was circled up,
	else if (m_msgNumLine == MsgMaxRow && n == 0) 
	{
		// set up the displaying point.
		m_msgDispLine	= (m_msgTailLine - m_nNumRow + MsgMaxRow + 1) % MsgMaxRow;
	}
}

/*====================================================*/
/* draw the message window.
*/
void CMsgWnd::DrawMsgWindow(CDC *pDC)
{
	int		i, n;
	CRect	rc;

	// �w�i����
	GetClientRect(rc);
	pDC->FillSolidRect(rc, MSGWINDOWCOLOR);

	/* for each message line,												  */
	for (n = 0, i = m_msgDispLine; i != m_msgTailLine && n < m_nNumRow; n++) {
		/* draw a message line.												  */
		DrawMsgLine(pDC, RowH * n, m_msgBuf[i], m_msgCol[i]);
		/* increase the displaying pointer.									  */
		if (++i == MsgMaxRow)		i	= 0;
	}
}

/*====================================================*/
/* draw a message line.
*/
void CMsgWnd::DrawMsgLine(CDC *pDC, int y, char *msg, COLORREF col)
{
//	CFont		*pOldFont;
	COLORREF	bkCol;

	pDC->FillSolidRect(0, y, MsgPlaneWidth, RowH, MSGWINDOWCOLOR);
//	pOldFont = pDC->SelectObject(&m_cFont);
	pDC->SetBkMode(TRANSPARENT);
	if( col != -1 )	bkCol = pDC->SetTextColor(col);
	pDC->TextOut(0, y, msg, strlen(msg));
	if( col != -1 )	pDC->SetTextColor(bkCol);
//	pDC->SelectObject(pOldFont);
}

void CMsgWnd::SetScrollPos(void)
{
	int		pos;

	/* calculate the scroll bar position.									  */
	pos	= (m_msgDispLine - m_msgHeadLine + MsgMaxRow) % MsgMaxRow;

	/* set the scroll bar position.											  */
	CWnd::SetScrollPos(SB_VERT, pos, TRUE);

	// �ĕ`��
	Invalidate(FALSE);
}

// �X�N���[��Down���̏���
void CMsgWnd::ScrolldwnMsgWin(void)
{
	/* if the display pointer is the head of buffer, do nothing.			  */
	if (m_msgDispLine == m_msgHeadLine)
		return;

	/* increase the display point.											  */
	if (--m_msgDispLine < 0)
		m_msgDispLine	= MsgMaxRow - 1;
}

// �X�N���[��Up���̏���
void CMsgWnd::ScrollupMsgWin(void)
{
	int		n;

	/* if the displayinh head line is the tail line in the buffer,			  */
	n	= (m_msgTailLine - m_msgDispLine + MsgMaxRow) % MsgMaxRow;
	if (n <= m_nNumRow) {
		/* do nothing.														  */
		return;
	}

	/* increase the display point.											  */
	if (++m_msgDispLine == MsgMaxRow)
		m_msgDispLine	= 0;
}

// �R���t�B�O���̃p�X��ݒ�
BOOL CMsgWnd::SetLogPath(const char *pszPath)
{
	int	len, max;

	// �ُ�A�h���X�`�F�b�N
	if( pszPath == NULL )
		return FALSE;

	// �p�X���`�F�b�N
	max = MAX_PATH-1;
	len = strlen(m_szLogPath);
	if( len > max )
		return FALSE;

	// �p�X���X�V
	strncpy(m_szLogPath, pszPath, max);

	return TRUE;
}

// �R���t�B�O���̃p�X���擾
const char *CMsgWnd::GetLogPath(void)
{
	return m_szLogPath;
}

// ���O�ۑ������ݒ�
void CMsgWnd::SetLogDays(int day)
{
	m_nDays = day;
}

// ���O�ۑ������擾
int CMsgWnd::GetLogDays(void)
{
	return m_nDays;
}

// ���O���t�@�C���ɕۑ��i�Â��t�@�C���͍폜�j
void CMsgWnd::SaveLogFile(void)
{
	// ���O�����Ȃ甲����
	if( !m_bLog )
		return;

	FILE	*streem = NULL;
	char	buf[MAX_PATH];
	int		i;

	// �t�@�C�����̍쐬
	HANDLE			fp;
	WIN32_FIND_DATA	fd; 

	try
	{
		//-----------------------------
		// �ߋ��̂���Ȃ��t�@�C�����폜
		//-----------------------------
		sprintf(buf, "%s\\*.*", m_szLogPath);
		fp = FindFirstFile(buf, &fd);

		FILETIME ft;
		SYSTEMTIME st;

		GetLocalTime(&st);
		SystemTimeToFileTime(&st, &ft);

		if(INVALID_HANDLE_VALUE != fp)
		{
			for(BOOL f = TRUE;f == TRUE;)
			{
				if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if( FT_Comp(FT_Add( fd.ftLastWriteTime, FT_Day(m_nDays)), ft) < 0)
					{
						sprintf(buf, "%s\\%s", m_szLogPath, fd.cFileName);
						DeleteFile(buf);
					}
				}
				f = FindNextFile(fp, &fd);
			}
		}
		FindClose(fp);

		//-----------------------------
		// �V���ȃt�@�C�����쐬
		//-----------------------------
		sprintf(buf, "%s\\%04d%02d%02d.log", m_szLogPath, st.wYear, st.wMonth, st.wDay);

		// �t�@�C���̃I�[�v��
		if(NULL == (streem = fopen(buf, "a+t")))
		{
			return;
		}

		for(i = 0;i != m_msgTailLine;i++)
		{
			if(i >= MsgMaxRow)
				i = 0;

			if(EOF == fputs(m_msgBuf[i], streem))
				break;

			if(EOF == fputs("\n", streem))
				break;
		}
		// �t�@�C�������
		fclose(streem);
		streem = NULL;
	}
	catch(...)
	{
		// �t�@�C�������
		if(streem != NULL)
			fclose(streem);
	}
}

// ���O�ۑ��t���O�ݒ�
void CMsgWnd::SetLog(bool bFlg)
{
	m_bLog = bFlg;
}

// ���O�ۑ��t���O�擾
bool CMsgWnd::GetLog(void)
{
	return m_bLog;
}

// �E�B���h�E�I�����̋�����������
void CMsgWnd::ForceSave(void)
{
	// �I�����Ƀ��O��ۑ�
	SaveLogFile();
	// �擪�ɖ߂�
	m_msgTailLine	= 0;
}

// ���b�Z�[�W���X�g�̃��Z�b�g
void CMsgWnd::ResetContent(void)
{
	// ���b�Z�[�W�o�b�t�@�|�C���^�̏�����
	m_msgHeadLine	= m_msgTailLine	= m_msgDispLine	= 0;
	m_msgNumLine	= 0;
	m_msgColumn		= 0;
	SetScrollPos();
}

// �t�@�C���^�C���̉��Z
FILETIME FT_Add(FILETIME ft1, FILETIME ft2)
{
	__int64	i1, i2, i3;
	FILETIME ft3;

	i1 = (__int64)( ( (__int64)ft1.dwHighDateTime << 32 ) | ft1.dwLowDateTime );
	i2 = (__int64)( ( (__int64)ft2.dwHighDateTime << 32 ) | ft2.dwLowDateTime );
	i3 = i1 + i2;

	ft3.dwLowDateTime = (DWORD)i3;
	ft3.dwHighDateTime = (DWORD)( (__int64)i3 >> 32 );

	return (FILETIME)ft3;
}

// �H�����̃f�[�^�擾
FILETIME FT_Day(INT nCount)
{
	FILETIME FT_Add(FILETIME, FILETIME);
	FILETIME ft;
	FILETIME Val = {0,0};

	// �P���̃f�[�^���Z�b�g
	ft.dwLowDateTime = 0x2a69c000;
	ft.dwHighDateTime = 0xc9;

	for(int i = 0;i < nCount;i++)
		Val = FT_Add(Val, ft);

	return (FILETIME)Val;
}

// �t�@�C���^�C���̔�r
int FT_Comp(FILETIME ft1, FILETIME ft2)
{
	if(ft1.dwHighDateTime > ft2.dwHighDateTime)
		return 1;
	else 
		if(ft1.dwHighDateTime < ft2.dwHighDateTime)
			return -1;

	if(ft1.dwLowDateTime > ft2.dwLowDateTime)
		return 1;
	else 
		if(ft1.dwLowDateTime < ft2.dwLowDateTime)
			return -1;

	return 0;	
}

