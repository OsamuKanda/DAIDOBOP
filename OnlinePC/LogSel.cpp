// LogSel.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "LogSel.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CBolog	&lg = theApp.m_log;


// CLogSel dialog
IMPLEMENT_DYNAMIC(CLogSel, CDialog)
CLogSel::CLogSel(CWnd* pParent /*=NULL*/)
	: CDialog(CLogSel::IDD, pParent)
{
	m_nSelectNum = 0;
	m_nTrendNum = 0;
}

CLogSel::~CLogSel()
{
}

void CLogSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_TIME, m_textStTime);
	DDX_Control(pDX, IDC_TOTAL_MIN, m_textTotalMin);
	DDX_Control(pDX, IDC_LOG_REMARK, m_editRemark);
	DDX_Control(pDX, IDC_LOG_LIST, m_listLog);
}


BEGIN_MESSAGE_MAP(CLogSel, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDC_RECORD, OnClickedRecord)
	ON_BN_CLICKED(IDC_DELETE, OnClickedDelete)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LOG_LIST, OnSelchangeLogList)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CLogSel message handler

BOOL CLogSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set up trend file directory entry.
	SetupTrendDirDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// clicked display button.
void CLogSel::OnClickedOk()
{
	m_nSelectNum = m_listLog.GetCurSel();
	if( m_nSelectNum >= 0 )
		OnOK();
}

// clicked record button.
void CLogSel::OnClickedRecord()
{
	int		sel;

	sel = m_listLog.GetCurSel();
	if(sel >= 0)
	{
		PutSelectedTrendEntry(sel);
		SetupTrendDirDlg();
		m_listLog.SetCurSel(sel);
		SetSelectedTrendEntry(sel);
	}
}

// clicked delete button.
void CLogSel::OnClickedDelete()
{
	int		sel;

	if( IDNO == AfxMessageBox(IDS_LOGSEL01, MB_YESNO) )
		return;

	sel = m_listLog.GetCurSel();
	if(sel >= 0)
	{
		DeleteTrendFile(sel);
		SetupTrendDirDlg();
	}
}

// clicked close button.
void CLogSel::OnClickedCancel()
{
	OnCancel();
}

// change log list selection.
void CLogSel::OnSelchangeLogList()
{
	int		sel;

	sel = m_listLog.GetCurSel();
	SetSelectedTrendEntry(sel);
}

///////////////////////////////////////////////////////////////////////////////
// CLogSel member function

// set up trend file directory entry.
void CLogSel::SetupTrendDirDlg(void)
{
	TrendDirectory	*p;
	int				numTrendFile;
	int				i;

	// get trend file entry in the trend file directory.
	numTrendFile = lg.GetTrendDirectory(trendDir);
	m_nTrendNum = numTrendFile;

	// reset trend file list.
	m_listLog.SetRedraw(FALSE);
	m_listLog.ResetContent();

	// for each entry,
	for (i = 0, p = trendDir; i < numTrendFile; i++, p++) 
	{
		// append trend file entry.
		m_listLog.AddString(lg.MakeupTrendEntString(p));
	}

	// terminate trend file entry.
	m_listLog.SetRedraw(TRUE);
}

// set now selected entry information.
void CLogSel::SetSelectedTrendEntry(int sel)
{
	FILE	*fp;
	_TCHAR	buf[256];
	_TCHAR	dirName[MAX_PATH];

	// sel no check
	if (sel < 0) return;

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( dirName, lg.LogRootDir ), _T("\\") ), trendDir[sel].name );

	// open and read the trend file header.
	if ((fp = _tfopen( dirName, _T("rb") )) == NULL)
	{
		// if file not found, display null string.
		m_textStTime.SetWindowText(_T(""));
		m_textTotalMin.SetWindowText(_T(""));
		m_editRemark.SetWindowText(_T(""));
		return;
	}
	fread(&m_header, sizeof(m_header), 1, fp);
	// close the trend file.
	fclose(fp);

	// display start time of this entry.
	m_textStTime.SetWindowText(trendDir[sel].date);

	// display total minutes of this entry.
	_stprintf(buf, _T("%7.3lf"),
		(double)trendDir[sel].totalTick * m_header.header.samplePeriod * 0.001 / 60.0);
	m_textTotalMin.SetWindowText(buf);

	// display remark of this entry.
	m_editRemark.SetWindowText(m_header.header.remark);
}

// put new selected entry information.
void CLogSel::PutSelectedTrendEntry(int sel)
{
	FILE	*fp;
	_TCHAR	buf[256];
	_TCHAR	dirName[MAX_PATH];

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( dirName, lg.LogRootDir ), _T("\\") ), trendDir[sel].name );

	// file attrib conv. normal
	_tchmod(dirName, S_IREAD | S_IWRITE);

	// open and write the trend file header.
	if ((fp = _tfopen( dirName, _T("r+b") )) == NULL)
		return;

	// load data file header. 
	fread(&m_header, sizeof(m_header), 1, fp);

	// get remark of this entry.
	m_editRemark.GetWindowText(buf, sizeof(buf));
	_tcsncpy(m_header.header.remark, buf, sizeof(m_header.header.remark)/sizeof(_TCHAR));

	// seek the file to remark position.
	fseek(fp, 0L, SEEK_SET);

	// header write
	fwrite(&m_header, sizeof(m_header), 1, fp);

	// close the trend file.
	fclose(fp);

	// file attrib conv. read only
	_tchmod(dirName, S_IREAD);
}

// put new selected entry information.
void CLogSel::DeleteTrendFile(int sel)
{
	_TCHAR	delFile[MAX_PATH];
	_TCHAR	backFile[MAX_PATH];
	DWORD	dwErr;

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( delFile, lg.LogRootDir ), _T("\\") ), trendDir[sel].name );

	// read only file check.
	if(_taccess(delFile, 2) == -1)
	{
		AfxMessageBox(IDS_LOGSEL02);
		return;
	}

	// create backup file name.
	_tcscat( _tcscat( _tcscpy( backFile, lg.LogRootDirBackDir ), _T("\\") ), trendDir[sel].name );

	// create backup file.
	if( !CopyFile(delFile, backFile, FALSE) )
	{
		// get error code.
		dwErr = GetLastError();

		// if path is not found,
		if( ERROR_PATH_NOT_FOUND == dwErr )
		{
			// make up the trend backup directory.
			_tmkdir( lg.LogRootDirBackDir );

			// retry copy file.
			if( CopyFile(delFile, backFile, FALSE) )
				dwErr = 0;
			else
				dwErr = GetLastError();
		}

		// if the error is occurred,
		if( dwErr > 0 )
		{
			// get error message.
			LPVOID lpMsgBuf;
			FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |
							FORMAT_MESSAGE_FROM_SYSTEM |
							FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR) &lpMsgBuf, 0, NULL);

			// disp error message
			AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
		
			// free buffer.
			LocalFree(lpMsgBuf);
			return;
		}
	}

	// remove old backup file.
	BackupFileRemove();

	// save backUp file name.
	BackupNameSave(trendDir[sel].name);

	// delete the trend data file.
	if( -1 == _tremove(delFile) )
	{
		int aaa = errno;
		LPTSTR	pszMsg = __tcserror(NULL);
		AfxMessageBox(pszMsg, MB_OK | MB_ICONINFORMATION);
	}
}

// remove backup file.
void CLogSel::BackupFileRemove(void)
{
	FILE	*fp;
	_TCHAR	buf[256];
	_TCHAR	dirName[MAX_PATH];

	// make backup path.
	_tcscat( _tcscpy( dirName, lg.LogRootDir ), LogBackUpNameWork );

	// open and write the trend file header.
	if((fp = _tfopen( dirName, _T("r+t") )) == NULL)
	{
		if ((fp = _tfopen( dirName, _T("w+t") )) == NULL)
			return;
		fclose(fp);
		return;
	}

	// backUp file name pickup
	if(fscanf(fp, _T("%s\r\n"), buf ) != 1) {
		fclose(fp);
		return;
	}
	fclose(fp);

	// create backup file name.
	_tcscat( _tcscat( _tcscpy( dirName, lg.LogRootDirBackDir ), _T("\\")), buf);

	// delete the trend data file.
	_tremove(dirName);
}

// save backUp log file name
void CLogSel::BackupNameSave(LPCTSTR lpszName)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];

	// make backup path.
	_tcscat( _tcscpy( dirName, lg.LogRootDir ), LogBackUpNameWork );

	// open and write the trend file header.
	if((fp = _tfopen( dirName, _T("r+t") )) == NULL)
	{
		if ((fp = _tfopen( dirName, _T("w+t") )) == NULL)
			return;
	}

	// write the file name
	_ftprintf(fp, _T("%s\r\n"), lpszName );

	// close file.
	fclose(fp);
}

BOOL CLogSel::NoDialogSetup(int sel)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];
	int		numTrendFile;

	// get trend file entry in the trend file directory.
	numTrendFile = lg.GetTrendDirectory(trendDir);
	m_nTrendNum = numTrendFile;

	// sel no check
	if (sel < 0 || sel >= m_nTrendNum) return FALSE;

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( dirName, lg.LogRootDir ), _T("\\") ), trendDir[sel].name );

	// open and read the trend file header.
	if ((fp = _tfopen( dirName, _T("rb") )) == NULL)
	{
		// if file not found, display null string.
		m_textStTime.SetWindowText(_T(""));
		m_textTotalMin.SetWindowText(_T(""));
		m_editRemark.SetWindowText(_T(""));
		return FALSE;
	}
	fread(&m_header, sizeof(m_header), 1, fp);
	// close the trend file.
	fclose(fp);

	m_nSelectNum = sel;

	return TRUE;
}

