#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"../include/neuralne.h"

///////////////////////////////////////////////////////////////////////////////
// 
double	eatha;								/* The convergence coefficient of back propagation. BP時の収束係数 */
double	alpha;								/* The convergence speed coefficient of back propagation. BP時の収束速度係数 */


/*====================================================*/
/* Neural Netを演算する。
// It calculates a neural net.
*/
void	doNeuralNet(
NeuralNet	*net,
double		*input,
double		*output )
{
	WeightMatrix	**wm;
	register int	m, nl;
	double	*in, *out;
	double	outVec1[NumLayerElement];
	double	outVec2[NumLayerElement];

	/* ﾊﾞｯﾌｧﾎﾟｲﾝﾀを初期化する。*/
	/* it initializes the buffer pointer.*/
	in	= input;
	out	= outVec1;

	/* 各層について、*/
	/* for each layer,*/
	nl	= net->numLayer - 1;
	wm	= net->wMatrix;
	for (m = 0; m < nl; m++, wm++) 
	{
		/* 最終段ならば、出力の格納位置を設定する。*/
		/* If it is the last paragraph, It sets the position of output. */
		if (m == nl - 1)			out	= output;

		/* 当該層の演算を行なう。*/
		/* It calculates a concerned layer. */
		doLayerCalculation(*wm, in, out);

		/* 入出力の格納場所を更新する。*/
		/* It renews the place of the I/O. */
		if (out == outVec1)			in	= outVec1,	out	= outVec2;
		else						in	= outVec2,	out	= outVec1;
	}

	/* 全て終了すれば、outputに演算結果が格納されたことになる。*/
	/* If ending completely, a calculation result is set to output. */
}

/*====================================================*/
/* 1層分のNeural Net演算を行なう。
// It calculates in Neural Net for 1 layer.
*/
void	doLayerCalculation(
register WeightMatrix	*wm,
register double	*input,
register double	*output )
{
	register int	o, i;
	int	no, ni;
	double	*w, *in;
	double	val;

	/* 各出力要素について、*/
	/* for each output element, */
	no	= wm->numOut;
	ni	= wm->numIn;
	w	= wm->weight;
	for (o = 0, no = wm->numOut; o < no; o++) 
	{
		/* 入力ﾍﾞｸﾀと重み係数ﾍﾞｸﾀの内積を求める。*/
		/* It calculates the inner product of input vector and weight coefficient vector. */
		val	= 0;
		for (i = 0, in = input; i < ni; i++)
			val	+= (*w++) * (*in++);
		val	+= (*w++);
		/* 内積値のｼｸﾞﾓｲﾄﾞ関数値を求める。*/
		/* It calculates sigmoid function value with inner product value. */
		*output++	= sigmoidFunction(val);
	}
}

/*====================================================*/
/* ｼｸﾞﾓｲﾄﾞ関数値を求める。
// It calculates sigmoid function
*/
double	sigmoidFunction(
double	val )
{
	double	ex;

	ex	= (val > 0.0) ?		exp(-val) : exp(val);

	/* ｼｸﾞﾓｲﾄﾞ値を演算して返す。*/
	/* return sigmoid value. */
	return	(val > 0.0) ?	1.0 / (1.0 + ex):
							ex / (1.0 + ex);
}

/*====================================================*/
/* Neural Netのﾊﾞｯｸﾌﾟﾛﾊﾟｹﾞｰｼｮﾝを実行する。

	ﾊﾞｯｸﾌﾟﾛﾊﾟｹﾞｰｼｮﾝのｱﾙｺﾞﾘｽﾞﾑは、以下の通りである。

	0.	第m層の演算値をyi(m)とする。
		yk(0)をNNの入力値、yi(M)をNNの出力値とする。
		この評価関数 Eを、次式にて与えるものとする。
		  E = 1/2Σ{yi(M) - di}^2										(1)
		diは出力の学習値である。

	1.	最終段層 Mについて、
		1.1	∂E/∂xi(M)を次式にて求める。
		  ∂E/∂xi(M)  = {yi(M) - di}･yi(M)･{1 - yi(M)}					(2)
		1.2	∂E/∂Wij(M)を次式にて求める。
		  ∂E/∂Wij(M) = ∂E/∂xi(M)･yj(M-1)							(3)

	2.	前段層 m(m = 1,...,M-1)について、
		2.1	∂E/∂xj(m)を次式にて求める。
		  ∂E/∂xj(m)  = [Σ{Wij･∂E/∂xi(m+1)}]･yj(m)･{1 - yj(m)}		(4)
		2.2	∂E/∂Wjk(m)を次式にて求める。
		  ∂E/∂Wjk(m)  = ∂E/∂xj(m)･yk(m-1)							(5)

	3.	前項 1及び 2にて、∂E/∂Wij(m)が全て求められることになる。

	4.	N回目演算時の Wij(m)[N]の増分 δWij(m)[N]を次式にて求める。
		  δWij(m)[N]  = - η･∂E/∂Wij(m) + α･δWij(m)[N-1]			(6)

	5.	N回目演算時の Wij(m)[N]を次式にて求める。
		  Wij(m)[N] = Wij(m)[N-1] + δWij(m)[N]							(7)

	6.	前項 1～5を Wij(m)が収束するまで繰り返す。

	※	δWij(m)[N]  = - η･{∂E/∂xi(m)･yj(m-1)} + α･δWij(m)[N-1]	(8)
		にて処理すると、∂E/∂Wij(m)を記憶する必要がなくなり、
		作業領域･計算時間が省かれる。

*/
/*====================================================*/
/* It executes the back propagation of Neural Net.

	The algorithm of back propagation is as the following.

	0.	It makes the calculation value of the m layer yi(m).
		It makes yk(0) the input value and it makes yi(M) the output value.
		This evaluation function E is given by the following formula.
		  E = 1/2Σ{yi(M) - di}^2										(1)
		di is the learning value of the output.

	1.	About M of the last paragraph layer,
		1.1	 It calculates ∂E/∂xi(M) by the following formula.
		  ∂E/∂xi(M)  = {yi(M) - di}･yi(M)･{1 - yi(M)}					(2)
		1.2	 It calculates ∂E/∂Wij(M) by the following formula.
		  ∂E/∂Wij(M) = ∂E/∂xi(M)･yj(M-1)							(3)

	2.	About m(m = 1,...,M-1) of the front  paragraph layer,
		2.1	 It calculates ∂E/∂xj(m) by the following formula.
		  ∂E/∂xj(m)  = [Σ{Wij･∂E/∂xi(m+1)}]･yj(m)･{1 - yj(m)}		(4)
		2.2	 It calculates ∂E/∂Wjk(m) by the following formula.
		  ∂E/∂Wjk(m)  = ∂E/∂xj(m)･yk(m-1)							(5)

	3.	All ∂E/∂Wij(m) are calculated by the preceding clause 1 and 2.

	4.	It calculates δWij(m)[N](The increment of Wij(m)[N] in N time eye calculation) 
		by the following formula.
		  δWij(m)[N]  = - η･∂E/∂Wij(m) + α･δWij(m)[N-1]			(6)

	5.	It calculates Wij(m)[N] in N time eye calculation by the following formula.
		  Wij(m)[N] = Wij(m)[N-1] + δWij(m)[N]							(7)

	6.	It repeats until Wij(m) converges by the preceding clause 1 to 5.

	※	δWij(m)[N]  = - η･{∂E/∂xi(m)･yj(m-1)} + α･δWij(m)[N-1]	(8)
		When processing in (8), To memorize ∂E/∂Wij(m) becomes not necessary.
		Therefore a work area and computation time are saved.

*/
void	doBPNeuralNet(
BPStructure	*bpStruc,
double	*input,
double	*target )
{
	register NeuralNet		*net;
	register WeightMatrix	*wM;
	register NeuralNet		*deltaNet;
	register WeightMatrix	*deltaWM;
	register double			*dEdx, *dEdxM, *yOut, *yIn, *d, *w;
	register int			m, M;
	register int			i, j, k, nOut, nIn;
	double	sum;

	/* ﾊﾞｯｸﾌﾟﾛﾊﾟｹﾞｰｼｮﾝ作業領域が未設定ならば、*/
	/* if It isn't set the back propagation work area, */
	if (!bpStruc->deltaNet) 
	{
		/* BPの為の作業領域を設定する。*/
		/* It sets the back propagation work area. */
		setupBPWorkArea(bpStruc);
	}
	net			= bpStruc->net;
	deltaNet	= bpStruc->deltaNet;

	/* 演算過程の履歴を残しながら、Neural Netの演算を行なう。*/
	/* While leaving the record of the process of the calculation, it calculates Neural Net. */
	doNNWithHistory(net, input, bpStruc->outVec);

	/* 最終段層について、∂E/∂xi(M)を次式にて求める。
		  ∂E/∂xi(M)  = {yi(M) - di}･yi(M)･{1 - yi(M)}
	*/
	/* About the last paragraph layer、It calculates ∂E/∂xi(M) by the following formula.
		  ∂E/∂xi(M)  = {yi(M) - di}･yi(M)･{1 - yi(M)}
	*/
	m		= net->numLayer - 1;
	deltaWM	= deltaNet->wMatrix[m-1];
	nOut	= deltaWM->numOut;
	nIn		= deltaWM->numIn;
	dEdx	= bpStruc->dEdx[m]->val;
	yOut	= bpStruc->outVec[m]->val;
	d		= target;
	for (i = 0; i < nOut; i++, yOut++)
		*dEdx++	= (*yOut - *d++) * (*yOut) * (1 - *yOut);

	/* 最終段層について、δWij(M)[N]を次式にて求める。
		  δWij(m)[N]  = - η･{∂E/∂xi(m)･yj(m-1)} + α･δWij(m)[N-1]
	*/
	/* About the last paragraph layer、It calculates δWij(m)[N] by the following formula.
		  δWij(m)[N]  = - η･{∂E/∂xi(m)･yj(m-1)} + α･δWij(m)[N-1]
	*/
	dEdx	= bpStruc->dEdx[m]->val;
	w		= deltaWM->weight;
	for (i = 0; i < nOut; i++, dEdx++) {
		yIn		= bpStruc->outVec[m-1]->val;
		for (j = 0; j < nIn; j++, w++) {
			*w	= -eatha * (*dEdx) * (*yIn++) + alpha * (*w);
		}
		*w	= -eatha * (*dEdx) + alpha * (*w);
		w++;
	}

	/* 前段層 m(m = 1,...,M-1)について、*/
	/* About m(m = 1,...,M-1) of the front  paragraph layer. */
	m--;
	for ( ; m > 0; m--) 
	{
		/* ∂E/∂xj(m)を次式にて求める。
			  ∂E/∂xj(m)  = [Σ{Wij･∂E/∂xi(m+1)}]･yj(m)･{1 - yj(m)}
		*/
		/* It calculates ∂E/∂xj(m) by the following formula.
			  ∂E/∂xj(m)  = [Σ{Wij･∂E/∂xi(m+1)}]･yj(m)･{1 - yj(m)}
		*/
		wM		= net->wMatrix[m];
		nOut	= wM->numOut;
		nIn		= wM->numIn;
		dEdx	= bpStruc->dEdx[m]->val;
		yOut	= bpStruc->outVec[m]->val;

		for (j = 0; j < nIn; j++, yOut++) {
			w		= wM->weight + j;
			dEdxM	= bpStruc->dEdx[m+1]->val;
			sum		= 0;
			for (i = 0; i < nOut; i++) {
				sum	+= (*w) * (*dEdxM++);
				w	+= nIn + 1;
			}
			*dEdx++	= sum * (*yOut) * (1 - *yOut);
		}

		/* δWjk(M)[N]を次式にて求める。
			  δWjk(m)[N]  = - η･{∂E/∂xj(m)･yk(m-1)} + α･δWjk(m)[N-1]
		*/
		/* It calculates δWjk(M)[N] by the following formula.
			  δWjk(m)[N]  = - η･{∂E/∂xj(m)･yk(m-1)} + α･δWjk(m)[N-1]
		*/
		deltaWM	= deltaNet->wMatrix[m-1];
		nOut	= deltaWM->numOut;
		nIn		= deltaWM->numIn;
		dEdx	= bpStruc->dEdx[m]->val;
		w		= deltaWM->weight;
		for (j = 0; j < nOut; j++, dEdx++) {
			yIn		= bpStruc->outVec[m-1]->val;
			for (k = 0; k < nIn; k++, w++) {
				*w	= -eatha * (*dEdx) * (*yIn++) + alpha * (*w);
			}
			*w	= -eatha * (*dEdx) + alpha * (*w);
			w++;
		}
	}

	/* 重み係数行列を更新する。*/
	/* It renews a weight coefficient procession. */
	M		= net->numLayer - 1;
	for (m = 0; m < M; m++) 
	{
		wM		= net->wMatrix[m];
		w		= wM->weight;
		deltaWM	= deltaNet->wMatrix[m];
		d		= deltaWM->weight;
		for (i = 0; i < wM->numOut; i++) 
		{
			for (j = 0; j < wM->numIn + 1; j++) 
			{
				*w++	+= *d++;
			}
		}
	}
}

/*====================================================*/
/* 演算過程の履歴を残しながら、Neural Netの演算を行なう。
// While leaving the record of the process of the calculation, it calculates Neural Net.
*/
void	doNNWithHistory(
NeuralNet	*net,
double	*input,
Vector	**outVec )
{
	register WeightMatrix	**wm;
	register int	l, nl;
	register int	i, ni;
	register double	*dp;

	/* 層数を得る。*/
	/* It gets the number of the layers. */
	nl	= net->numLayer - 1;
	/* 重み係数行列を設定する。*/
	/* It sets a weight coefficient procession. */
	wm	= net->wMatrix;

	/* 入力ﾍﾞｸﾀを初段の演算値に格納する。*/
	/* It sets an input vector to the calculation value of the first paragraph. */
	ni	= (*wm)->numIn;
	for (i = 0, dp = outVec[0]->val; i < ni; i++)
		*dp++	= *input++;

	/* 各層について、*/
	/* for each layer, */
	for (l = 0; l < nl; l++, wm++, outVec++) 
	{
		/* 当該層の演算を行なう。*/
		/* It calculates a concerned layer. */
		doLayerCalculation(*wm, outVec[0]->val, outVec[1]->val);
	}
}

/*====================================================*/
/* BPの為の作業領域を設定する。
// It sets the back propagation work area. 
*/
void	setupBPWorkArea(
BPStructure	*bpStruc )
{
	register NeuralNet		*net;
	register Vector			**vP;
	register WeightMatrix	**wm;
	register int	m, nl;

	/* Neural Netを得る。*/
	/* It gets a neural net. */
	net	= bpStruc->net;

	/* Neural Netの構造を複写する。*/
	/* It copies the structure of Neural Net. */
	bpStruc->deltaNet	= copyNeuralNet(net);

	/* Neural Net各層の演算値格納場所を確保する。*/
	/* It secures a calculation value setting place in each Neural Net layer. */
	nl	= net->numLayer - 1;
	for (m = 0, wm = net->wMatrix, vP = bpStruc->outVec; m < nl; m++, wm++) 
	{
		*vP++	= newVector((*wm)->numIn);
		if (m == nl - 1)
			*vP++	= newVector((*wm)->numOut);
	}
	/* Neural Net各層の内積値変分格納場所を確保する。*/
	/* It secures an inner product value setting place in each Neural Net layer. */
	for (m = 0, wm = net->wMatrix, vP = bpStruc->dEdx; m < nl; m++, wm++) 
	{
		*vP++	= newVector((*wm)->numIn);
		if (m == nl - 1)
			*vP++	= newVector((*wm)->numOut);
	}
}

/*====================================================*/
/* Neural Net重み係数の初期値として、-1.0～1.0の乱数を設定する。
// It sets the initialize value of the Neural Net weight coefficient 
   ( the random number of 1.0-1.0 ).
*/
void	setRandomWeight(
NeuralNet	*net )
{
	register int	m, M;
	register int	i, n;
	register double *w;

	/* 各層について、*/
	/* for each layer, */
	for (M = net->numLayer - 1, m = 0; m < M; m++) {
		/* 重み係数行列の要素数を設定する。*/
		/* It sets the weight coefficient procession number of the elements. */
		n	= net->wMatrix[m]->numOut * (net->wMatrix[m]->numIn + 1);
		/* 行列の各要素について、*/
		/* for each element of the procession, */
		for (i = 0, w = net->wMatrix[m]->weight; i < n; i++) {
			/* 乱数を設定する。*/
			/* It sets a random number. */
			*w++	= (double)rand() / ((double)0x4000) - 1.0;
		}
	}
}

/*====================================================*/
/* Neural Net構造体をﾌｧｲﾙに格納する。
// It saves Neural Net structure in the file.
*/
int saveNeuralNet(
char	*fname,
char	*name,
NeuralNet	*net )
{
	register FILE	*fp;
	register int	m, out, in;
	register WeightMatrix	*p;

	/* 指定されたﾌｧｲﾙをopenする。*/
	/* It opens a specified file. */
	if (!net || (fp = fopen(fname, "w")) == NULL) {
		/* 失敗すれば、ERROR。*/
		/* If failing, it is ERROR. */
		return	ERROR;
	}

	/* Neural Netの要素数を出力する。*/
	/* It outputs the Neural Net number of the elements. */
	fprintf(fp, "#  Neural Net Structure %s:\n", name);
	fprintf(fp, "# The number of element in each layer is ...\n");

	/* 各層について、*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* 当該層の要素数を出力する。*/
		/* It outputs the concerned layer number of the elements. */
		fprintf(fp, "  %d,", net->wMatrix[m]->numIn);
	}
	/* 最終段層の出力数を出力する。*/
	/* It outputs the last paragraph layer of the output number. */
	fprintf(fp, "  %d\n", net->wMatrix[net->numLayer - 2]->numOut);

	/* 各層について、*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* 係数行列を得る。*/
		/* It gets a coefficient procession. */
		p	= net->wMatrix[m];
		fprintf(fp,
			"# Weight coefficients of %dth layer element are ...\n", m + 1);
		/* 重み係数を出力する。*/
		/* It outputs a weight coefficient. */
		for (out = 0; out < p->numOut; out++) 
		{
			/* 係数行列を出力する。*/
			/* It outputs a coefficient procession. */
			for (in = 0; in < p->numIn + 1; in++)
				fprintf(fp, " % .8lE", welm(p, out, in));
			fprintf(fp, "\n");
		}
	}

	/* 学習回数の要素数を出力する。*/
	fprintf(fp, "#  The Learning Number of Times\n");
	fprintf(fp, "  %d\n", net->times);

	/* 出力ﾌｧｲﾙをcloseする。*/
	/* It closes an output file. */
	fclose(fp);

	/* 全て終了すれば、OK。*/
	return	OK;
}

/*====================================================*/
/* Neural Net構造体をﾌｧｲﾙから読み込む。
// It reads Neural Net structure from the file.
*/
NeuralNet	*loadNeuralNet(
char	*fname )
{
	register NeuralNet	*net;
	register FILE		*fp;
	register int		numLayer;
	register int		m;
	int		numLayerElement[NumNetLayer];

	/* 指定されたﾌｧｲﾙをopenする。*/
	/* It opens a specified file. */
	if ((fp = fopen(fname, "r")) == NULL) 
	{
		/* 失敗すれば、NULL。*/
		/* If failing, it is NULL. */
		return	NULL;
	}

	/* 要素数宣言を解析する。*/
	if ((numLayer = parseNumLayer(fp, numLayerElement)) == ERROR) 
	{
		/* 入力ﾌｧｲﾙをcloseする。*/
		fclose(fp);
		/* 不正があれば、NULL。*/
		return	NULL;
	}

	/* Neural Net構造体を確保する。*/
	/* It secures Neural Net structure. */
	net	= newNeuralNet(numLayer, numLayerElement);

	/* 各層について、*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* 重み係数宣言を解析する。*/
		/* It analyzes a weight coefficient declaration. */
		if (parseWeightMatrix(fp, net->wMatrix[m]) == ERROR) 
		{
			/* 不正があれば確保した領域を解放して、NULL。*/
			freeNeuralNet(net);
			/* 入力ﾌｧｲﾙをcloseする。*/
			fclose(fp);
			return	NULL;
		}
	}

	/* 学習回数を解析する。*/
	net->times = parseLarningTimes(fp);

	/* 入力ﾌｧｲﾙをcloseする。*/
	fclose(fp);

	/* 全て終了すれば、読み込んだ構造体を返す。*/
	return	net;
}

/*====================================================*/
/* 要素数宣言を解析する。
// It analyzes a element number declaration.
*/
int parseNumLayer(
FILE	*fp,
int		*numElement )
{
	char	line[1024];
	char	buf[256];
	register int	numLayer;
	register char	*p, *q;
	int	value;

	/* 最初の宣言を読み込む。*/
	/* It reads the first declaration. */
	for ( ; (p = fgets(line, 1024, fp)) != NULL && *line == '#'; )
		;
	/* 読めなければ、NULL。*/
	/* If it isn't possible to read, it is NULL. */
	if (!p)
		return	0;

	/* 行末まで、下記の要件を繰り返す。*/
	/* It repeats the following important matter to the end of a sentence. */
	for (numLayer = 0, p = line; ; ) {
		/* 空白を読み飛ばす。*/
		/* It skips a blank. */
		for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
			;
		/* 行末ならば、中断。*/
		/* If it does an end of a sentence, it stops. */
		if (!*p)
			break;

		/* ','か改行までの文字列を読み取る。*/
		/* It reads a character line until the comma or the starting a new line. */
		for (q = buf; *p && *p != ',' && *p != '\n'; p++)
			*q++	= *p;
		*q	= '\0';

		/* 数値を読み取る。*/
		/* It reads numerical value. */
		if (sscanf(buf, "%d", &value) != 1) {
			/* 数値でなければ、ERROR。*/
			return	ERROR;
		}
		/* 読み取った数値を格納する。*/
		/* It sets read numerical value. */
		numElement[numLayer++]	= value;

		/* 読み取り位置を進める。*/
		/* It carries forward a reading position. */
		if (*p)
			p++;
	}

	/* 層数が零ならば、ERROR。*/
	/* If the number of the layers is a zero, it is ERROR. */
	if (!numLayer)		return	ERROR;

	/* 全て終了すれば、層数を返す。*/
	return	numLayer;
}

/*====================================================*/
/* 重み係数宣言を解析する。
// It analyzes a weight coefficient declaration.
*/
int parseWeightMatrix(
FILE			*fp,
WeightMatrix	*matrix )
{
	char	line[8192];
	char	buf[256];
	register int	in, out;
	register char	*p, *q;
	double	value;

	/* 下記の要件を繰り返す。*/
	/* It repeats the following important matter. */
	for (out = 0; out < matrix->numOut; out++) 
	{
		/* 行を読み込む。*/
		/* It reads a line. */
		for ( ; (p = fgets(line, 8192, fp)) != NULL && *line == '#'; )
			;
		/* 全て読み込んでいないのに EOFを検知すると、ERROR。*/
		/* When detecting EOF in spite of not being reading completely, it is ERROR. */
		if (!p)
			return	ERROR;
		/* 入力要素数だけ下記の要件を繰り返す。*/
		/* Only the number of the input elements repeats the following important matter. */
		for (p = line, in = 0; in < matrix->numIn + 1; in++) 
		{
			/* 読み込んだ行から、空白を読み飛ばす。*/
			/* It skips a blank from the read line. */
			for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
				;
			/* 数値を読み込む。*/
			/* It reads numerical value. */
			for (q = buf; *p && *p != ' ' && *p != '\t' && *p != '\n'; p++)
				*q++	= *p;
			*q	= '\0';
			if (sscanf(buf, "%lf", &value) != 1) 
			{
				/* 読み込めなければ、ERROR。*/
				return	ERROR;
			}
			/* 読み込んだ数値を係数行列に設定する。*/
			/* It sets read numerical value to the coefficient procession. */
			welm(matrix, out, in)	= value;
		}
		/* 数値が残っていないか検査する。*/
		/* It checks the existence of numerical value. */
		for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
			;
		if (*p) {
			/* 残っていれば、ERROR。*/
			return	ERROR;
		}
	}
	/* 全て終了すれば、OK。*/
	return	OK;
}

/*====================================================*/
/* Neural Netの構造を複写する。
// It copies the structure of Neural Net.
*/
NeuralNet	*copyNeuralNet(
NeuralNet	*net )
{
	register int	m, nl, n;
	int	numElement[NumNetLayer];

	/* 入力された各層の要素数を設定する。*/
	/* It sets the element number inputted of each layer. */
	nl	= net->numLayer - 1;
	for (m = 0, n = 0; m < nl; m++) {
		numElement[n++]	= net->wMatrix[m]->numIn;
		if (m == nl - 1)
			numElement[n++]	= net->wMatrix[m]->numOut;
	}

	/* 新しい Neural Net構造体を確保する。*/
	/* It secures a new Neural Net structure. */
	return	newNeuralNet(n, numElement);
}

/*====================================================*/
/* Neural Net構造体を確保する。
// It secures Neural Net structure.
*/
NeuralNet	*newNeuralNet(
int	numLayer,
int	*numElement )
{
	register int	m, M, nIn, nOut;
	register NeuralNet		*p;

	/* Neural Netの構造体を確保する。*/
	/* It secures the structure of Neural Net. */
	p	= (NeuralNet *)calloc(1, neuralNetSize(numLayer - 1));
	if (!p) {
		fprintf(stderr, "newNeuralNet: not enough memory sorry.\n");
		exit(1);
	}

	/* Neural Netの層数を設定する。*/
	/* It sets the neural net number of the layers. */
	p->numLayer	= numLayer;

	/* 各層について、*/
	/* for each layer, */
	for (M = numLayer - 1, m = 0; m < M; m++) {
		/* 当該層の入力数を得る。*/
		/* It gets the concerned layer number of the input. */
		nIn		= numElement[m];
		/* 当該層の出力数を得る。*/
		/* It gets the concerned layer number of the output. */
		nOut	= numElement[m + 1];

		/* 重み行列を確保して設定する。*/
		/* it sets a weight procession and It secures. */
		p->wMatrix[m]	= newWeightMatrix(nOut, nIn);
	}

	/* 学習回数を初期化する。*/
	p->times = 0;

	/* 全て終了すれば、確保したNeural Net構造体のﾎﾟｲﾝﾀを返す。*/
	return	p;
}

/*====================================================*/
/* 新しい係数行列を確保する。
// It secures a new coefficient procession.
*/
WeightMatrix	*newWeightMatrix(
int	nOut,
int	nIn )
{
	register WeightMatrix	*p;
	size_t size;

	/* 係数行列領域を確保する。*/
	/* It secures a coefficient procession area. */
	size = weightMatrixSize(nOut, nIn);
	p	= (WeightMatrix *)calloc(1, size);
	if (!p) {
		fprintf(stderr, "newWeightMatrix: not enough memory sorry.\n");
		exit(1);
	}

	/* 行列の大きさを設定する。*/
	/* It sets the size of the procession. */
	p->numOut	= nOut;
	p->numIn	= nIn;

	/* 全て終了すれば、確保した係数行列のﾎﾟｲﾝﾀを返す。*/
	return	p;
}

/*====================================================*/
/* 新しい零ﾍﾞｸﾀを確保する。
// It secures a new zero vector.
*/
Vector	*newVector(
int	n )
{
	Vector	*v;

	/* 格納領域を確保する。*/
	/* It secures a setting area. */
	v	= (Vector *)calloc(1, vectorSize(n));

	/* ﾍﾞｸﾀのｻｲｽﾞを設定する。*/
	/* It sets the size of the vector. */
	v->num	= n;

	/* 領域を返す。*/
	/* It returns an area. */
	return	v;
}

/*====================================================*/
/* Neural Net構造体を解放する。
// It releases Neural Net structure.
*/
void	freeNeuralNet(
NeuralNet	*net )
{
	register int	m;

	/* 各層について、*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* 係数行列を解放する。*/
		/* It releases a coefficient procession. */
		free(net->wMatrix[m]);
	}

	/* Neural Net構造体を解放する。*/
	/* It releases Neural Net structure. */
	free(net);
}

/*====================================================*/
/* バックプロパゲーションの過程を表示する。
// It displays the process of back propagation.
*/
void	printBP(
BPStructure	*bpStruc )
{
	register int	n;

	/* Neural Net構造を表示する。*/
	/* It displays Neural Net structure. */
	printNN(bpStruc->net);

	/* 出力値ﾍﾞｸﾀを表示する。*/
	/* It displays an output value vector. */
	for (n = 0; bpStruc->outVec[n]; n++) {
		logPrintf( _T("# %dth layer outputs ...\n"), n);
		printVec(bpStruc->outVec[n]);
	}

	/* 内積値変分を表示する。*/
	/* It displays an inner product value. */
	for (n = 0; bpStruc->dEdx[n]; n++) {
		logPrintf( _T("# %dth layer dE/dx ...\n"), n);
		printVec(bpStruc->dEdx[n]);
	}

	/* Neural Netの更新量を表示する。*/
	/* It displays the renewal quantity of Neural Net. */
	logPrintf( _T("Up to date Volume of Neural Net ...\n"));
	printNN(bpStruc->deltaNet);
}

/*====================================================*/
/* Neural Net構造を表示する。
// It displays Neural Net structure.
*/
int printNN(
NeuralNet	*net )
{
	register int	m, out, in;
	register WeightMatrix	*p;

	/* Neural Netの要素数を出力する。*/
	/* It outputs the Neural Net number of the elements. */
	logPrintf( _T("#  Neural Net Structure\n"));
	logPrintf( _T("# The number of element in each layer is ...\n"));

	/* 各層について、*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* 当該層の要素数を出力する。*/
		/* It outputs the concerned layer number of the elements. */
		logPrintf( _T("  %d,"), net->wMatrix[m]->numIn);
	}
	/* 最終段層の出力数を出力する。*/
	/* It outputs the last paragraph layer of the output number. */
	logPrintf( _T("  %d\n"), net->wMatrix[net->numLayer - 2]->numOut);

	/* 各層について、*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* 係数行列を得る。*/
		/* It gets a coefficient procession. */
		p	= net->wMatrix[m];
		logPrintf( _T("# Weight coefficients of %dth layer element are ...\n"), m + 1);
		/* 重み係数を出力する。*/
		/* It outputs a weight coefficient. */
		for (out = 0; out < p->numOut; out++) 
		{
			/* 係数行列を出力する。*/
			/* It outputs a coefficient procession. */
			for (in = 0; in < p->numIn + 1; in++)
				logPrintf( _T(" % .8lE"), welm(p, out, in));
			logPrintf( _T("\n"));
		}
	}
	/* 全て終了すれば、OK。*/
	return	OK;
}

/*====================================================*/
/* 演算値ﾍﾞｸﾀを表示する。
// It displays a calculation value vector.
*/
int printVec(
Vector	*vec )
{
	register int	i;

	/* 各要素について、*/
	/* for each layer, */
	for (i = 0; i < vec->num; i++) 
	{
		/* 要素を表示する。*/
		/* It displays a element. */
		logPrintf( _T(" % .8lE"), vec->val[i]);
	}
	logPrintf( _T("\n") );

	/* 全て終了すれば、OK。*/
	return	OK;
}

/*====================================================*/
/* 学習回数を解析する。
// It analyzes a larning number of times.
*/
int parseLarningTimes(
FILE	*fp)
{
	char	line[1024];
	char	buf[256];
	char	*p, *q;
	int		times;
	int		value;

	/* 最初の宣言を読み込む。*/
	/* It reads the first declaration. */
	for ( ; (p = fgets(line, 1024, fp)) != NULL && *line == '#'; )
		;
	/* 読めなければ、NULL。*/
	/* If it isn't possible to read, it is NULL. */
	if (!p)
		return	0;

	/* 行末まで、下記の要件を繰り返す。*/
	/* It repeats the following important matter to the end of a sentence. */
	for (times = 0, p = line; ; ) {
		/* 空白を読み飛ばす。*/
		/* It skips a blank. */
		for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
			;
		/* 行末ならば、中断。*/
		/* If it does an end of a sentence, it stops. */
		if (!*p)
			break;

		/* ','か改行までの文字列を読み取る。*/
		/* It reads a character line until the comma or the starting a new line. */
		for (q = buf; *p && *p != ',' && *p != '\n'; p++)
			*q++	= *p;
		*q	= '\0';

		/* 数値を読み取る。*/
		/* It reads numerical value. */
		if (sscanf(buf, "%d", &value) != 1) {
			/* 数値でなければ、0を返す*/
			return	0;
		}
		/* 読み取った数値を格納する。*/
		/* It sets read numerical value. */
		times	= value;

		// 読み取れたので終了
		break;
	}

	/* 全て終了すれば、層数を返す。*/
	return	times;
}

