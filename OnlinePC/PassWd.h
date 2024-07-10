// PassWd.H - Passwordアクセスクラス用ヘッダファイル
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 著作権表示
//-----------------------------------------------------------------------------
/****************************************************************************** 
Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
rights reserved.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
******************************************************************************/
#include "../lib/md5.h"

// Passwordクラス
class CPassWd
{
// public member function.
public:
	CPassWd(){_tcscpy(m_strPath,_T("../Config/passwd"));};
	virtual ~CPassWd(){};

	// compare password
	BOOL Compare(unsigned char *input, unsigned int inputLen);

	// save new password
	BOOL SaveNew(unsigned char *input, unsigned int inputLen);

	// set passwd path
	void SetPassPath(LPCTSTR pszPath);

// protected member function.
protected:
	// read password from file
	BOOL ReadPasswd(unsigned char digest[16]);

	// write password to file
	BOOL WritePasswd(unsigned char digest[16]);

// protected member variable.
protected:
	CMD5		m_md5;
	_TCHAR		m_strPath[_MAX_PATH];
};
