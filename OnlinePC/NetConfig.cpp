// SysConfig.cpp: CNetConfig �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OnlinePC.h"
#include "NetConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/�j��
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ���O	: CNetConfig
// ���e	: �\�z
// ����	: �Ȃ�
// �ߒl	: �Ȃ�
//-----------------------------------------------------------------------------
CNetConfig::CNetConfig()
{
	// ���������f�t�H���g�l�ݒ�
	memset(&m_stLog, 0, sizeof(m_stLog));
	memset(m_szPath, 0, sizeof(m_szPath));
	strcpy(m_szPath, DEF_CONFIG_PATH);
}

//-----------------------------------------------------------------------------
// ���O	: ~CNetConfig
// ���e	: �j��
// ����	: �Ȃ�
// �ߒl	: �Ȃ�
//-----------------------------------------------------------------------------
CNetConfig::~CNetConfig()
{
}

//-----------------------------------------------------------------------------
// ���O	: LoadConfig
// ���e	: �R���t�B�O���̓ǂݍ���
// ����	: �Ȃ�
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::LoadConfig(void)
{
	StLogPrm	stLog;
	BOOL		bRet;

#define	GET_LOG_DW(x)	\
	bRet = GetDataDW(LOG_SECTION_NAME, #x, &stLog.dw##x);\
	if( !bRet ) return FALSE;

#define	GET_LOG_STR(x,c)	\
	bRet = GetDataStr(LOG_SECTION_NAME, #x, stLog.sz##x, c);\
	if( !bRet ) return FALSE;

	// Log�p�����[�^�ǂݏo��
	GET_LOG_STR(SysLogPath, MAX_LOG_PATH);
	GET_LOG_DW(SysLogDays);
	GET_LOG_DW(SysLogSw);

	// �p�����[�^�̗p
	m_stLog = stLog;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O	: SaveConfig
// ���e	: �R���t�B�O���̕ۑ�
// ����	: �Ȃ�
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::SaveConfig(void)
{
	StLogPrm	stLog;
	BOOL		bRet;

#define	SET_LOG_DW(x)	\
	bRet = SetDataDW(LOG_SECTION_NAME, #x, stLog.dw##x);\
	if( !bRet ) return FALSE;

#define	SET_LOG_STR(x,c)	\
	bRet = SetDataStr(LOG_SECTION_NAME, #x, stLog.sz##x, c);\
	if( !bRet ) return FALSE;

	// get current parameter.
	stLog = m_stLog;

	// Log�p�����[�^�ǂݏo��
	SET_LOG_STR(SysLogPath, MAX_LOG_PATH);
	SET_LOG_DW(SysLogDays);
	SET_LOG_DW(SysLogSw);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O	: SetConfigPath
// ���e	: �R���t�B�O���̃p�X��ݒ�
// ����	: pszPath	... �p�X�̃A�h���X
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::SetConfigPath(LPCSTR pszPath)
{
	int	len, max;

	// �ُ�A�h���X�`�F�b�N
	if( pszPath == NULL )
		return FALSE;

	// �w��ł���p�X���𓾂�
	len = strlen(CONFIG_NAME);
	max = MAX_CONFIG_PATH - len - 1;

	// �p�X���`�F�b�N
	len = strlen(pszPath);
	if( len > max )
		return FALSE;

	// �p�X���X�V
	strncpy(m_szPath, pszPath, len);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O	: GetConfigPath
// ���e	: �R���t�B�O���̃p�X���擾
// ����	: �Ȃ�
// �ߒl	: �p�X�̃A�h���X
//-----------------------------------------------------------------------------
LPCTSTR CNetConfig::GetConfigPath(void)
{
	return m_szPath;
}

//-----------------------------------------------------------------------------
// ���O	: GetDataStr
// ���e	: �p�����[�^�i������f�[�^�j�ǂݍ���
// ����	: szSession	... �Z�b�V������
//		: szName	... �L�[��
//		: pszString	... ������i�[�A�h���X
//		: nSize		... �o�b�t�@�T�C�Y
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::GetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize)
{
	char	szPath[MAX_CONFIG_PATH];
	char	szData[MAX_DATA_SIZE];
	UINT	uiRet;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	uiRet = GetPrivateProfileString(szSession, szName, "", szData, nSize, szPath);
	if( uiRet == 0 )
		return FALSE;

	strncpy(pszString, szData, nSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O	: GetDataDW
// ���e	: �p�����[�^�iDWORD�f�[�^�j�ǂݍ���
//		: ���j�|�P�ُ͈�F���Ɏg�p���Ă���̂Ŏg�p�s��
// ����	: szSession	... �Z�b�V������
//		: szName	... �L�[��
//		: pData		... �f�[�^�i�[�A�h���X
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::GetDataDW(const char *szSession, const char *szName, DWORD	*pData)
{
	char	szPath[MAX_CONFIG_PATH];
	UINT	uiData;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	uiData = GetPrivateProfileInt(szSession, szName, -1, szPath);
	if( uiData == -1 )
		return FALSE;

	*pData = (DWORD)(uiData);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O	: SetDataStr
// ���e	: �p�����[�^�i������f�[�^�j�ǂݍ���
// ����	: szSession	... �Z�b�V������
//		: szName	... �L�[��
//		: pszString	... ������i�[�A�h���X
//		: nSize		... �o�b�t�@�T�C�Y
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::SetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize)
{
	char	szPath[MAX_CONFIG_PATH];
	char	szData[MAX_DATA_SIZE];
	UINT	uiRet;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	strncpy(szData, pszString, nSize);

	uiRet = WritePrivateProfileString(szSession, szName, szData, szPath);
	if( uiRet == 0 )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O	: SetDataDW
// ���e	: �p�����[�^�iDWORD�f�[�^�j�ǂݍ���
//		: ���j�|�P�ُ͈�F���Ɏg�p���Ă���̂Ŏg�p�s��
// ����	: szSession	... �Z�b�V������
//		: szName	... �L�[��
//		: pData		... �f�[�^�i�[�A�h���X
// �ߒl	: TRUE�F����AFALSE�F�ُ�
//-----------------------------------------------------------------------------
BOOL CNetConfig::SetDataDW(const char *szSession, const char *szName, DWORD	dwData)
{
	char	szPath[MAX_CONFIG_PATH];
	char	szData[MAX_DATA_SIZE];
	UINT	uiRet;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	sprintf(szData, "%d", dwData);

	uiRet = WritePrivateProfileString(szSession, szName, szData, szPath);
	if( uiRet == 0 )
		return FALSE;

	return TRUE;
}
