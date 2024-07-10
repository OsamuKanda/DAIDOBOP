///////////////////////////////////////////////////////////////////////////////////
//	AJ71QE71 
//	File Name	�F Aj71Qe.h
//	Contents	�F Defined Transmit Data
//				�F
//	Trend		�F2001/04/16 Create ,K.Tanno
//				�F
//				�F
///////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------
// #define�@��`
// -------------------------------------------------------------------------
#define	LEN_BASIC_CMD		12		// �R�}���h��{�v���f�[�^��
#define	LEN_BASIC_RES		2		// ���X�|���X��{�v���f�[�^��
#define	LEN_DEVNO			3		// �f�o�C�X�m�n�o�b�t�@��
#define	LEN_DEVCNT			2		// �_���o�b�t�@��
#define LEN_ENDCODE			2		// �I���R�[�h��

#define POS_DATA_CMD		21		// �R�}���h�f�[�^�Z�b�g�ʒu
#define POS_DATA_RES		11		// ���X�|���X�f�[�^�擾�ʒu

#define	DEV_D				0xA8	// �f�o�C�X�R�[�h�c
#define	DEV_R				0xAF	// �f�o�C�X�R�[�h�q
#define	DEV_W				0xB4	// �f�o�C�X�R�[�h�v

typedef struct
{
	BYTE	nSub[2],				// �T�u�w�b�_
			nNetNo,					// �l�b�g���[�N�m�n
			nPcNo,					// �o�b�m�n
			nFix1[3],				// �Œ�l
			nDataLen[2],			// �v���f�[�^��
			nCpuTime[2],			// �b�o�t�Ď��^�C�}
			nCmd[2],				// �R�}���h
			nSubCmd[2],				// �T�u�R�}���h
			nDevNo[LEN_DEVNO],		// �擪�f�o�C�X
			nDevCode,				// �f�o�C�X�R�[�h
			nDevCount[LEN_DEVCNT];	// �f�o�C�X�_��
}CMD_MSG,*PCMD_MSG;

typedef struct
{
	BYTE	nSub[2],				// �T�u�w�b�_
			nNetNo,					// �l�b�g���[�N�m�n
			nPcNo,					// �o�b�m�n
			nFix1[3],				// �Œ�l
			nDataLen[2],			// �v���f�[�^��
			nEndCode[2];			// �I���R�[�h
}RES_MSG,*PRES_MSG;

// PC�ް�(���l)��PLC�ް�(�޲��ް��̔z��)�ɕϊ�����B
inline void PcToPlc(UINT uiPcData,LPBYTE lpData,int iDataLen)
{
															// �����֐������ϐ���`����
	UINT	uiWork,											// PC�ް���߰�̈�
			uiShiftSize;									// ��Ļ���
	int		iCnt;											// ���ޯ��
															// ����PLC�ް��ɕϊ�����
	uiWork = uiPcData;										// PC�ް����߰
	uiShiftSize = sizeof(BYTE) * 8;							// �ް����Ă��黲�ނ��
	if(iDataLen > 1)										// �ް�����1���傫���ꍇ
	{
		for(iCnt = iDataLen - 1;iCnt > 0;iCnt--)			// PLC�ް��ϊ�
		{
			lpData[iCnt] = (BYTE)(uiWork >> (uiShiftSize * iCnt));	// ��ʂ��珇�Ԃ��ް����擾����
			uiWork -= (lpData[iCnt] << (uiShiftSize * iCnt));
		}
	}
	lpData[0] = (BYTE)uiWork;										// �ŉ��ʂ��ް����
}

// PLC�ް�(�޲��ް��̔z��)��PC�ް�(���l)�ɕϊ�����B
inline UINT PlcToPc(LPBYTE	lpData,int iDataLen)
{
															// �����֐������ϐ���`����
	UINT	uiRet,											// �߂�l
			uiShiftSize;									// ��Ļ���
	int		iCnt;											// ���ޯ��
															// ����PC�ް��ɕϊ�����
	uiShiftSize = sizeof(BYTE) * 8;							// �ް����Ă��黲�ނ��
	if(iDataLen > 1)										// �ް�����1���傫���ꍇ
	{
		uiRet = lpData[iDataLen - 1];						// ��ʂ̒l���
		for(iCnt = iDataLen - 1;iCnt > 0;iCnt--)			// PC�ް��ɕϊ�
			uiRet = (uiRet << uiShiftSize) + lpData[iCnt - 1];
	}
	else													// �ް�����1�ȉ��̏ꍇ
		uiRet = *lpData;									// 1�ް����

	return uiRet;											// �ϊ������ް���Ԃ�
}

// ���[�h�ꊇ�����݃R�}���h�f�[�^�Z�b�g(�߂�l�̓f�[�^�Z�b�g�ʒu)
inline int SetWriteCmdW( LPBYTE pbyData, BYTE nDev, UINT uiDevNo, WORD wLen)
{
	PCMD_MSG	pcm = (PCMD_MSG)pbyData;
	UINT	uiDataLen;

	pcm->nSub[0]		= 0x50;					// �T�u�R�}���h
	pcm->nSub[1]		= 0x00;					// ��
	pcm->nNetNo			= 0x00;					// �l�b�g�m�n
	pcm->nPcNo			= 0xFF;					// �o�b�m�n
	pcm->nFix1[0]		= 0xFF;					// �Œ�l
	pcm->nFix1[1]		= 0x03;					// ��
	pcm->nFix1[2]		= 0x00;					// ��
	uiDataLen =  LEN_BASIC_CMD + wLen * 2;		// �v���f�[�^��
	PcToPlc( uiDataLen, pcm->nDataLen, 2);		// ��
	pcm->nCpuTime[0]	= 0x10;					// �b�o�t�Ď��^�C�}
	pcm->nCpuTime[1]	= 0x00;					// ��
	pcm->nCmd[0]		= 0x01;					// �R�}���h
	pcm->nCmd[1]		= 0x14;					// ��
	pcm->nSubCmd[0]		= 0x00;					// �T�u�R�}���h
	pcm->nSubCmd[1]		= 0x00;					// ��
	PcToPlc( uiDevNo, pcm->nDevNo, LEN_DEVNO);	// �f�o�C�X�m�n
	pcm->nDevCode		= nDev;					// �f�o�C�X�R�[�h
	PcToPlc( wLen, pcm->nDevCount, LEN_DEVCNT);	// �_��

	return (int)(POS_DATA_CMD + wLen * 2);
}

// ���[�h�ꊇ�Ǎ��݃R�}���h�f�[�^�Z�b�g
inline int SetReadCmdW( LPBYTE pbyData, BYTE nDev, UINT uiDevNo, WORD wLen)
{
	PCMD_MSG	pcm = (PCMD_MSG)pbyData;
	UINT		uiDataLen;

	pcm->nSub[0]		= 0x50;					// �T�u�R�}���h
	pcm->nSub[1]		= 0x00;					// ��
	pcm->nNetNo			= 0x00;					// �l�b�g�m�n
	pcm->nPcNo			= 0xFF;					// �o�b�m�n
	pcm->nFix1[0]		= 0xFF;					// �Œ�l
	pcm->nFix1[1]		= 0x03;					// ��
	pcm->nFix1[2]		= 0x00;					// ��
	uiDataLen =  LEN_BASIC_CMD;					// �v���f�[�^��
	PcToPlc( uiDataLen, pcm->nDataLen, 2);		// ��
	pcm->nCpuTime[0]	= 0x10;					// �b�o�t�Ď��^�C�}
	pcm->nCpuTime[1]	= 0x00;					// ��
	pcm->nCmd[0]		= 0x01;					// �R�}���h
	pcm->nCmd[1]		= 0x04;					// ��
	pcm->nSubCmd[0]		= 0x00;					// �T�u�R�}���h
	pcm->nSubCmd[1]		= 0x00;					// ��
	PcToPlc( uiDevNo, pcm->nDevNo, LEN_DEVNO);	// �f�o�C�X�m�n
	pcm->nDevCode		= nDev;					// �f�o�C�X�R�[�h
	PcToPlc( wLen, pcm->nDevCount, LEN_DEVCNT);	// �_��

	return (int)(POS_DATA_CMD);
}

// ���[�h�ꊇ�����݃��X�|���X�f�[�^�擾(�`�F�b�N�L��)
inline int GetWriteResW( LPBYTE pbyData )
{
	PRES_MSG	pcm = (PRES_MSG)pbyData;
	UINT		uiData;

	// �T�u�w�b�_�`�F�b�N
	uiData = PlcToPc( pcm->nSub, 2);
	if(	0xD0 != uiData)
		return -1;

	// �l�b�g�m�n�`�F�b�N
	if( 0x00 != pcm->nNetNo )
		return -2;

	// �o�b�m�n�`�F�b�N
	if( 0xFF != pcm->nPcNo )
		return -2;

	// �v���f�[�^���`�F�b�N
	uiData = PlcToPc( pcm->nDataLen, 2);
	if(	UINT(LEN_BASIC_RES) != uiData)
		return -3;

	// �I���R�[�h�`�F�b�N
	uiData = PlcToPc( pcm->nEndCode, LEN_ENDCODE);
	if(	UINT(0x00) != uiData)
		return -4;

	return (int)(POS_DATA_RES);
}

// ���[�h�ꊇ�Ǎ��݃��X�|���X�f�[�^�擾(�`�F�b�N�L��)
inline int GetReadResW( LPBYTE pbyData, WORD wLen)
{
	PRES_MSG	pcm = (PRES_MSG)pbyData;
	UINT		uiData;

	// �T�u�w�b�_�`�F�b�N
	uiData = PlcToPc( pcm->nSub, 2);
	if(	0xD0 != uiData)
		return -1;

	// �l�b�g�m�n�`�F�b�N
	if( 0x00 != pcm->nNetNo )
		return -2;

	// �o�b�m�n�`�F�b�N
	if( 0xFF != pcm->nPcNo )
		return -2;

	// �v���f�[�^���`�F�b�N
	uiData = PlcToPc( pcm->nDataLen, 2);
	if(	UINT(LEN_BASIC_RES + wLen) != uiData)
		return -3;

	// �I���R�[�h�`�F�b�N
	uiData = PlcToPc( pcm->nEndCode, LEN_ENDCODE);
	if(	UINT(0x00) != uiData)
		return -4;

	return (int)(POS_DATA_RES);
}

// �I���R�[�h�擾
inline int GetEndCode( LPBYTE pbyData )
{
	PRES_MSG	pcm = (PRES_MSG)pbyData;

	return (int)(PlcToPc( pcm->nEndCode, LEN_ENDCODE));
}


