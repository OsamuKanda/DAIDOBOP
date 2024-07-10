#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"../include/neuralne.h"

///////////////////////////////////////////////////////////////////////////////
// 
double	eatha;								/* The convergence coefficient of back propagation. BP���̎����W�� */
double	alpha;								/* The convergence speed coefficient of back propagation. BP���̎������x�W�� */


/*====================================================*/
/* Neural Net�����Z����B
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

	/* �ޯ̧�߲��������������B*/
	/* it initializes the buffer pointer.*/
	in	= input;
	out	= outVec1;

	/* �e�w�ɂ��āA*/
	/* for each layer,*/
	nl	= net->numLayer - 1;
	wm	= net->wMatrix;
	for (m = 0; m < nl; m++, wm++) 
	{
		/* �ŏI�i�Ȃ�΁A�o�͂̊i�[�ʒu��ݒ肷��B*/
		/* If it is the last paragraph, It sets the position of output. */
		if (m == nl - 1)			out	= output;

		/* ���Y�w�̉��Z���s�Ȃ��B*/
		/* It calculates a concerned layer. */
		doLayerCalculation(*wm, in, out);

		/* ���o�͂̊i�[�ꏊ���X�V����B*/
		/* It renews the place of the I/O. */
		if (out == outVec1)			in	= outVec1,	out	= outVec2;
		else						in	= outVec2,	out	= outVec1;
	}

	/* �S�ďI������΁Aoutput�ɉ��Z���ʂ��i�[���ꂽ���ƂɂȂ�B*/
	/* If ending completely, a calculation result is set to output. */
}

/*====================================================*/
/* 1�w����Neural Net���Z���s�Ȃ��B
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

	/* �e�o�͗v�f�ɂ��āA*/
	/* for each output element, */
	no	= wm->numOut;
	ni	= wm->numIn;
	w	= wm->weight;
	for (o = 0, no = wm->numOut; o < no; o++) 
	{
		/* �����޸��Əd�݌W���޸��̓��ς����߂�B*/
		/* It calculates the inner product of input vector and weight coefficient vector. */
		val	= 0;
		for (i = 0, in = input; i < ni; i++)
			val	+= (*w++) * (*in++);
		val	+= (*w++);
		/* ���ϒl�̼��Ӳ�ފ֐��l�����߂�B*/
		/* It calculates sigmoid function value with inner product value. */
		*output++	= sigmoidFunction(val);
	}
}

/*====================================================*/
/* ���Ӳ�ފ֐��l�����߂�B
// It calculates sigmoid function
*/
double	sigmoidFunction(
double	val )
{
	double	ex;

	ex	= (val > 0.0) ?		exp(-val) : exp(val);

	/* ���Ӳ�ޒl�����Z���ĕԂ��B*/
	/* return sigmoid value. */
	return	(val > 0.0) ?	1.0 / (1.0 + ex):
							ex / (1.0 + ex);
}

/*====================================================*/
/* Neural Net���ޯ�����߹ް��݂����s����B

	�ޯ�����߹ް��݂̱ٺ�ؽ�т́A�ȉ��̒ʂ�ł���B

	0.	��m�w�̉��Z�l��yi(m)�Ƃ���B
		yk(0)��NN�̓��͒l�Ayi(M)��NN�̏o�͒l�Ƃ���B
		���̕]���֐� E���A�����ɂė^������̂Ƃ���B
		  E = 1/2��{yi(M) - di}^2										(1)
		di�͏o�͂̊w�K�l�ł���B

	1.	�ŏI�i�w M�ɂ��āA
		1.1	��E/��xi(M)�������ɂċ��߂�B
		  ��E/��xi(M)  = {yi(M) - di}�yi(M)�{1 - yi(M)}					(2)
		1.2	��E/��Wij(M)�������ɂċ��߂�B
		  ��E/��Wij(M) = ��E/��xi(M)�yj(M-1)							(3)

	2.	�O�i�w m(m = 1,...,M-1)�ɂ��āA
		2.1	��E/��xj(m)�������ɂċ��߂�B
		  ��E/��xj(m)  = [��{Wij���E/��xi(m+1)}]�yj(m)�{1 - yj(m)}		(4)
		2.2	��E/��Wjk(m)�������ɂċ��߂�B
		  ��E/��Wjk(m)  = ��E/��xj(m)�yk(m-1)							(5)

	3.	�O�� 1�y�� 2�ɂāA��E/��Wij(m)���S�ċ��߂��邱�ƂɂȂ�B

	4.	N��ډ��Z���� Wij(m)[N]�̑��� ��Wij(m)[N]�������ɂċ��߂�B
		  ��Wij(m)[N]  = - �ť��E/��Wij(m) + �����Wij(m)[N-1]			(6)

	5.	N��ډ��Z���� Wij(m)[N]�������ɂċ��߂�B
		  Wij(m)[N] = Wij(m)[N-1] + ��Wij(m)[N]							(7)

	6.	�O�� 1�`5�� Wij(m)����������܂ŌJ��Ԃ��B

	��	��Wij(m)[N]  = - �ť{��E/��xi(m)�yj(m-1)} + �����Wij(m)[N-1]	(8)
		�ɂď�������ƁA��E/��Wij(m)���L������K�v���Ȃ��Ȃ�A
		��Ɨ̈楌v�Z���Ԃ��Ȃ����B

*/
/*====================================================*/
/* It executes the back propagation of Neural Net.

	The algorithm of back propagation is as the following.

	0.	It makes the calculation value of the m layer yi(m).
		It makes yk(0) the input value and it makes yi(M) the output value.
		This evaluation function E is given by the following formula.
		  E = 1/2��{yi(M) - di}^2										(1)
		di is the learning value of the output.

	1.	About M of the last paragraph layer,
		1.1	 It calculates ��E/��xi(M) by the following formula.
		  ��E/��xi(M)  = {yi(M) - di}�yi(M)�{1 - yi(M)}					(2)
		1.2	 It calculates ��E/��Wij(M) by the following formula.
		  ��E/��Wij(M) = ��E/��xi(M)�yj(M-1)							(3)

	2.	About m(m = 1,...,M-1) of the front  paragraph layer,
		2.1	 It calculates ��E/��xj(m) by the following formula.
		  ��E/��xj(m)  = [��{Wij���E/��xi(m+1)}]�yj(m)�{1 - yj(m)}		(4)
		2.2	 It calculates ��E/��Wjk(m) by the following formula.
		  ��E/��Wjk(m)  = ��E/��xj(m)�yk(m-1)							(5)

	3.	All ��E/��Wij(m) are calculated by the preceding clause 1 and 2.

	4.	It calculates ��Wij(m)[N](The increment of Wij(m)[N] in N time eye calculation) 
		by the following formula.
		  ��Wij(m)[N]  = - �ť��E/��Wij(m) + �����Wij(m)[N-1]			(6)

	5.	It calculates Wij(m)[N] in N time eye calculation by the following formula.
		  Wij(m)[N] = Wij(m)[N-1] + ��Wij(m)[N]							(7)

	6.	It repeats until Wij(m) converges by the preceding clause 1 to 5.

	��	��Wij(m)[N]  = - �ť{��E/��xi(m)�yj(m-1)} + �����Wij(m)[N-1]	(8)
		When processing in (8), To memorize ��E/��Wij(m) becomes not necessary.
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

	/* �ޯ�����߹ް��ݍ�Ɨ̈悪���ݒ�Ȃ�΁A*/
	/* if It isn't set the back propagation work area, */
	if (!bpStruc->deltaNet) 
	{
		/* BP�ׂ̈̍�Ɨ̈��ݒ肷��B*/
		/* It sets the back propagation work area. */
		setupBPWorkArea(bpStruc);
	}
	net			= bpStruc->net;
	deltaNet	= bpStruc->deltaNet;

	/* ���Z�ߒ��̗������c���Ȃ���ANeural Net�̉��Z���s�Ȃ��B*/
	/* While leaving the record of the process of the calculation, it calculates Neural Net. */
	doNNWithHistory(net, input, bpStruc->outVec);

	/* �ŏI�i�w�ɂ��āA��E/��xi(M)�������ɂċ��߂�B
		  ��E/��xi(M)  = {yi(M) - di}�yi(M)�{1 - yi(M)}
	*/
	/* About the last paragraph layer�AIt calculates ��E/��xi(M) by the following formula.
		  ��E/��xi(M)  = {yi(M) - di}�yi(M)�{1 - yi(M)}
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

	/* �ŏI�i�w�ɂ��āA��Wij(M)[N]�������ɂċ��߂�B
		  ��Wij(m)[N]  = - �ť{��E/��xi(m)�yj(m-1)} + �����Wij(m)[N-1]
	*/
	/* About the last paragraph layer�AIt calculates ��Wij(m)[N] by the following formula.
		  ��Wij(m)[N]  = - �ť{��E/��xi(m)�yj(m-1)} + �����Wij(m)[N-1]
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

	/* �O�i�w m(m = 1,...,M-1)�ɂ��āA*/
	/* About m(m = 1,...,M-1) of the front  paragraph layer. */
	m--;
	for ( ; m > 0; m--) 
	{
		/* ��E/��xj(m)�������ɂċ��߂�B
			  ��E/��xj(m)  = [��{Wij���E/��xi(m+1)}]�yj(m)�{1 - yj(m)}
		*/
		/* It calculates ��E/��xj(m) by the following formula.
			  ��E/��xj(m)  = [��{Wij���E/��xi(m+1)}]�yj(m)�{1 - yj(m)}
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

		/* ��Wjk(M)[N]�������ɂċ��߂�B
			  ��Wjk(m)[N]  = - �ť{��E/��xj(m)�yk(m-1)} + �����Wjk(m)[N-1]
		*/
		/* It calculates ��Wjk(M)[N] by the following formula.
			  ��Wjk(m)[N]  = - �ť{��E/��xj(m)�yk(m-1)} + �����Wjk(m)[N-1]
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

	/* �d�݌W���s����X�V����B*/
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
/* ���Z�ߒ��̗������c���Ȃ���ANeural Net�̉��Z���s�Ȃ��B
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

	/* �w���𓾂�B*/
	/* It gets the number of the layers. */
	nl	= net->numLayer - 1;
	/* �d�݌W���s���ݒ肷��B*/
	/* It sets a weight coefficient procession. */
	wm	= net->wMatrix;

	/* �����޸������i�̉��Z�l�Ɋi�[����B*/
	/* It sets an input vector to the calculation value of the first paragraph. */
	ni	= (*wm)->numIn;
	for (i = 0, dp = outVec[0]->val; i < ni; i++)
		*dp++	= *input++;

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (l = 0; l < nl; l++, wm++, outVec++) 
	{
		/* ���Y�w�̉��Z���s�Ȃ��B*/
		/* It calculates a concerned layer. */
		doLayerCalculation(*wm, outVec[0]->val, outVec[1]->val);
	}
}

/*====================================================*/
/* BP�ׂ̈̍�Ɨ̈��ݒ肷��B
// It sets the back propagation work area. 
*/
void	setupBPWorkArea(
BPStructure	*bpStruc )
{
	register NeuralNet		*net;
	register Vector			**vP;
	register WeightMatrix	**wm;
	register int	m, nl;

	/* Neural Net�𓾂�B*/
	/* It gets a neural net. */
	net	= bpStruc->net;

	/* Neural Net�̍\���𕡎ʂ���B*/
	/* It copies the structure of Neural Net. */
	bpStruc->deltaNet	= copyNeuralNet(net);

	/* Neural Net�e�w�̉��Z�l�i�[�ꏊ���m�ۂ���B*/
	/* It secures a calculation value setting place in each Neural Net layer. */
	nl	= net->numLayer - 1;
	for (m = 0, wm = net->wMatrix, vP = bpStruc->outVec; m < nl; m++, wm++) 
	{
		*vP++	= newVector((*wm)->numIn);
		if (m == nl - 1)
			*vP++	= newVector((*wm)->numOut);
	}
	/* Neural Net�e�w�̓��ϒl�ϕ��i�[�ꏊ���m�ۂ���B*/
	/* It secures an inner product value setting place in each Neural Net layer. */
	for (m = 0, wm = net->wMatrix, vP = bpStruc->dEdx; m < nl; m++, wm++) 
	{
		*vP++	= newVector((*wm)->numIn);
		if (m == nl - 1)
			*vP++	= newVector((*wm)->numOut);
	}
}

/*====================================================*/
/* Neural Net�d�݌W���̏����l�Ƃ��āA-1.0�`1.0�̗�����ݒ肷��B
// It sets the initialize value of the Neural Net weight coefficient 
   ( the random number of 1.0-1.0 ).
*/
void	setRandomWeight(
NeuralNet	*net )
{
	register int	m, M;
	register int	i, n;
	register double *w;

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (M = net->numLayer - 1, m = 0; m < M; m++) {
		/* �d�݌W���s��̗v�f����ݒ肷��B*/
		/* It sets the weight coefficient procession number of the elements. */
		n	= net->wMatrix[m]->numOut * (net->wMatrix[m]->numIn + 1);
		/* �s��̊e�v�f�ɂ��āA*/
		/* for each element of the procession, */
		for (i = 0, w = net->wMatrix[m]->weight; i < n; i++) {
			/* ������ݒ肷��B*/
			/* It sets a random number. */
			*w++	= (double)rand() / ((double)0x4000) - 1.0;
		}
	}
}

/*====================================================*/
/* Neural Net�\���̂�̧�قɊi�[����B
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

	/* �w�肳�ꂽ̧�ق�open����B*/
	/* It opens a specified file. */
	if (!net || (fp = fopen(fname, "w")) == NULL) {
		/* ���s����΁AERROR�B*/
		/* If failing, it is ERROR. */
		return	ERROR;
	}

	/* Neural Net�̗v�f�����o�͂���B*/
	/* It outputs the Neural Net number of the elements. */
	fprintf(fp, "#  Neural Net Structure %s:\n", name);
	fprintf(fp, "# The number of element in each layer is ...\n");

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* ���Y�w�̗v�f�����o�͂���B*/
		/* It outputs the concerned layer number of the elements. */
		fprintf(fp, "  %d,", net->wMatrix[m]->numIn);
	}
	/* �ŏI�i�w�̏o�͐����o�͂���B*/
	/* It outputs the last paragraph layer of the output number. */
	fprintf(fp, "  %d\n", net->wMatrix[net->numLayer - 2]->numOut);

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* �W���s��𓾂�B*/
		/* It gets a coefficient procession. */
		p	= net->wMatrix[m];
		fprintf(fp,
			"# Weight coefficients of %dth layer element are ...\n", m + 1);
		/* �d�݌W�����o�͂���B*/
		/* It outputs a weight coefficient. */
		for (out = 0; out < p->numOut; out++) 
		{
			/* �W���s����o�͂���B*/
			/* It outputs a coefficient procession. */
			for (in = 0; in < p->numIn + 1; in++)
				fprintf(fp, " % .8lE", welm(p, out, in));
			fprintf(fp, "\n");
		}
	}

	/* �w�K�񐔂̗v�f�����o�͂���B*/
	fprintf(fp, "#  The Learning Number of Times\n");
	fprintf(fp, "  %d\n", net->times);

	/* �o��̧�ق�close����B*/
	/* It closes an output file. */
	fclose(fp);

	/* �S�ďI������΁AOK�B*/
	return	OK;
}

/*====================================================*/
/* Neural Net�\���̂�̧�ق���ǂݍ��ށB
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

	/* �w�肳�ꂽ̧�ق�open����B*/
	/* It opens a specified file. */
	if ((fp = fopen(fname, "r")) == NULL) 
	{
		/* ���s����΁ANULL�B*/
		/* If failing, it is NULL. */
		return	NULL;
	}

	/* �v�f���錾����͂���B*/
	if ((numLayer = parseNumLayer(fp, numLayerElement)) == ERROR) 
	{
		/* ����̧�ق�close����B*/
		fclose(fp);
		/* �s��������΁ANULL�B*/
		return	NULL;
	}

	/* Neural Net�\���̂��m�ۂ���B*/
	/* It secures Neural Net structure. */
	net	= newNeuralNet(numLayer, numLayerElement);

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* �d�݌W���錾����͂���B*/
		/* It analyzes a weight coefficient declaration. */
		if (parseWeightMatrix(fp, net->wMatrix[m]) == ERROR) 
		{
			/* �s��������Ίm�ۂ����̈��������āANULL�B*/
			freeNeuralNet(net);
			/* ����̧�ق�close����B*/
			fclose(fp);
			return	NULL;
		}
	}

	/* �w�K�񐔂���͂���B*/
	net->times = parseLarningTimes(fp);

	/* ����̧�ق�close����B*/
	fclose(fp);

	/* �S�ďI������΁A�ǂݍ��񂾍\���̂�Ԃ��B*/
	return	net;
}

/*====================================================*/
/* �v�f���錾����͂���B
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

	/* �ŏ��̐錾��ǂݍ��ށB*/
	/* It reads the first declaration. */
	for ( ; (p = fgets(line, 1024, fp)) != NULL && *line == '#'; )
		;
	/* �ǂ߂Ȃ���΁ANULL�B*/
	/* If it isn't possible to read, it is NULL. */
	if (!p)
		return	0;

	/* �s���܂ŁA���L�̗v�����J��Ԃ��B*/
	/* It repeats the following important matter to the end of a sentence. */
	for (numLayer = 0, p = line; ; ) {
		/* �󔒂�ǂݔ�΂��B*/
		/* It skips a blank. */
		for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
			;
		/* �s���Ȃ�΁A���f�B*/
		/* If it does an end of a sentence, it stops. */
		if (!*p)
			break;

		/* ','�����s�܂ł̕������ǂݎ��B*/
		/* It reads a character line until the comma or the starting a new line. */
		for (q = buf; *p && *p != ',' && *p != '\n'; p++)
			*q++	= *p;
		*q	= '\0';

		/* ���l��ǂݎ��B*/
		/* It reads numerical value. */
		if (sscanf(buf, "%d", &value) != 1) {
			/* ���l�łȂ���΁AERROR�B*/
			return	ERROR;
		}
		/* �ǂݎ�������l���i�[����B*/
		/* It sets read numerical value. */
		numElement[numLayer++]	= value;

		/* �ǂݎ��ʒu��i�߂�B*/
		/* It carries forward a reading position. */
		if (*p)
			p++;
	}

	/* �w������Ȃ�΁AERROR�B*/
	/* If the number of the layers is a zero, it is ERROR. */
	if (!numLayer)		return	ERROR;

	/* �S�ďI������΁A�w����Ԃ��B*/
	return	numLayer;
}

/*====================================================*/
/* �d�݌W���錾����͂���B
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

	/* ���L�̗v�����J��Ԃ��B*/
	/* It repeats the following important matter. */
	for (out = 0; out < matrix->numOut; out++) 
	{
		/* �s��ǂݍ��ށB*/
		/* It reads a line. */
		for ( ; (p = fgets(line, 8192, fp)) != NULL && *line == '#'; )
			;
		/* �S�ēǂݍ���ł��Ȃ��̂� EOF�����m����ƁAERROR�B*/
		/* When detecting EOF in spite of not being reading completely, it is ERROR. */
		if (!p)
			return	ERROR;
		/* ���͗v�f���������L�̗v�����J��Ԃ��B*/
		/* Only the number of the input elements repeats the following important matter. */
		for (p = line, in = 0; in < matrix->numIn + 1; in++) 
		{
			/* �ǂݍ��񂾍s����A�󔒂�ǂݔ�΂��B*/
			/* It skips a blank from the read line. */
			for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
				;
			/* ���l��ǂݍ��ށB*/
			/* It reads numerical value. */
			for (q = buf; *p && *p != ' ' && *p != '\t' && *p != '\n'; p++)
				*q++	= *p;
			*q	= '\0';
			if (sscanf(buf, "%lf", &value) != 1) 
			{
				/* �ǂݍ��߂Ȃ���΁AERROR�B*/
				return	ERROR;
			}
			/* �ǂݍ��񂾐��l���W���s��ɐݒ肷��B*/
			/* It sets read numerical value to the coefficient procession. */
			welm(matrix, out, in)	= value;
		}
		/* ���l���c���Ă��Ȃ�����������B*/
		/* It checks the existence of numerical value. */
		for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
			;
		if (*p) {
			/* �c���Ă���΁AERROR�B*/
			return	ERROR;
		}
	}
	/* �S�ďI������΁AOK�B*/
	return	OK;
}

/*====================================================*/
/* Neural Net�̍\���𕡎ʂ���B
// It copies the structure of Neural Net.
*/
NeuralNet	*copyNeuralNet(
NeuralNet	*net )
{
	register int	m, nl, n;
	int	numElement[NumNetLayer];

	/* ���͂��ꂽ�e�w�̗v�f����ݒ肷��B*/
	/* It sets the element number inputted of each layer. */
	nl	= net->numLayer - 1;
	for (m = 0, n = 0; m < nl; m++) {
		numElement[n++]	= net->wMatrix[m]->numIn;
		if (m == nl - 1)
			numElement[n++]	= net->wMatrix[m]->numOut;
	}

	/* �V���� Neural Net�\���̂��m�ۂ���B*/
	/* It secures a new Neural Net structure. */
	return	newNeuralNet(n, numElement);
}

/*====================================================*/
/* Neural Net�\���̂��m�ۂ���B
// It secures Neural Net structure.
*/
NeuralNet	*newNeuralNet(
int	numLayer,
int	*numElement )
{
	register int	m, M, nIn, nOut;
	register NeuralNet		*p;

	/* Neural Net�̍\���̂��m�ۂ���B*/
	/* It secures the structure of Neural Net. */
	p	= (NeuralNet *)calloc(1, neuralNetSize(numLayer - 1));
	if (!p) {
		fprintf(stderr, "newNeuralNet: not enough memory sorry.\n");
		exit(1);
	}

	/* Neural Net�̑w����ݒ肷��B*/
	/* It sets the neural net number of the layers. */
	p->numLayer	= numLayer;

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (M = numLayer - 1, m = 0; m < M; m++) {
		/* ���Y�w�̓��͐��𓾂�B*/
		/* It gets the concerned layer number of the input. */
		nIn		= numElement[m];
		/* ���Y�w�̏o�͐��𓾂�B*/
		/* It gets the concerned layer number of the output. */
		nOut	= numElement[m + 1];

		/* �d�ݍs����m�ۂ��Đݒ肷��B*/
		/* it sets a weight procession and It secures. */
		p->wMatrix[m]	= newWeightMatrix(nOut, nIn);
	}

	/* �w�K�񐔂�����������B*/
	p->times = 0;

	/* �S�ďI������΁A�m�ۂ���Neural Net�\���̂��߲����Ԃ��B*/
	return	p;
}

/*====================================================*/
/* �V�����W���s����m�ۂ���B
// It secures a new coefficient procession.
*/
WeightMatrix	*newWeightMatrix(
int	nOut,
int	nIn )
{
	register WeightMatrix	*p;
	size_t size;

	/* �W���s��̈���m�ۂ���B*/
	/* It secures a coefficient procession area. */
	size = weightMatrixSize(nOut, nIn);
	p	= (WeightMatrix *)calloc(1, size);
	if (!p) {
		fprintf(stderr, "newWeightMatrix: not enough memory sorry.\n");
		exit(1);
	}

	/* �s��̑傫����ݒ肷��B*/
	/* It sets the size of the procession. */
	p->numOut	= nOut;
	p->numIn	= nIn;

	/* �S�ďI������΁A�m�ۂ����W���s����߲����Ԃ��B*/
	return	p;
}

/*====================================================*/
/* �V�������޸����m�ۂ���B
// It secures a new zero vector.
*/
Vector	*newVector(
int	n )
{
	Vector	*v;

	/* �i�[�̈���m�ۂ���B*/
	/* It secures a setting area. */
	v	= (Vector *)calloc(1, vectorSize(n));

	/* �޸��̻��ނ�ݒ肷��B*/
	/* It sets the size of the vector. */
	v->num	= n;

	/* �̈��Ԃ��B*/
	/* It returns an area. */
	return	v;
}

/*====================================================*/
/* Neural Net�\���̂��������B
// It releases Neural Net structure.
*/
void	freeNeuralNet(
NeuralNet	*net )
{
	register int	m;

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* �W���s����������B*/
		/* It releases a coefficient procession. */
		free(net->wMatrix[m]);
	}

	/* Neural Net�\���̂��������B*/
	/* It releases Neural Net structure. */
	free(net);
}

/*====================================================*/
/* �o�b�N�v���p�Q�[�V�����̉ߒ���\������B
// It displays the process of back propagation.
*/
void	printBP(
BPStructure	*bpStruc )
{
	register int	n;

	/* Neural Net�\����\������B*/
	/* It displays Neural Net structure. */
	printNN(bpStruc->net);

	/* �o�͒l�޸���\������B*/
	/* It displays an output value vector. */
	for (n = 0; bpStruc->outVec[n]; n++) {
		logPrintf( _T("# %dth layer outputs ...\n"), n);
		printVec(bpStruc->outVec[n]);
	}

	/* ���ϒl�ϕ���\������B*/
	/* It displays an inner product value. */
	for (n = 0; bpStruc->dEdx[n]; n++) {
		logPrintf( _T("# %dth layer dE/dx ...\n"), n);
		printVec(bpStruc->dEdx[n]);
	}

	/* Neural Net�̍X�V�ʂ�\������B*/
	/* It displays the renewal quantity of Neural Net. */
	logPrintf( _T("Up to date Volume of Neural Net ...\n"));
	printNN(bpStruc->deltaNet);
}

/*====================================================*/
/* Neural Net�\����\������B
// It displays Neural Net structure.
*/
int printNN(
NeuralNet	*net )
{
	register int	m, out, in;
	register WeightMatrix	*p;

	/* Neural Net�̗v�f�����o�͂���B*/
	/* It outputs the Neural Net number of the elements. */
	logPrintf( _T("#  Neural Net Structure\n"));
	logPrintf( _T("# The number of element in each layer is ...\n"));

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* ���Y�w�̗v�f�����o�͂���B*/
		/* It outputs the concerned layer number of the elements. */
		logPrintf( _T("  %d,"), net->wMatrix[m]->numIn);
	}
	/* �ŏI�i�w�̏o�͐����o�͂���B*/
	/* It outputs the last paragraph layer of the output number. */
	logPrintf( _T("  %d\n"), net->wMatrix[net->numLayer - 2]->numOut);

	/* �e�w�ɂ��āA*/
	/* for each layer, */
	for (m = 0; m < net->numLayer - 1; m++) 
	{
		/* �W���s��𓾂�B*/
		/* It gets a coefficient procession. */
		p	= net->wMatrix[m];
		logPrintf( _T("# Weight coefficients of %dth layer element are ...\n"), m + 1);
		/* �d�݌W�����o�͂���B*/
		/* It outputs a weight coefficient. */
		for (out = 0; out < p->numOut; out++) 
		{
			/* �W���s����o�͂���B*/
			/* It outputs a coefficient procession. */
			for (in = 0; in < p->numIn + 1; in++)
				logPrintf( _T(" % .8lE"), welm(p, out, in));
			logPrintf( _T("\n"));
		}
	}
	/* �S�ďI������΁AOK�B*/
	return	OK;
}

/*====================================================*/
/* ���Z�l�޸���\������B
// It displays a calculation value vector.
*/
int printVec(
Vector	*vec )
{
	register int	i;

	/* �e�v�f�ɂ��āA*/
	/* for each layer, */
	for (i = 0; i < vec->num; i++) 
	{
		/* �v�f��\������B*/
		/* It displays a element. */
		logPrintf( _T(" % .8lE"), vec->val[i]);
	}
	logPrintf( _T("\n") );

	/* �S�ďI������΁AOK�B*/
	return	OK;
}

/*====================================================*/
/* �w�K�񐔂���͂���B
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

	/* �ŏ��̐錾��ǂݍ��ށB*/
	/* It reads the first declaration. */
	for ( ; (p = fgets(line, 1024, fp)) != NULL && *line == '#'; )
		;
	/* �ǂ߂Ȃ���΁ANULL�B*/
	/* If it isn't possible to read, it is NULL. */
	if (!p)
		return	0;

	/* �s���܂ŁA���L�̗v�����J��Ԃ��B*/
	/* It repeats the following important matter to the end of a sentence. */
	for (times = 0, p = line; ; ) {
		/* �󔒂�ǂݔ�΂��B*/
		/* It skips a blank. */
		for ( ; *p == ' ' || *p == '\t' || *p == '\n'; p++)
			;
		/* �s���Ȃ�΁A���f�B*/
		/* If it does an end of a sentence, it stops. */
		if (!*p)
			break;

		/* ','�����s�܂ł̕������ǂݎ��B*/
		/* It reads a character line until the comma or the starting a new line. */
		for (q = buf; *p && *p != ',' && *p != '\n'; p++)
			*q++	= *p;
		*q	= '\0';

		/* ���l��ǂݎ��B*/
		/* It reads numerical value. */
		if (sscanf(buf, "%d", &value) != 1) {
			/* ���l�łȂ���΁A0��Ԃ�*/
			return	0;
		}
		/* �ǂݎ�������l���i�[����B*/
		/* It sets read numerical value. */
		times	= value;

		// �ǂݎ�ꂽ�̂ŏI��
		break;
	}

	/* �S�ďI������΁A�w����Ԃ��B*/
	return	times;
}

