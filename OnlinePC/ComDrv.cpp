#include	"stdafx.h"
#include	"OnlinePC.h"

// select sample dummy
// 1 : randam data.
// 2 : file data.
#define	USE_SAMPLE_DUMMY	2

#include    "ComDrv.h"
#include	"Caio.h"

///////////////////////////////////////////////////////////////////////////////
// definition
#define	VC_OFF_DELAY_MAX	30000		// [ms]
#define	NUM_PTN_NO			NumSteelKind


//// �p�^�[��No��ݒ�
//void SetPatternNo(ViewData *pVd);


///////////////////////////////////////////////////////////////////////////////
// realtime class function

CComDrv::CComDrv()
{
	m_isFirst = TRUE;
	m_nDevCntR = 0;
	m_nDevCntW = 0;
	m_nVcOffCnt = (VC_OFF_DELAY_MAX / SamplingPeriod);
	memset(&m_stData, 0, sizeof(m_stData));
	memset(&m_stOldData, 0, sizeof(m_stOldData));
	m_nID = -1;

	// ��񏉊���
	m_bPreVcOn			= FALSE;
	m_bPreChargeChg		= FALSE;
	m_bPreCastStart		= FALSE;
	m_bSteelInfoFix		= FALSE;
	m_nFixPtnNo			= 0;
	memset(m_cFixGradeCode,  0, sizeof(m_cFixGradeCode));
	memset(m_cFixHeatNo,	 0, sizeof(m_cFixHeatNo));
}

CComDrv::~CComDrv()
{
}


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig			&cnf	= theApp.m_cnfBoexe;
static ADTranslation*	adTranslation = theApp.m_cnfBoexe.m_adTranslation;


// initialize Siemens PLC Communication
int CComDrv::Initialize(HWND hWnd)
{
	int			ret = 0;

	return ret;
}

//=============================================================================
// Name		: ReadInputData
// Argument	: none.
// Return	: none.
// Comment	: read input data.
// JP		: ���̓f�[�^����
//=============================================================================
int CComDrv::ReadInputData( StComInParam *pPrm, StComInResult *pRst )
{
	int			inAlarmResetExt = 0;
	ViewData	*vd = GetSharedMemory2();

	int		i, ch, ret;

	// get shared memory data
	///////////////////////////////////////////////////////////////////
	ViewLock();

	m_stData.vd = *vd;

	ViewUnlock();
	///////////////////////////////////////////////////////////////////

#ifndef OFFLINE
	// ���̓f�[�^������
	memset(&m_stData.sd, 0, sizeof(m_stData.sd));

	// ���x�f�[�^�擾
	for(i = 0; i < NumTCTap; i++ )
		m_stData.sd.val[i] = m_stData.vd.nTemp[i];

	// ���x���擾
	ch = cnf.inputPoint[LevelID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdLevel;

	// �������x�擾
	ch = cnf.inputPoint[VcID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nCastSpeed;

	// SN�J�x�擾
	ch = cnf.inputPoint[SNID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nSnOpen;

	// ���[���h��
	//ch = cnf.inputPoint[WidthID].adCh;
	//if( ch != -1 )
	//	m_stData.sd.val[ch] = m_stData.vd.nMdWidth;

	// ������
	ch = cnf.inputPoint[LengthID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nCastLengthC;

	// TD�d��
	ch = cnf.inputPoint[TdWeiID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nTdWeidht;

	// MD��p������A
	ch = cnf.inputPoint[MdWtAID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtA;

	// MD��p������C
	ch = cnf.inputPoint[MdWtCID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtC;

	// MD��p������D
	ch = cnf.inputPoint[MdWtDID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtD;

	// MD��p�����xD
	ch = cnf.inputPoint[MdTmpDID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtTempD;

	// �M���f�[�^
	pRst->tstVcOn			= m_stData.vd.isPrOn;
	pRst->tstChargeChange	= m_stData.vd.isCharge;
	pRst->tstAutoLVCtl		= m_stData.vd.isAutoLvCtrl;
	pRst->tstCastStart		= m_stData.vd.isHeatDataChg;
	pRst->tstL1Err			= m_stData.vd.isL1Err;
	inAlarmResetExt			= m_stData.vd.isBoRst;

	// Vc Off delay
	if( !pRst->tstVcOn && m_nVcOffCnt < (VC_OFF_DELAY_MAX / SamplingPeriod) )
	{
		pRst->tstVcOn = 1;
		m_nVcOffCnt++;
	}
	else if( pRst->tstVcOn )
	{
		m_nVcOffCnt = 0;
	}
	
#else//OFFLINE
	// set sampling dummy data.
	SetSampleDummy();
	pRst->tstVcOn			= m_stData.vd.isPrOn;
	pRst->tstChargeChange	= m_stData.vd.isCharge;
	pRst->tstAutoLVCtl		= m_stData.vd.isAutoLvCtrl;
	pRst->tstCastStart		= m_stData.vd.isHeatDataChg;
	pRst->tstL1Err			= m_stData.vd.isL1Err;
	inAlarmResetExt			= m_stData.vd.isBoRst;
	//pRst->tstVcOn = 0;
	//pRst->tstChargeChange = 0;
	//pRst->tstAutoLVCtl = 0;
	//pRst->tstCastStart = 0;
#endif//OFFLINE

	// if the simulation mode is enabled,
	if(pPrm->isEnabSimSignal)
	{
		// sample each signal and generate OR with simulation request.
		pRst->isVcOn		 = pRst->tstVcOn		 || pPrm->simVcOn;
		pRst->isChargeChange = pRst->tstChargeChange || pPrm->simChargeChange;
		pRst->isAutoLVCtl	 = pRst->tstAutoLVCtl	 || pPrm->simAutoLVCtl;
		pRst->isCastStart	 = pRst->tstCastStart	 || pPrm->simCastStart;
		pRst->isL1Err		 = pRst->tstL1Err		 || pPrm->simL1Err;
		pRst->outAlarmReset  = inAlarmResetExt		 || pPrm->inAlarmReset;
	}
	// other wise,
	else {
		// sample the input signals.
		pRst->isVcOn		 = pRst->tstVcOn;		
		pRst->isChargeChange = pRst->tstChargeChange;
		pRst->isAutoLVCtl	 = pRst->tstAutoLVCtl;
		pRst->isCastStart	 = pRst->tstCastStart;
		pRst->isL1Err		 = pRst->tstL1Err;
		pRst->outAlarmReset  = inAlarmResetExt || pPrm->inAlarmReset;
	}
	pRst->inPlcErr1  = m_stData.vd.nPlcErr1;
	pRst->inPlcErr2  = m_stData.vd.nPlcErr2;

	// '11/04/04 �|����FIX�������폜
	////-----------------------------------------------------------------
	//// �|����FIX����

	//// �����J�n ON�̗����オ��ō|����FIX��ON
	//// �|����FIX��OFF�Œ����J�n�����オ��Ȃ�|����FIX��ON
	//// �|����FIX��OFF�Ń`���[�W�������オ��Ȃ�|����FIX��ON
	//if( (!m_bPreCastStart   && pRst->isCastStart) ||
	//	(!m_bSteelInfoFix && !m_bPreVcOn && pRst->isVcOn) ||
	//	(!m_bSteelInfoFix && !m_bPreChargeChg && pRst->isChargeChange) )
	//{
	//	m_nFixPtnNo = m_stData.vd.nPtnNo;
	//	memcpy(m_cFixGradeCode,  m_stData.vd.cGradeCode,	sizeof(m_cFixGradeCode));
	//	memcpy(m_cFixHeatNo,	 m_stData.vd.cHeatNo,		sizeof(m_cFixHeatNo));
	//	memcpy(m_cFixStandardNo, m_stData.vd.cStandardNo,	sizeof(m_cFixStandardNo));
	//	m_bSteelInfoFix = TRUE;
	//}

	//// �����I����FIX���N���A
	//if( m_bPreVcOn && !pRst->isVcOn )
	//{
	//	m_bSteelInfoFix = FALSE;
	//}

	//// �|����FIX��ON�Ȃ�
	//if( m_bSteelInfoFix )
	//{
	//	// �Œ�l�ɏ���������
	//	m_stData.vd.nPtnNo = m_nFixPtnNo;
	//	memcpy(m_stData.vd.cGradeCode,	m_cFixGradeCode,	sizeof(m_cFixGradeCode));
	//	memcpy(m_stData.vd.cHeatNo,		m_cFixHeatNo,		sizeof(m_cFixHeatNo));
	//	memcpy(m_stData.vd.cStandardNo,	m_cFixStandardNo,	sizeof(m_cFixStandardNo));
	//}

	// �p�^�[��No�ݒ�
//	SetPatternNo(&m_stData.vd);

	// �O��l�L��
	m_bPreVcOn		= pRst->isVcOn;
	m_bPreChargeChg = pRst->isChargeChange;
	m_bPreCastStart	= pRst->isCastStart;

	return 0;
}

// �o�̓f�[�^����������
int CComDrv::WriteOutputData( StComOutParam *pPrm, StComOutResult *pRst )
{
	short	out;

	switch(pPrm->pulseOutBOFetched)
	{
	case 0x00:
		// BO�o�͏�Ԃ�ر����.
		pRst->OutBOFetched = 0x00;
		break;
	case 0x01:
		// BO�x��o�͂�ݒ肷��.
		// asert the signal.
		pRst->OutBOFetched = 0x01;
		break;
	case 0x02:
		// BO�f�v���b�V�����x��o�͂�ݒ肷��.
		// asert the signal.
		pRst->OutBOFetched = 0x08;
		break;
	case 0x03:
		// BO���f�v���b�V�����x��o�͂�ݒ肷��.
		// asert the signal.
		pRst->OutBOFetched = 0x09;
		break;
	default:
		pRst->OutBOFetched = 0x00;
		break;
	}

	//// �P�ƃ��[�h�Ȃ�
	//if(pPrm->isEnabSimSignal)
	//{
	//	// DO�o��
	//	out = (((pRst->OutBOFetched & 0x08) == 0x08) || pPrm->simBocOut);
	//	AioOutputDoBit(m_nID, 0, out);
	//	out = (((pRst->OutBOFetched & 0x01) == 0x01) || pPrm->simBoOut);
	//	AioOutputDoBit(m_nID, 1, out);
	//}
	//// �ʏ탂�[�h�Ȃ�
	//else
	//{
	//	// DO�o��
	//	out = ((pRst->OutBOFetched & 0x08) == 0x08);
	//	AioOutputDoBit(m_nID, 0, out);
	//	out = ((pRst->OutBOFetched & 0x01) == 0x01);
	//	AioOutputDoBit(m_nID, 1, out);
	//}
	return 0;
}

// uninitialize Siemens PLC Communication
int CComDrv::Uninitialize( void )
{

	return 0;
}

// get sample data from A/D channel. asm version
void CComDrv::GetSampledData(StSampledParam *pPrm, StSampledResult *pRst, StSampledData *pData)
{
	static ADTranslation far *ap;
	float	fVal, fFilter;
	int		nWdData = 0, nLenData = 0;
	int		i;

	// first data only
	if( m_isFirst )
	{
		m_stOldData = m_stData;
		m_isFirst	= FALSE;
	}

	// for each A/D channel,
	for(i = 0, ap = adTranslation; i < NumADChannel; i++, ap++)
	{
		// if this channel is not configured, do nothing.
		if(!ap->isValid)	continue;

		// raw data
		m_stData.sd_raw.val[i] = m_stData.sd.val[i];

		// scale the input.
		if(pPrm->filterSwitch)
		{	
			switch(ap->arg)
			{
			case SNID:		// SN Channel No.
				fFilter = pPrm->filterFacterSn;		break;
			case LevelID:	// Level Channel No.
				fFilter = pPrm->filterFacterLv;		break;
			case VcID: 		// P/R Speed Channel No.
				fFilter = pPrm->filterFacterPr;		break;
			//case WidthID:	// Width Channel No.
			//case WidthLID:	// Width Left Channel No.
			//case WidthRID:	// Width Right Channel No.
//				fFilter = pPrm->filterFacterWd;		break;
			case LengthID:	// Length Channel No.
				fFilter = 1.0;						break;
			default:		// Temp Channel No.
				fFilter = pPrm->filterFacter;		break;
			}
			fVal =	(float)m_stOldData.sd.val[i] * (1.0f - fFilter) +
					(float)m_stData.sd.val[i]	 * fFilter;
			m_stOldData.sd.val[i] = (short)fVal;
		}
	}

	//// MOLD LEVEL
	//if ( m_stData.sd.val[LevelID] <= m_stOldData.sd.val[LevelID] )	 
	//	pRst->MoldLevelNotUp = TRUE;	// MOLD LEVEL �p���㏸���Ŗ���
	//else
	//	pRst->MoldLevelNotUp = FALSE;	// MOLD LEVEL �p���㏸��

	//// Vc Speed 
	//if ( m_stData.sd.val[VcID] <= m_stOldData.sd.val[VcID] ) 
	//	pRst->VcSpeedNotUp = TRUE;		// Vc �p���㏸���Ŗ���
	//else
	//	pRst->VcSpeedNotUp = FALSE;		// Vc �p���㏸��

	//// SN LEVEL
	//if ( m_stData.sd.val[SNID] >= m_stOldData.sd.val[SNID] ) 
	//	pRst->SNLevelUp = TRUE;			// SN LEVEL �p���㏸��
	//else
	//	pRst->SNLevelUp = FALSE;		// SN LEVEL �p���㏸���Ŗ���

	// filter mold cooling water.
	if(pPrm->filterSwitch)
	{	
		// set filter value.
		*pData						= m_stData; 
		pData->sd					= m_stOldData.sd; 
	}
	else
	{
		m_stOldData	= m_stData; 
		*pData		= m_stData; 
	}

	//pRst->moldSizeL = m_stData.sd.val[WidthLID];
	//pRst->moldSizeR = m_stData.sd.val[WidthRID];
	//pRst->moldSizeL = m_stData.sd.val[WidthID]/2;
	//pRst->moldSizeR = m_stData.sd.val[WidthID]/2;
	//pRst->moldSize = pRst->moldSizeL + pRst->moldSizeR;
}

#ifdef USE_SAMPLE_DUMMY
// set sample dummy data.
void CComDrv::SetSampleDummy(void)
{
// randam data.
#if USE_SAMPLE_DUMMY == 1
	static int cnt = 0;
	static int ran = 10;
	int	i, ch;
	cnt++;
	if( cnt > 10 ){
		cnt = 0;
		ran = rand()%100;
	}
	int test[] = {0,100,200,300,400,500,600,700,800,900,1000,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,1200,1100};
//	for( int i = 0; i < 72/3; i++ )
//		for( int j = 0; j < 3; j++ ){
//			m_stData.sd.val[i*3+j] = test[i];
//		}
//	for( int i = 72; i < 80; i++ )
//		m_stData.sd.val[i] = rand()%100+800;
	static int a = 0;
	a++;
	// ���x�f�[�^�擾
	for(i = 0; i < NumTCTap; i++ )
		m_stData.sd.val[i] = m_stData.vd.nTemp[i];

	// ���x���擾
	ch = cnf.inputPoint[LevelID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdLevel;

	// �������x�擾
	ch = cnf.inputPoint[VcID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nCastSpeed;

	// SN�J�x�擾
	ch = cnf.inputPoint[SNID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nSnOpen;

	// ������
	ch = cnf.inputPoint[LengthID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nCastLengthC;

	// TD�d��
	ch = cnf.inputPoint[TdWeiID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nTdWeidht;

	// MD��p������A
	ch = cnf.inputPoint[MdWtAID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtA;

	// MD��p������C
	ch = cnf.inputPoint[MdWtCID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtC;

	// MD��p������D
	ch = cnf.inputPoint[MdWtDID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtD;

	// MD��p�����xD
	ch = cnf.inputPoint[MdTmpDID].adCh;
	if( ch != -1 )
		m_stData.sd.val[ch] = m_stData.vd.nMdCoolWtTempD;

	BYTE	*buf = (BYTE*)"ABCD1234567";
	memcpy(m_stData.vd.cGradeCode, buf, sizeof(m_stData.vd.cGradeCode));
	memcpy(m_stData.vd.cHeatNo, buf, sizeof(m_stData.vd.cHeatNo));
	m_stData.vd.nMdLevel		= (rand()%10 + -100) * 10;

	//// ������
	//static int isPrOn = 0, isCharge = 0;
	//static int CastLen = 0;

	//if( m_stData.vd.isPrOn )
	//	CastLen += (int)((m_stData.sd.val[cnf.inputPoint[VcID].adCh] * 0.01)*(0.5/60.0)*100.0);
	//else
	//	CastLen = 0;

	//if( (m_stData.vd.isCharge && isCharge == 0) )
	//{
	//	CastLen = 0;
	//}
	//CastLen	= 500;
	//m_stData.sd.val[cnf.inputPoint[LengthID].adCh] = CastLen;
	//m_stData.vd.nCastLengthC = CastLen;

	//m_stData.vd.nSnOpen = 750 + rand()%10;
	//m_stData.sd.val[cnf.inputPoint[SNID].adCh] = m_stData.vd.nSnOpen;

	//m_stData.sd.val[cnf.inputPoint[VcID].adCh] = 500;
	//m_stData.vd.nMdLevel = -995 + rand()%10*10;
	//m_stData.sd.val[cnf.inputPoint[LevelID].adCh] = m_stData.vd.nMdLevel;


#endif//USE_SAMPLE_DUMMY==1

// file data.
#if USE_SAMPLE_DUMMY == 2
	static int sw = 0;
	static FILE *fp = NULL;
	static DataFileHeader hd;
	static int wcnt = 0;
	_TCHAR	path[MAX_PATH];
	BYTE	*buf = (BYTE*)"                    ";
	int		nCnt;
	int		i, ch;
	switch(sw)
	{
	case 0:
		_stprintf(path, _T("%s\\..\\00000000.000"), theApp.m_log.LogRootDir );
		fp = _tfopen(path,_T("rb"));
		if( fp != NULL )
		{	
			fread(&hd, sizeof(hd), 1, fp);
			sw = 1;
		}
//		break;
	case 1:
		memset(&m_stData.sd, 0, sizeof(m_stData.sd));
#ifdef Compatibility
		if( hd.header.version <= 500 ){
			nCnt = fread(&m_stData.sd, sizeof(SampledDataV5), 1, fp);
			for( i = NumADChannelV5; i < NumADChannel; i++ )
				m_stData.sd.val[i] = rand()%100 + 100;
		}
		else{
			nCnt = fread(&m_stData.sd, sizeof(SampledData), 1, fp);
		}
#else
		nCnt = fread(&m_stData.sd, sizeof(SampledData), 1, fp);
#endif
		memcpy(m_stData.vd.cGradeCode,	buf, sizeof(m_stData.vd.cGradeCode));
		memcpy(m_stData.vd.cHeatNo,	buf, sizeof(m_stData.vd.cHeatNo));
		memcpy(m_stData.vd.cStandardNo, buf, sizeof(m_stData.vd.cStandardNo));
		m_stData.vd.nTdWeidht		= (short)(rand()%10 + 1000 * 0.5);
		m_stData.vd.nCastLengthS	= (short)(rand()%100 + 30000 * 0.5);
		m_stData.vd.nMdCoolWtA		= (short)(rand()%10 + 1000 * 0.5);
		m_stData.vd.nMdCoolWtC		= (short)(rand()%10 + 1000 * 0.5);
		m_stData.vd.nMdCoolWtD		= (short)(rand()%10 + 1000 * 0.5);
		m_stData.vd.nMdCoolWtTempD	= (short)(rand()%10 + 1000 * 0.5);

		// get vc
		ch = cnf.inputPoint[VcID].adCh;
		if( ch != -1 )
			m_stData.vd.nCastSpeed = m_stData.sd.val[ch];
		else
			m_stData.vd.nCastSpeed = 0;

		// get vc
		ch = cnf.inputPoint[LevelID].adCh;
		if( ch != -1 )
			m_stData.vd.nMdLevel = m_stData.sd.val[ch];
		else
			m_stData.vd.nMdLevel = 0;

//		m_stData.vd.isAutoLvCtrl	= 1;

		if( nCnt < 1 ) {
			sw = 2;
			fclose(fp);
		}
		break;
	case 2:
		if( ++wcnt > 20 )
		{
			sw = 0;
			wcnt = 0;
		}
		break;
	}
#endif//USE_SAMPLE_DUMMY==2

}
#endif//USE_SAMPLE_DUMMY

//// Bosys���̃p�E�_�[�����`�F�b�N���p�^�[��No��ݒ�
//void SetPatternNo(ViewData *pVd)
//{
//	BoSysInfo	stBsInfo[NUM_PTN_NO];		
//	int			i, j, n, len, len2;
//
//	// �����̒������`�F�b�N
//	for( i = 0; i < 10; i++ )
//		if( pVd->cFluxPowder[i] == 0 )
//			break;
//	len = i;
//
//	// Bosys����ǂݍ���
//	for( i = 0; i < NUM_PTN_NO; i++ )
//	{
//		// Bosys information is read.
//		if(FALSE == cnf.ReadBosysInfo(i, &stBsInfo[i]))
//		{
//			// �ُ�Ȃ牽�����Ȃ�
//			return;
//		}
//	}
//
//	// �S�|��O���[�v�ɂ���
//	for( n = 0; n < NUM_PTN_NO; n++ )
//	{
//		// �o�^���ꂽ�p�E�_�[�ɂ���
//		for( i = 0; i < stBsInfo[n].numRegPow; i++ )
//		{
//			// �����̒������`�F�b�N
//			for( j = 0; j < 32; j++ )
//				if( stBsInfo[n].szPowder[i][j] == 0 )
//					break;
//			len2 = j;
//
//			// ���݂̃p�E�_�[���ƈ�v���邩�`�F�b�N
//			if( len == len2 && memcmp( pVd->cFluxPowder, stBsInfo[n].szPowder[i], sizeof(BYTE)*len ) == 0 )
//			{
//				// ��v������p�^�[��No��ݒ�
//				pVd->nPtnNo = n;
//				return; 
//			}
//		}
//	}
//}
