// TcName.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TcName.h"


// CTcName �_�C�A���O

IMPLEMENT_DYNAMIC(CTcName, CDialog)

CTcName::CTcName(int nAnsArg, CWnd* pParent /*=NULL*/)
	: CDialog(CTcName::IDD, pParent)
{
	CRegKey		reg;
	DWORD		dwSize;

	m_nAsnArg = nAnsArg;
	memset(&m_stTcNm, 0, sizeof(m_stTcNm));
	memset(&m_cRootDir, 0, sizeof(m_cRootDir));

	reg.Open(HKEY_CURRENT_USER, _T("Software\\BOP"));

	dwSize = sizeof(m_cRootDir);
	reg.QueryValue(&m_cRootDir[0], _T("ROOTDIR"),&dwSize);
}

CTcName::~CTcName()
{
}

void CTcName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TCN, m_editTcName);
}


BEGIN_MESSAGE_MAP(CTcName, CDialog)
	ON_BN_CLICKED(IDOK, &CTcName::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTcName::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTcName ���b�Z�[�W �n���h��

BOOL CTcName::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString		str;

	// real tc name from file.
	if( !ReadTcNameFile(&m_stTcNm) )
	{
		// if err,
		SetDefaultTCName(&m_stTcNm);
		WriteTcNameFile(&m_stTcNm);
	}

	if( m_nAsnArg >= 0 && m_nAsnArg < NumTCTap )
	{
		str = m_stTcNm.name[m_nAsnArg];
		m_editTcName.SetWindowText(str);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
}

// set default tc name.
void CTcName::SetDefaultTCName(TcName *tcnm)
{
	_TCHAR	buf[10];						// file path buffer
	int		idx;

	// for each tc, 
	for ( idx = 0; idx < NumTCTap; idx++ ) 
	{
		// store the default tc name 
		_stprintf(buf, _T("#%d"), idx+1);
		_tcscpy(tcnm->name[idx], buf);
	}
}


// real tc name from file.
BOOL CTcName::ReadTcNameFile(TcName *tcnm)
{
	FILE	*fp;							// file handle
	_TCHAR	buf[256];						// file path buffer
	_TCHAR	dirName[MAX_PATH];
	int		idx, i;

	// make up file name. 
	_tcscat( _tcscat( _tcscpy( dirName, m_cRootDir ), _T("\\Config")), TCNameFileName );

	// open the configration file. 
	if ((fp = _tfopen( dirName, _T("r") )) == NULL)
		return FALSE;

	// for each line in the file, 
	for ( idx = 0; _fgetts(buf, 256, fp); ) {

		// store the value. 
		_tcsncpy(tcnm->name[idx], buf, TCNM_MAX_LEN);

		// delete return code.
		for( i = 0; i < TCNM_MAX_LEN; i++ )
			if( tcnm->name[idx][i] == 10 )
				tcnm->name[idx][i] = 0;

		if( ++idx >= NumTCTap )
			break;
	}

	// after reading all lines, close the file. 
	fclose(fp);

	return TRUE;
}

// write tc name from file.
BOOL CTcName::WriteTcNameFile(TcName *tcnm)
{
	FILE	*fp;							// file handle
	_TCHAR	buf[256];						// file path buffer
	_TCHAR	dirName[MAX_PATH];
	int		idx;

	// make up file name. 
	_tcscat( _tcscat( _tcscpy( dirName, m_cRootDir ), _T("\\Config")), TCNameFileName );

	// open the configration file. 
	if ((fp = _tfopen( dirName, _T("w") )) == NULL)
		return FALSE;

	// for each line in the file, 
	for ( idx = 0; idx < NumTCTap; idx++ ) 
	{
		// store the assignment. 
		_ftprintf(fp, _T("%s\n"), tcnm->name[idx]);
	}

	// after reading all lines, close the file. 
	fclose(fp);

	return TRUE;
}

// �ύX�{�^��
void CTcName::OnBnClickedOk()
{
	CString	strIn, strChk;
	int		len, idx;

	// TCNo���s���Ȃ�I��
	if( m_nAsnArg < 0 || m_nAsnArg >= NumTCTap )
	{
		OnCancel();
	}


	// ���͒l�擾
	m_editTcName.GetWindowText(strIn);

	// ���̓`�F�b�N
	len = strIn.GetLength();
	if( len <= 0 )
	{
		AfxMessageBox(IDS_TCNAME01);
		return;
	}
	if( len > TCNM_MAX_LEN )
	{
		AfxMessageBox(IDS_TCNAME02);
		return;
	}

	// ���͒l�𐧌�
	strIn = strIn.Left(TCNM_MAX_LEN);

	// �d���`�F�b�N
	for ( idx = 0; idx < NumTCTap; idx++ ) 
	{
		// �����ȊO��
		if( m_nAsnArg != idx )
		{
			// ��v���镶���񂪂���Δ�����
			strChk = m_stTcNm.name[idx];
			if( strIn.Compare(strChk) == 0 )
				break;
		}
	}
	
	// ��v������̂������
	if( idx < NumTCTap )
	{
		AfxMessageBox(IDS_TCNAME03);
		return;
	}

	// ���ʂ�ۑ�
	_tcscpy(m_stTcNm.name[m_nAsnArg], strIn);
	WriteTcNameFile(&m_stTcNm);

	// ��ʂ����
	OnOK();
}

// �L�����Z���{�^��
void CTcName::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	OnCancel();
}
