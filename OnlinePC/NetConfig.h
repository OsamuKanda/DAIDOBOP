// SysConfig.h: CNetConfig �N���X�̃C���^�[�t�F�C�X
//
#if !defined(AFX_SYSCONFIG_H_INCLUDED_)
#define AFX_SYSCONFIG_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===================================================================
// �萔
//===================================================================
#define	DEF_CONFIG_PATH		"../Config/"
#define	CONFIG_NAME			"Log.ini"	
#define	LOG_SECTION_NAME	"Log"
#define	MAX_DATA_SIZE		256
#define	MAX_LOG_PATH		256
#define	MAX_CONFIG_PATH		MAX_PATH

//===================================================================
// �\���̒�`
//===================================================================
// ���O�p�����[�^
typedef struct
{
	char	szSysLogPath[MAX_LOG_PATH];			// �V�X�e�����O�p�X
	DWORD	dwSysLogDays;						// �V�X�e�����O�ۑ�����
	DWORD	dwSysLogSw;							// �V�X�e�����O�ۑ��L��
}StLogPrm;

//===================================================================
// �N���X���
//===================================================================
class CNetConfig  // �p�����Ȃ�
{
// �\�z���j��
public:
	CNetConfig();						// �\�z
	~CNetConfig();						// �j��

/////////////////////////////
// �����o�ϐ��i�O�����J�j
public:
	StLogPrm	m_stLog;				// ���O�p�����[�^

/////////////////////////////
// �����o�ϐ��i�O������J�j
private:
	char	m_szPath[MAX_CONFIG_PATH];	// �R���t�B�O���̃p�X

/////////////////////////////
// ���\�b�h�i�O�����J�j
public:
	BOOL LoadConfig(void);				// �R���t�B�O����ǂݍ���
	BOOL SaveConfig(void);				// �R���t�B�O������������
	BOOL SetConfigPath(LPCTSTR);		// �R���t�B�O���̃p�X��ݒ�
	LPCTSTR GetConfigPath(void);		// �R���t�B�O���̃p�X���擾

/////////////////////////////
// ���\�b�h�i�O������J�j
private:
	BOOL GetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize);
										// �p�����[�^�i������f�[�^�j�ǂݍ���
	BOOL GetDataDW(LPCTSTR szSession, LPCTSTR szName, DWORD	*pData);
										// �p�����[�^�iDWORD�f�[�^�j�ǂݍ���
	BOOL SetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize);
										// �p�����[�^�i������f�[�^�j��������
	BOOL SetDataDW(const char *szSession, const char *szName, DWORD	dwData);
										// �p�����[�^�iDWORD�f�[�^�j��������
};

#endif // !defined(AFX_SYSCONFIG_H_INCLUDED_)
