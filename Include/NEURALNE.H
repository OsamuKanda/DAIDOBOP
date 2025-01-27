/************************************************************************

	Copyright (C) 1995 by Sumitomo Heavy Industries, Ltd. (SHI)

	This Program is the property of SHI and is confidential
	it shall NOT be reproduced, NOR used for any other purpose,
	NOR submittied to outside parties without SHI's permition.

*************************************************************************/

/*----------------------------------------------------------------------*/
/*
		Structure Declaration of Neural Network
		Function definition for NN Execution and Back propagation

*/


#ifndef	neuralnet_h
# define	neuralnet_h
#include	<TCHAR.H>
#include	<math.h>

/*----------------------------------------------------*/
/* general symbols
*/
#ifndef	TRUE
# define	TRUE	1
#endif
#ifndef	FALSE
# define	FALSE	0
#endif
#ifndef	ERROR
/* caution!, the value of ERROR is not -1 in windows apprication. */
# define	ERROR	0
#endif
#ifndef	OK
/* caution!, the value of OK is not 0 in windows apprication. */
# define	OK		(!ERROR)
#endif
#ifndef	M_PI
# define	M_PI	3.14159265358979323846
#endif
#ifndef	newcell
# include	<malloc.h>
# define	newcell(type)		(type *)calloc(1, sizeof(type))
#endif

/*----------------------------------------------------*/
/* 許容ﾈｯﾄｻｲｽﾞの宣言
/* The declaration of the permission net size
*/
#define	NumLayerElement		320				/* The number of the maximum elements in the layer. 層内の最大層数	*/
#define	NumNetLayer			32				/* The number of the maximum layers. 最大層数	*/

/*----------------------------------------------------*/
/* 重み係数行列の構造
	重み係数行列は、各行が当該層の出力に対応し、
	各列が当該層の入力に対応する。更に最後の列はｵﾌｾｯﾄの項を表現する。
*/
/* The structure of the weight coefficient procession
	As for the weight coefficient procession, 
	each row corresponds to the output of the concerned layer,
	And each column corresponds to the input of the concerned layer. Moreover. 
	the last line expresses the clause of the offset.
*/
typedef	struct {
	int	numOut;						/* The number of the output elements. 出力要素数 */
	int	numIn;						/* The number of the input elements. 入力要素数(非含ｵﾌｾｯﾄ項) */
	double	weight[1];				/* The coefficient procession. 係数行列	*/
} WeightMatrix;

#define	welm(w,o,i)			((w)->weight[((w)->numIn + 1)*(o) + (i)])
#define	weightMatrixSize(o, i)	\
				(sizeof(WeightMatrix) + sizeof(double)*((o)*((i)+1)-1))

WeightMatrix	*newWeightMatrix(int numOut, int numIn);


/*----------------------------------------------------*/
/* 各層の出力値ﾍﾞｸﾀの構造
// The structure of the output value vector in each layer
*/
typedef	struct {
	int	num;								/* The number of the elements. 要素数 */
	double	val[1];							/* The vector value. ﾍﾞｸﾀ値 */
} Vector;

#define	vectorSize(n)		(sizeof(Vector) + sizeof(double)*((n)-1))

Vector	*newVector(int size);				/* The generation function of the zero vector. 零ﾍﾞｸﾀの生成関数 */

/*----------------------------------------------------*/
/* Neural Netの構造
// The structure of the neural net
*/
typedef	struct {
	char	*name;							/* The neural net name. Neural Net名 */
	int	numLayer;							/* The number of the layers. 層数(含入力層) */
	int	times;								/* The learning number of times 学習回数 */
	WeightMatrix	*wMatrix[1];			/* The weight coefficient procession arrangement. */
} NeuralNet;

#define	lelm(n, i)			((n)->wMatrix[i])
#define	neuralNetSize(l)	(sizeof(NeuralNet) + sizeof(WeightMatrix *)*((l)-1))

/*----------------------------------------------------*/
/* ﾊﾞｯｸﾌﾟﾛﾊﾟｹﾞｰｼｮﾝ作業領域の構造
// The structure of the back propagation work area
*/
typedef	struct {
	NeuralNet	*net;						/* NN which contains a weight coefficient after the learning. 学習後の重み係数を含むNN	*/
	NeuralNet	*deltaNet;					/* NN which expresses the renewal quantity of the weight coefficient. 重み係数の更新量を表現するNN	*/
	Vector		*outVec[NumNetLayer];		/* The calculation value of each layer. 各層の演算値 */
	Vector		*dEdx[NumNetLayer];			/* The ∂E/∂x value of each layer. 各層の∂Ｅ/∂ｘ値 */
} BPStructure;


NeuralNet		*newNeuralNet(int numLayer, int numElm[]);
NeuralNet		*copyNeuralNet(NeuralNet *net);
NeuralNet		*loadNeuralNet(char *name);	/* ﾌｧｲﾙからNNを入力する関数		*/
void	freeNeuralNet(NeuralNet *net);
void	doBPNeuralNet(BPStructure *bp, double *input, double *target);
void	doNNWithHistory(NeuralNet *net, double *input, Vector **output);
void	setupBPWorkArea(BPStructure *bp);
void	doNeuralNet(NeuralNet *net,double *input, double *output);
void	doLayerCalculation(WeightMatrix *wm, double *input, double *output);
void	setRandomWeight(NeuralNet *net);
int		saveNeuralNet(char *fname, char *name, NeuralNet *net);
int		parseNumLayer(FILE *fp, int numElement[]);
int		parseWeightMatrix(FILE *fp, WeightMatrix *matrix);
void	printBP(BPStructure *bp);
int		printNN(NeuralNet *net);
int		printVec(Vector *vec);
int		parseLarningTimes(FILE *fp);


extern double	eatha;								/* The convergence coefficient of back propagation. BP時の収束係数 */
extern double	alpha;								/* The convergence speed coefficient of back propagation. BP時の収束速度係数 */

/*----------------------------------------------------*/
double	sigmoidFunction(double input);		/* The sigmoid function. ｼｸﾞﾓｲﾄﾞ関数 */


void	logPrintf(_TCHAR *fmt,...);			/* for debug. */

#endif	/* neuralnet_h	*/
