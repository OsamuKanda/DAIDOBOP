#define		STRICT
#include	"../include/bodef.h"
#include	"../include/bomodule.h"


	void	preparePatternNNInput(SampledData far *,int,int,int,int,double *);
	void	prepareCorrNNInput(SampledData far *,int,int,int,int,int,double *);

/*====================================================*/
/* prepare the input vectors for the neural networks
*/
int	prepareBOInputVector(
SampledData far	*dataBuf,
int	bufSize,
int	startIdx,
int	inputSize,
DataFileHeader	*header,
TCCombination	*combi,
double	*patternVec,
double	*corrVec )
{
	int	upper, lower;

	/* get assigned A/D channels. */
	upper	= header->header.adCh[combi->upper];
	lower	= header->header.adCh[combi->lower];

	/* if no A/D channel assigned, return ERROR. */
	if (upper < 0 || lower < 0)		return	ERROR;

	/* prepare the input vector of the pattern neural network. */
	preparePatternNNInput(dataBuf, bufSize, startIdx,
					inputSize, upper, patternVec);

	/* prepare the input vector of the correlation neural network. */
	prepareCorrNNInput(dataBuf, bufSize, startIdx,
					inputSize, upper, lower, corrVec);

	/* after above procedures, return OK. */
	return	OK;
}

/*====================================================*/
/* prepare the input vector of the pattern neural network.
*/
void	preparePatternNNInput(
SampledData far	*dataBuf,
int	bufSize,
int	startIdx,
int	inputSize,
int	ch,
double	*patternVec )
{
	SampledData far	*p;
	int	i;
	int	max, min, val, div;
	int	vec[256];	/* vec[] are declared for debug. */

	/* for each data in the ring buffer, */
	for (i = 0, p = dataBuf + startIdx; i < inputSize; i++) {
		/* get the maximum and the minimum value in the trend. */
		vec[i]	= val	= p->val[ch];
		if (!i)					max = min = val;
		else if (max < val)		max	= val;
		else if (min > val)		min	= val;
		/* increase buffer pointer. */
		if (++p >= dataBuf + bufSize)		p	= dataBuf;
	}

	/* calculate the scaling factor. */
	if ((div = max - min) == 0)		div	= 50;

	/* for each data in the ring buffer, */
	for (i = 0, p = dataBuf + startIdx; i < inputSize; i++) {
		/* scale the teaching data and store the value into vector. */
		*patternVec++	= 0.1 * (p->val[ch] - min);
		/* increase buffer pointer. */
		if (++p >= dataBuf + bufSize)		p	= dataBuf;
	}

	/* append the scaling factor at the tail of the input vector. */
	*patternVec	= div;
}
	void	normalizeTrend(SampledData far *,int,int,int,int,double *);
	double	calcCorrelation(double *,double *,int);

/*====================================================*/
/* prepare the input vector of the correlation neural network.
*/
void	prepareCorrNNInput(
SampledData far	*dataBuf,
int	bufSize,
int	startIdx,
int	inputSize,
int	upperCh,
int	lowerCh,
double	*corrVec )
{
	double	upperTrend[256];
	double	lowerTrend[256];

	/* normalize the input trends of temperatures. */
	normalizeTrend(dataBuf, bufSize, startIdx, inputSize, upperCh, upperTrend);
	normalizeTrend(dataBuf, bufSize, startIdx, inputSize, lowerCh, lowerTrend);

	/* calculate the correlation of the input trends
		and store the value into the second column of the input vector. */
	corrVec[1]	= calcCorrelation(upperTrend, lowerTrend, inputSize);
}

/*====================================================*/
/* normalize the input trends of temperatures.
*/
void	normalizeTrend(
SampledData far	*dataBuf,
int	bufSize,
int	startIdx,
int	inputSize,
int	ch,
double	*normalTrend )
{
	register SampledData far	*p;
	register int	i;
	double	val, xsum, xxsum, ave, sigma;

	/* for each data in the ring buffer, */
	xsum = xxsum	= 0.0;
	for (i = 0, p = dataBuf + startIdx; i < inputSize; i++) {
		/* calculate the summation of the trend. */
		xsum	+= (val = p->val[ch]);
		xxsum	+= val * val;
		/* increase buffer pointer. */
		if (++p >= dataBuf + bufSize)		p	= dataBuf;
	}

	/* calculate the average of the trend. */
	ave		= xsum / inputSize;
	/* calculate the sigma of the trend. */
//	sigma	= sqrt((xxsum - xsum * xsum / inputSize) / inputSize); // 2010/05 Ž®•ÏX [/ inputSize] -> [* InputSize]
	sigma	= sqrt((xxsum - xsum * xsum * inputSize) / inputSize);

	/* test the sigma. */
	if (sigma == 0.0)			sigma	= 25.0;

	/* for each data in the ring buffer, */
	for (i = 0, p = dataBuf + startIdx; i < inputSize; i++) {
		/* normalize the data and store the value into normalized vector. */
		*normalTrend++	= (p->val[ch] - ave) / sigma;
		/* increase buffer pointer. */
		if (++p >= dataBuf + bufSize)		p	= dataBuf;
	}
}

/*====================================================*/
/* calculate the correlation of the T.C. pair trend.
*/
double	calcCorrelation(
double	*upper,
double	*lower,
int	inputSize )
{
	double	rel;
	double	sum, max;
	register int	i, r, peakT;

	max = 0.0;
	for (r = 0, peakT = 0; r < inputSize * 2 / 3; r++) {
		for (i = 0, sum = 0.0; i < inputSize; i++) {
			sum	+= (i + r < inputSize) ?	upper[i] * lower[i + r] :
											upper[i] * lower[inputSize - 1];
		}
		rel	= sum / inputSize;
		if (rel > max)		max	= rel,	peakT	= r;
	}
	return	(double)peakT / ((double)inputSize * 2 / 3);
}

/*====================================================*/
/* execute the B.O. prediction neural networks.
*/
double	doBONeuralNet(
NeuralNet	*patNet,
NeuralNet	*corrNet,
double	*patternVec,
double	*corrVec,
double	*patOutP,
double	*corOutP )
{
	double	patternOut, corrOut;

	/* test the neural networks
		and return 0.0 if the neural networks are invalid. */
	if (!patNet || !corrNet)		return	0.0;

	/* execute the pattern neural network. */
	doNeuralNet(patNet, patternVec, &patternOut);

	/* store the output value of the pattern neural network into the first
		column of the input vector of the correlation neural network. */
	corrVec[0]	= patternOut;

	/* execute the correlation neural network. */
	doNeuralNet(corrNet, corrVec, &corrOut);

	/* store the output values if the pointers are specified. */
	if (patOutP)		*patOutP	= patternOut;
	if (corOutP)		*corOutP	= corrOut;

	/* return the value of the correlation neural network. */
	return	corrOut;
}

