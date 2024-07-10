#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"BoGroup.h"

//int testtest = 0;

// tcno->��No�ϊ��e�[�u��
static int tc2line[28] = 
{
	0,	0,
	1,	1,
	2,	2,
	3,	3,	3,
	4,	4,
	5,	5,	5,
	6,	6,
	7,	7,
	8,	8,
	9,	9,	9,
	10,	10,
	11,	11,	11
};

// ��No->tcno�ϊ��e�[�u��
static int line2tc[12][3] = 
{
	{0,	1, -1},
	{2,	3, -1},
	{4,	5, -1},
	{6,	7,	8},
	{9,10, -1},
	{11,12,13},
	{14,15,-1},
	{16,17,-1},
	{18,19,-1},
	{20,21,22},
	{23,24,-1},
	{25,26,27}
};

///////////////////////////////////////////////////////////////////////////////
// definition

void CBoGroup::CreateGroup(CConfig &cnf)
{
	TCCombination *cp;
	int		i, j, k, idx;
	int		dumy_max;
	GrInfo	dumy_Gr[TCEditNum];
	BOOL	dumyTCOkNg[TCEditNum];
	BOOL	iTCOkNg[TCEditNum];

	// �����l��ݒ�
	memset( iTCOkNg, 0, sizeof(iTCOkNg) );
	for ( i = 0; i < TCEditNum; i++ )
		iTCOkNg[i] = TRUE;

	// TC��ɔM�d�΃R���r������U���Ă��邩�`�F�b�N
	memset( dumyTCOkNg, 0, sizeof(dumyTCOkNg) );
	for( i = 0; i < cnf.numCombination; i++)
	{
		if( cnf.inputPoint[cnf.tcCombination[i].upper].adCh != -1 && 
			cnf.inputPoint[cnf.tcCombination[i].lower].adCh != -1 )
		{
			dumyTCOkNg[tc2line[cnf.tcCombination[i].upper]] = TRUE;
			dumyTCOkNg[tc2line[cnf.tcCombination[i].lower]] = TRUE;
		}
	}
	for ( i = 0; i < TCEditNum; i++ )
		iTCOkNg[i] &= dumyTCOkNg[i];

	// �eTC��̍��E�̗������
	memset( m_iTCLR, -1, sizeof( m_iTCLR ));
	for( k = 0; k < TCEditNum; k++ )
	{
		//�@�E������
		for( i = 0; i < TCEditNum; i++ )
		{
			idx = i + k;
			if( idx >= TCEditNum ) idx = 0;
			if( idx != k && iTCOkNg[idx] == 1 ){
				m_iTCLR[k][idR] = idx;break;
			}
		}
		//�@��������
		for( i = 0; i < TCEditNum; i++ )
		{
			idx = -i + k;
			if( idx < 0 ) idx = idx + TCEditNum;
			if( idx != k && iTCOkNg[idx] == 1 ){
				m_iTCLR[k][idL] = idx;break;
			}
		}
	}

	// �O���[�v��񏉊���
	memset(dumy_Gr, 0, sizeof(dumy_Gr));

	// �S��ɑ΂���
	for( i = 0, dumy_max = 0; i < TCEditNum; i++ )
	{
		// �Ď��ΏۂȂ�
		if( m_iTCLR[i][idR] != -1 && m_iTCLR[i][idL] != -1 )
		{
			dumy_Gr[dumy_max].cnt = 0;
			dumy_Gr[dumy_max].column = i;
			// ���E�̗񂩂�R�U�ʂ�̑g�ݍ��킹���쐬
			SetCombiPtn( i, m_iTCLR[i][idR], m_iTCLR[i][idL]);
			// �S�R���r�l�[�V�����ɑ΂���
			for( j = 0, cp = cnf.tcCombination; j < cnf.numCombination; j++, cp++ )
			{
				// �O���[�v���ɑ��݂���Ȃ�
				if( TRUE == CheckCombiPtn( cp->upper, cp->lower) ) 
				{
					// �Y�������L������
					dumy_Gr[dumy_max].no[dumy_Gr[dumy_max].cnt++] = j;
				}
			}
			dumy_max++;
		}
	}

	m_nGrMax = dumy_max;
	memcpy( m_stGr, dumy_Gr, sizeof(m_stGr));

#ifdef TEST_GROUP_OUT
	if( testtest == 1 )
	{
	FILE *fp;
	_TCHAR buf[100];
	fp = fopen(_T("c:\\BOP\\test.txt"), _T("w+"));
	for(i = 0;i < m_nGrMax;i++)
	{
		_stprintf(buf, _T("\nGroup%d"), i+1);
		fwrite(buf, _tcslen(buf), 1, fp);
		for(j = 0;j < m_stGr[i].cnt;j++)
		{
			_stprintf( buf, _T(" <#%d,#%d>"), cnf.tcCombination[m_stGr[i].no[j]].upper+1,cnf.tcCombination[m_stGr[i].no[j]].lower+1);
			fwrite(buf,strlen(buf),1,fp);
		}
		sprintf(buf, _T("\n"));
		fwrite(buf, _tcslen(buf),1,fp);
	}
	fclose(fp);
	testtest = 0;
	}
#endif
}

// �O�̔M�d�Η�ԍ�����X�̔M�d�Δԍ��𒊏o���A
// �X�̔M�d�΂̑g�ݍ��킹������
void CBoGroup::SetCombiPtn(int n1,int n2,int n3)
{
	int n,m,k;
	int	TcNo[9];

	// �O�̔M�d�Η�ԍ�����X�̔M�d�Δԍ��𒊏o
	TcNo[0] = line2tc[n1][0];
	TcNo[1] = line2tc[n1][1];
	TcNo[2] = line2tc[n1][2];
	TcNo[3] = line2tc[n2][0];
	TcNo[4] = line2tc[n2][1];
	TcNo[5] = line2tc[n2][2];
	TcNo[6] = line2tc[n3][0];
	TcNo[7] = line2tc[n3][1];
	TcNo[8] = line2tc[n3][2];

	// �X�̔M�d�Δԍ�����Q�̔M�d�Δԍ��̑g�ݍ��킹���o
	for(n = 0, k = 0;n < 8;n++)
	{
		for(m = n+1;m < 9;m++)
		{
			m_nChkCombiPtn[k][0] = TcNo[n];
			m_nChkCombiPtn[k][1] = TcNo[m];
			k++;
		}
	}
}

// �Q�̔M�d�Δԍ��̑g�ݍ��킹�����݂��邩�`�F�b�N
BOOL CBoGroup::CheckCombiPtn(int chk1, int chk2)
{
	int n;

	// �S�Ă̑g�ݍ��킹�ɑ΂���
	for(n = 0;n < 36;n++)
	{
		// �Q�̔M�d�Δԍ��̑g�ݍ��킹�����݂��邩�`�F�b�N
		if( ( m_nChkCombiPtn[n][0] == chk1 && m_nChkCombiPtn[n][1] == chk2 ) || 
			( m_nChkCombiPtn[n][0] == chk2 && m_nChkCombiPtn[n][1] == chk1 ) )
		{
			// ��v����g�ݍ��킹�L��
			return TRUE;
		}
	}
	// ��v����g�ݍ��킹����
	return FALSE;
}

//BOOL CBoGroup::IsGroup(int upper, int lower)
//{
//	if( m_iTCLR[(int)(upper / 3)][0] != -1 && 
//		m_iTCLR[(int)(upper / 3)][1] != -1 &&
//		m_iTCLR[(int)(lower / 3)][0] != -1 && 
//		m_iTCLR[(int)(lower / 3)][1] != -1 )
//		return TRUE;
//	else
//		return FALSE;
//}
