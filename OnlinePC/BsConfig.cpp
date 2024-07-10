#include	"stdafx.h"
#include	"Config.h"


// read BO system information.
BOOL CConfig::ReadBosysInfo(int sel, BoSysInfo *pInfo)
{
	FILE		*fp;
	_TCHAR		buf[256];
	_TCHAR		*p;
	int			i;
	BoSysInfo	dummy = {0};

	// make up file name.
	_stprintf(buf, _T("%s") BoSystemFile, WorkRootDir, sel);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("r"))) == NULL)
		return FALSE;

	// read data
	if(_fgetts(buf, 256, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// version check
	if( NULL == _tcsstr(buf,_T("Ver:")) )
	{
		// version <= 500
		switch(sel)
		{
		case 0:
			_tcscpy(dummy.szComment, _T("HC"));
			break;
		case 1:
			_tcscpy(dummy.szComment, _T("MC"));
			break;
		case 2:
			_tcscpy(dummy.szComment, _T("LC"));
			break;
		case 3:
			_tcscpy(dummy.szComment, _T("HSLA"));
			break;
		default:
			_tcscpy(dummy.szComment, _T(""));
			break;
		}
	}
	else
	{
		// version > 500
		// read comment data
		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
		{
			fclose(fp);
			return FALSE;
		}
		if( NULL != (p = strrchr(buf, _T('\n'))) )
			*p = _T('\0');
		_tcsncpy(dummy.szComment, buf, 32);

		//// read next data
		//if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
		//{
		//	fclose(fp);
		//	return FALSE;
		//}
	}

	//// if value is normal,
	//if (_stscanf(buf, _T("%d %d"), &dummy.boWatchTime, &dummy.numRegister) == 2)
	//{
	//	// if value is abnormal,
	//	if(dummy.numRegister > NumBoOccur)
	//	{
	//		fclose(fp);
	//		return FALSE;
	//	}

	//	// while  the registration count is no end,
	//	for(i = 0;i < dummy.numRegister;i++)
	//	{
	//		// read data
	//		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
	//		{
	//			fclose(fp);
	//			return FALSE;
	//		}
	//		// if value is abnormal,
	//		if (_stscanf(buf, _T("%f %d"), &dummy.boAlarm[i],&dummy.boTimes[i]) != 2)
	//		{
	//			fclose(fp);
	//			return FALSE;
	//		}
	//	}

		// read data
		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
		{
			dummy.numCombiBA = 1;			// <- The code for the compatibility
		}
		else if (_stscanf(buf, _T("%d"), &dummy.numCombiBA) != 1)
		{
			dummy.numCombiBA = 1;			// <- The code for the compatibility
		}
		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
		{
			dummy.numCombiBAC = 1;			// <- The code for the compatibility
		}
		else if (_stscanf(buf, _T("%d"), &dummy.numCombiBAC) != 1)
		{
			dummy.numCombiBAC = 1;			// <- The code for the compatibility
		}

	//}
	//// if value is abnormal,
	//else
	//{
	//	fclose(fp);
	//	return FALSE;
	//}

	// read config name for BO
	if(_fgetts(buf, 256, fp) == NULL)
	{
		buf[0] = _T('\0');
	}
	if (_stscanf(buf, _T("%s"), &dummy.szConfName) != 1)
	{
		buf[0] = _T('\0');
	}

	// read config name for DEP
	if(_fgetts(buf, 256, fp) == NULL)
	{
		buf[0] = _T('\0');
	}
	if (_stscanf(buf, _T("%s"), &dummy.szConfNameD) != 1)
	{
		buf[0] = _T('\0');
	}
	//// read next data
	//if(_fgetts(buf, 256, fp) == NULL)
	//{
	//	fclose(fp);
	//	return FALSE;
	//}

	//// if value is normal,
	//if (_stscanf(buf, _T("%d"), &dummy.numRegPow) == 1)
	//{
	//	// if value is abnormal,
	//	if(dummy.numRegPow > NumPowder)
	//	{
	//		fclose(fp);
	//		return FALSE;
	//	}

	//	// while  the registration count is no end,
	//	for(i = 0;i < dummy.numRegPow;i++)
	//	{
	//		// read data
	//		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
	//		{
	//			fclose(fp);
	//			return FALSE;
	//		}
	//		// if value is abnormal,
	//		if (_stscanf(buf, _T("%s"), dummy.szPowder[i]) != 1)
	//		{
	//			fclose(fp);
	//			return FALSE;
	//		}
	//	}
	//}
	// if value is abnormal,
	//else
	//{
	//	fclose(fp);
	//	return FALSE;
	//}

	// after reading all lines, close the file.
	fclose(fp);

	// Read Data is adopted.
	memcpy( pInfo, &dummy, sizeof(dummy));

	return TRUE;
}

// save BO system information.
BOOL CConfig::WriteBosysInfo(int sel, BoSysInfo *pInfo)
{
	FILE		*fp;
	_TCHAR		buf[512];
	int			i;

	// make directories.
	_tmkdir(WorkRootDir);

	// make up file name.
	_stprintf(buf, _T("%s") BoSystemFile, WorkRootDir, sel);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("w"))) == NULL)
		return FALSE;

	// write data
	if(_ftprintf(fp, _T("Ver:%d\n"), BOSystemVer) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// write data
	if(_ftprintf(fp, _T("%s\n"), pInfo->szComment) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	//// write data
	//if(_ftprintf(fp, _T("%d %d\n"), pInfo->boWatchTime, pInfo->numRegister) < 0)
	//{
	//	fclose(fp);
	//	return FALSE;
	//}

	//// while  the registration count is no end,
	//for(i = 0;i < pInfo->numRegister;i++)
	//{
	//	// write data
	//	if(_ftprintf(fp, _T("%.2f %d\n"), pInfo->boAlarm[i], pInfo->boTimes[i]) < 0)
	//	{
	//		fclose(fp);
	//		return FALSE;
	//	}
	//}

	if(_ftprintf(fp, _T("%d\n"), pInfo->numCombiBA) < 0)
	{
		fclose(fp);
		return FALSE;
	}
	if(_ftprintf(fp, _T("%d\n"), pInfo->numCombiBAC) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// Ver7.00:
	// write config name
	if(_ftprintf(fp, _T("%s\n"), pInfo->szConfName) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// Ver8.00:
	// write config name
	if(_ftprintf(fp, _T("%s\n"), pInfo->szConfNameD) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	//// Ver7.00:
	//// write data
	//if(_ftprintf(fp, _T("%d\n"), pInfo->numRegPow) < 0)
	//{
	//	fclose(fp);
	//	return FALSE;
	//}

	//// Ver7.00:
	//// while  the registration count is no end,
	//for(i = 0;i < pInfo->numRegPow;i++)
	//{
	//	// write powder name
	//	if(_ftprintf(fp, _T("%s\n"), pInfo->szPowder[i]) < 0)
	//	{
	//		fclose(fp);
	//		return FALSE;
	//	}
	//}

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}

// read BO system information.
BOOL CConfig::ReadBosysCondition(BoCondInfo *pInfo)
{
	FILE		*fp;
	_TCHAR		buf[256];
	BoCondInfo	dummy = {0};

	// make up file name.
	_stprintf(buf, _T("%s") BoSysCondFile, WorkRootDir);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("r"))) == NULL)
	{
		pInfo->fCondLen		= 2.0;
		pInfo->fCondVc		= 0.5;
		WriteBosysCondition(pInfo);
		return FALSE;
	}

	// read data
	if(_fgetts(buf, 256, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// version check
	if( NULL == _tcsstr(buf,_T("Ver:")) )
	{
		fclose(fp);
		return FALSE;
	}
	else
	{
		// version > 500
		// read data
		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
		{
			fclose(fp);
			return FALSE;
		}

		// if value is normal,
		if (_stscanf(buf, _T("%f %f"), &dummy.fCondLen, &dummy.fCondVc) != 2)
		{
			fclose(fp);
			return FALSE;
		}
	}

	// after reading all lines, close the file.
	fclose(fp);

	// Read Data is adopted.
	memcpy( pInfo, &dummy, sizeof(dummy));

	return TRUE;
}

// save BO system information.
BOOL CConfig::WriteBosysCondition(BoCondInfo *pInfo)
{
	FILE		*fp;
	_TCHAR		buf[512];

	// make directories.
	_tmkdir(WorkRootDir);

	// make up file name.
	_stprintf(buf, _T("%s") BoSysCondFile, WorkRootDir);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("w"))) == NULL)
		return FALSE;

	// write data
	if(_ftprintf(fp, _T("Ver:%d\n"), BOSystemVer) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// write data
	if(_ftprintf(fp, _T("%.2f %.2f\n"), pInfo->fCondLen, pInfo->fCondVc) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}

// read BO network information.
BOOL CConfig::ReadBosysNetwork(BoNetInfo *pInfo)
{
	FILE		*fp;
	_TCHAR		buf[256];
	_TCHAR		buf2[256];
	DWORD		dwIP;
	int			st, ip1, ip2, ip3, ip4;
	BoNetInfo	dummy = {0};

	// make up file name.
	_tcscpy(buf, WorkRootDir);
	_tcscat(buf, _T("/../Config"));
	_tcscpy(buf2, buf);
	_tcscat(buf,  BoSysStFile);
	_tcscat(buf2, BoSysNetFile);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("r"))) == NULL)
	{
		pInfo->dwST		= 1;
		pInfo->dwIP		= 0xC0A86464;
		WriteBosysNetwork(pInfo);
		return FALSE;
	}
	// read data
	if(_fgetts(buf, 256, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// if value is normal,
	if (_stscanf(buf, _T("%d"), &st) != 1)
	{
		fclose(fp);
		return FALSE;
	}
	dummy.dwST = st;

	fclose(fp);

	// open the configration file.
	if ((fp = _tfopen(buf2, _T("r"))) == NULL)
	{
		pInfo->dwST		= 1;
		pInfo->dwIP		= 0xC0A86464;
		WriteBosysNetwork(pInfo);
		return FALSE;
	}

	// read data
	if(_fgetts(buf, 256, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// if value is normal,
	if (_stscanf(buf, _T("%d.%d.%d.%d"), &ip1, &ip2, &ip3, &ip4) != 4)
	{
		fclose(fp);
		return FALSE;
	}
	dwIP = MAKEIPADDRESS(ip1, ip2, ip3, ip4);
	dummy.dwIP = htonl(dwIP);

	switch( st )
	{
	case 1:// 1ST
		dummy.dwPortD = 0x1000;
		dummy.dwPortC = 0x1001;
		dummy.dwAddrR = 1000;
		dummy.dwAddrW = 1100;
		break;
	case 2:// 2ST
		dummy.dwPortD = 0x1002;
		dummy.dwPortC = 0x1003;
		dummy.dwAddrR = 2000;
		dummy.dwAddrW = 2100;
		break;
	default:
		dummy.dwPortD = 0x1000;
		dummy.dwPortC = 0x1001;
		dummy.dwAddrR = 1000;
		dummy.dwAddrW = 1100;
		break;
	}

	// after reading all lines, close the file.
	fclose(fp);

	// Read Data is adopted.
	memcpy( pInfo, &dummy, sizeof(dummy));

	return TRUE;
}

// save BO network information.
BOOL CConfig::WriteBosysNetwork(BoNetInfo *pInfo)
{
	FILE		*fp;
	_TCHAR		buf[512];
	DWORD		dwIP;
	int			ip1, ip2, ip3, ip4;

	// make directories.
	_tmkdir(WorkRootDir);

	// make up file name.
	_tcscpy(buf, WorkRootDir);
	_tcscat(buf, _T("/../Config"));
	_tcscat(buf, BoSysNetFile);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("w"))) == NULL)
		return FALSE;

	//// write data
	//if(_ftprintf(fp, _T("%d\n"), pInfo->dwST) < 0)
	//{
	//	fclose(fp);
	//	return FALSE;
	//}

	// get ip data.
	dwIP = ntohl(pInfo->dwIP);
	ip1 = FIRST_IPADDRESS(dwIP);
	ip2 = SECOND_IPADDRESS(dwIP);
	ip3 = THIRD_IPADDRESS(dwIP);
	ip4 = FOURTH_IPADDRESS(dwIP);

	// write data
	if(_ftprintf(fp, _T("%d.%d.%d.%d\n"), ip1, ip2, ip3, ip4) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}
