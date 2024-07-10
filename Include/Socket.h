#ifndef _SOCKET_H
#define _SOCKET_H
#include <winsock.h>
#include <tchar.h>

// -----------------------------------------------------------------
// ���j����ifdef�͐G��Ȃ���
// -----------------------------------------------------------------
#ifdef SOCKET_EXPORTS
#define SOCKET_API __declspec(dllexport)
#else
#define SOCKET_API __declspec(dllimport)
#endif
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// ����		�FVer1.0�쐬	->�@2001/07 �O��
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// �N���X��	�FCWinSock
// ���e		�FWinSock���g���ׂ̏����N���X�ł��B
// -----------------------------------------------------------------
class SOCKET_API CWinSock
{
	// �����o�ϐ�
	WSADATA		m_wsaData;
	int			m_nErr;	

public:
	~CWinSock();								// �f�X�g���N�^
	BOOL	InitWinSock( BYTE, BYTE);			// WinSock������
	int		GetError(void);						// �ُ�R�[�h�擾
};



// -----------------------------------------------------------------
// �N���X��	�FCSock1
// ���e		�Fwinsock1.1�N���X�ł��B�i�����^�j
//			�F�ڑ��⑗��M�Ȃǂ̎��Ԃ̂�����֐��͂���p�̃X���b�h����
//			�F�Ăяo���悤�ɂ��܂��傤�B
//			�F�����o�֐��͒ʏ�p���g�����𐄏����܂��B
// -----------------------------------------------------------------
class SOCKET_API CSock1
{
private:												
	SOCKET	m_sock;								// socket �n���h��
	int		m_nPort;							// �|�[�g�ԍ�
	int		m_nType;							// �\�P�b�g�^�C�v
	int		m_nErr;								// �G���[�R�[�h
public:
	CSock1( );									// �R���X�g���N�^
	virtual	~CSock1( );							// �f�X�g���N�^

public:
	virtual	BOOL	Create( int, int, int);			// �\�P�b�g�쐬�i��{�j
	virtual	BOOL	Create( int );					// �\�P�b�g�쐬�i�ʏ�p�@TCP or UDP�j
	virtual	void	Create( SOCKET, 
						int nType = SOCK_STREAM );	// �\�P�b�g�쐬�iACCEPT�p�j
	virtual	BOOL	Shutdown( int );				// �\�P�b�g����M�֎~�i�ʏ�p�j
	virtual	BOOL	Close( void );					// �\�P�b�g�N���[�Y�i�ʏ�p�j
	virtual	BOOL	Bind( struct sockaddr * );		// �o�C���h�i��{�j
	virtual	BOOL	Bind( DWORD, int);				// �o�C���h�i�ʏ�p�@��ɃT�[�o�[�p�j
	virtual	BOOL	Connect( struct sockaddr * );	// �ڑ��i��{�j
	virtual	BOOL	Connect( DWORD, int);			// �ڑ��i�ʏ�p�j
	virtual	SOCKET	Accept( struct sockaddr * );	// �ڑ����󂯕t����i��{�j
	virtual	SOCKET	Accept( void );					// �ڑ����󂯕t����i�ʏ�p�j
	virtual	BOOL	Listen( int );					// �ڑ��҂���ʒm�i��{,�ʏ�p�j
	virtual	int		Recv( LPSTR, int, int);			// ��M�����i��{,TCP�p�j
	virtual	int		Recv( LPSTR, int, int, 
						struct sockaddr *);			// ��M�����i��{,UDP�p�j
	virtual	int		Recv( LPVOID, int);				// ��M�����i�ʏ�p,TCP�p�j
	virtual int		Recv( LPVOID, int, LPDWORD, LPINT); // ��M�����i�ʏ�p,UDP�p�j
	virtual	int		Send( LPSTR, int, int);			// ��M�����i��{,TCP�p�j
	virtual	int		Send( LPSTR, int, int, 
						struct sockaddr *);			// ��M�����i��{,UDP�p�j
	virtual	int		Send( LPVOID, int);				// ��M�����i�ʏ�p,TCP�p�j
	virtual	int		Send( LPVOID, int, DWORD, int);	// ��M�����i�ʏ�p,UDP�p�j
	virtual	int		Select( int, fd_set *, fd_set *, 
						fd_set *, timeval *);		// �\�P�b�g��Ԓ��ׂ�i��{�j
	virtual	BOOL	Select( DWORD, DWORD );			// �\�P�b�g��Ԓ��ׂ�i�ʏ�p�j
	virtual	SOCKET	GetSocket( void );				// �\�P�b�g�n���h���擾
//	virtual	int		GetPortNo( void );				// �\�P�b�g�Ɍ��т��Ă���|�[�g�ԍ��擾
	virtual	int		GetError( void );				// �G���[�R�[�h�擾
	virtual int		GetSockType();					// �\�P�b�g�^�C�v�擾( 1..TCP  2..UDP )
	virtual void	GetErrorMsg(LPTSTR pMsg);		//	�G���[���b�Z�[�W�擾
	virtual BOOL	SetAsyncSock( HWND hWnd,		//	���� -> �񓯊��ؑւ�
								  UINT uMsg,
								  long nEvent );
	virtual BOOL	Get_HostInfo(LPTSTR pHostName,
								 LPTSTR pAddr);
//	2004/11/11	add
	virtual BOOL	SetSockOut(int nMode, DWORD dwTimeout);	//	�^�C���A�E�g�ݒ� nMode=1..���M 2..��M dwTimeout=msec
//	2004/11/11	end


};


// -----------------------------------------------------------------
// �N���X��	�FCSock2
// ���e		�FWinSock2.0�N���X�ł��B�i��邩�ǂ����͖���j
// -----------------------------------------------------------------
//class SOCKET_API CSock2
//{
//};
#endif