#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"BoGroup.h"

//int testtest = 0;

// tcno->列No変換テーブル
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

// 列No->tcno変換テーブル
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

	// 初期値を設定
	memset( iTCOkNg, 0, sizeof(iTCOkNg) );
	for ( i = 0; i < TCEditNum; i++ )
		iTCOkNg[i] = TRUE;

	// TC列に熱電対コンビが割り振られているかチェック
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

	// 各TC列の左右の列を検索
	memset( m_iTCLR, -1, sizeof( m_iTCLR ));
	for( k = 0; k < TCEditNum; k++ )
	{
		//　右側検索
		for( i = 0; i < TCEditNum; i++ )
		{
			idx = i + k;
			if( idx >= TCEditNum ) idx = 0;
			if( idx != k && iTCOkNg[idx] == 1 ){
				m_iTCLR[k][idR] = idx;break;
			}
		}
		//　左側検索
		for( i = 0; i < TCEditNum; i++ )
		{
			idx = -i + k;
			if( idx < 0 ) idx = idx + TCEditNum;
			if( idx != k && iTCOkNg[idx] == 1 ){
				m_iTCLR[k][idL] = idx;break;
			}
		}
	}

	// グループ情報初期化
	memset(dumy_Gr, 0, sizeof(dumy_Gr));

	// 全列に対して
	for( i = 0, dumy_max = 0; i < TCEditNum; i++ )
	{
		// 監視対象なら
		if( m_iTCLR[i][idR] != -1 && m_iTCLR[i][idL] != -1 )
		{
			dumy_Gr[dumy_max].cnt = 0;
			dumy_Gr[dumy_max].column = i;
			// 左右の列から３６通りの組み合わせを作成
			SetCombiPtn( i, m_iTCLR[i][idR], m_iTCLR[i][idL]);
			// 全コンビネーションに対して
			for( j = 0, cp = cnf.tcCombination; j < cnf.numCombination; j++, cp++ )
			{
				// グループ内に存在するなら
				if( TRUE == CheckCombiPtn( cp->upper, cp->lower) ) 
				{
					// 添え字を記憶する
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

// 三つの熱電対列番号から９つの熱電対番号を抽出し、
// ９つの熱電対の組み合わせを決定
void CBoGroup::SetCombiPtn(int n1,int n2,int n3)
{
	int n,m,k;
	int	TcNo[9];

	// 三つの熱電対列番号から９つの熱電対番号を抽出
	TcNo[0] = line2tc[n1][0];
	TcNo[1] = line2tc[n1][1];
	TcNo[2] = line2tc[n1][2];
	TcNo[3] = line2tc[n2][0];
	TcNo[4] = line2tc[n2][1];
	TcNo[5] = line2tc[n2][2];
	TcNo[6] = line2tc[n3][0];
	TcNo[7] = line2tc[n3][1];
	TcNo[8] = line2tc[n3][2];

	// ９つの熱電対番号から２つの熱電対番号の組み合わせ抽出
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

// ２つの熱電対番号の組み合わせが存在するかチェック
BOOL CBoGroup::CheckCombiPtn(int chk1, int chk2)
{
	int n;

	// 全ての組み合わせに対して
	for(n = 0;n < 36;n++)
	{
		// ２つの熱電対番号の組み合わせが存在するかチェック
		if( ( m_nChkCombiPtn[n][0] == chk1 && m_nChkCombiPtn[n][1] == chk2 ) || 
			( m_nChkCombiPtn[n][0] == chk2 && m_nChkCombiPtn[n][1] == chk1 ) )
		{
			// 一致する組み合わせ有り
			return TRUE;
		}
	}
	// 一致する組み合わせ無し
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
