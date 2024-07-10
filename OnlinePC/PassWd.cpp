// PassWd.cpp - Passwordアクセスクラス用ソースファイル
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

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "PassWd.h"

// compare password
BOOL CPassWd::Compare(unsigned char *input, unsigned int inputLen)
{
	unsigned char digest1[16];
	unsigned char digest2[16];

	// crypt password
	m_md5.Init();
	m_md5.Update(input, inputLen);
	m_md5.Final(digest1);

	// read password
	if( !ReadPasswd(digest2) )
		return FALSE;

	return (memcmp(digest1, digest2, sizeof(digest1)) == 0);
}

// save new password
BOOL CPassWd::SaveNew(unsigned char *input, unsigned int inputLen)
{
	unsigned char digest[16];

	// crypt password
	m_md5.Init();
	m_md5.Update(input, inputLen);
	m_md5.Final(digest);

	// write password
	return WritePasswd(digest);
}

// read password from file
BOOL CPassWd::ReadPasswd(unsigned char digest[16])
{
	FILE	*fp;

	fp = _tfopen(m_strPath, _T("rb"));
	if( fp == NULL )
		return FALSE;
	fread(digest, sizeof(BYTE)*16, 1, fp);
	fclose(fp);

	return TRUE;
}

// write password to file
BOOL CPassWd::WritePasswd(unsigned char digest[16])
{
	FILE	*fp;

	fp = _tfopen(m_strPath, _T("wb"));
	if( fp == NULL )
		return FALSE;
	fwrite(digest, sizeof(BYTE)*16, 1, fp);
	fclose(fp);

	return TRUE;
}

// set passwd path
void CPassWd::SetPassPath(LPCTSTR pszPath)
{
	_tcscpy(m_strPath, pszPath);
}

