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
/* ‹–—eÈ¯Ä»²½Ş‚ÌéŒ¾
/* The declaration of the permission net size
*/
#define	NumLayerElement		320				/* The number of the maximum elements in the layer. ‘w“à‚ÌÅ‘å‘w”	*/
#define	NumNetLayer			32				/* The number of the maximum layers. Å‘å‘w”	*/

/*----------------------------------------------------*/
/* d‚İŒW”s—ñ‚Ì\‘¢
	d‚İŒW”s—ñ‚ÍAŠes‚ª“–ŠY‘w‚Ìo—Í‚É‘Î‰‚µA
	Še—ñ‚ª“–ŠY‘w‚Ì“ü—Í‚É‘Î‰‚·‚éBX‚ÉÅŒã‚Ì—ñ‚ÍµÌ¾¯Ä‚Ì€‚ğ•\Œ»‚·‚éB
*/
/* The structure of the weight coefficient procession
	As for the weight coefficient procession, 
	each row corresponds to the output of the concerned layer,
	And each column corresponds to the input of the concerned layer. Moreover. 
	the last line expresses the clause of the offset.
*/
typedef	struct {
	int	numOut;						/* The number of the output elements. o—Í—v‘f” */
	int	numIn;						/* The number of the input elements. “ü—Í—v‘f”(”ñŠÜµÌ¾¯Ä€) */
	double	weight[1];				/* The coefficient procession. ŒW”s—ñ	*/
} WeightMatrix;

#define	welm(w,o,i)			((w)->weight[((w)->numIn + 1)*(o) + (i)])
#define	weightMatrixSize(o, i)	\
				(sizeof(WeightMatrix) + sizeof(double)*((o)*((i)+1)-1))

WeightMatrix	*newWeightMatrix(int numOut, int numIn);


/*----------------------------------------------------*/
/* Še‘w‚Ìo—Í’lÍŞ¸À‚Ì\‘¢
// The structure of the output value vector in each layer
*/
typedef	struct {
	int	num;								/* The number of the elements. —v‘f” */
	double	val[1];							/* The vector value. ÍŞ¸À’l */
} Vector;

#define	vectorSize(n)		(sizeof(Vector) + sizeof(double)*((n)-1))

Vector	*newVector(int size);				/* The generation function of the zero vector. —ëÍŞ¸À‚Ì¶¬ŠÖ” */

/*----------------------------------------------------*/
/* Neural Net‚Ì\‘¢
// The structure of the neural net
*/
typedef	struct {
	char	*name;							/* The neural net name. Neural Net–¼ */
	int	numLayer;							/* The number of the layers. ‘w”(ŠÜ“ü—Í‘w) */
	int	times;								/* The learning number of times ŠwK‰ñ” */
	WeightMatrix	*wMatrix[1];			/* The weight coefficient procession arrangement. */
} NeuralNet;

#define	lelm(n, i)			((n)->wMatrix[i])
#define	neuralNetSize(l)	(sizeof(NeuralNet) + sizeof(WeightMatrix *)*((l)-1))

/*----------------------------------------------------*/
/* ÊŞ¯¸ÌßÛÊß¹Ş°¼®İì‹Æ—Ìˆæ‚Ì\‘¢
// The structure of the back propagation work area
*/
typedef	struct {
	NeuralNet	*net;						/* NN which contains a weight coefficient after the learning. ŠwKŒã‚Ìd‚İŒW”‚ğŠÜ‚ŞNN	*/
	NeuralNet	*deltaNet;					/* NN which expresses the renewal quantity of the weight coefficient. d‚İŒW”‚ÌXV—Ê‚ğ•\Œ»‚·‚éNN	*/
	Vector		*outVec[NumNetLayer];		/* The calculation value of each layer. Še‘w‚Ì‰‰Z’l */
	Vector		*dEdx[NumNetLayer];			/* The İE/İx value of each layer. Še‘w‚Ìİ‚d/İ‚˜’l */
} BPStructure;


NeuralNet		*newNeuralNet(int numLayer, int numElm[]);
NeuralNet		*copyNeuralNet(NeuralNet *net);
NeuralNet		*loadNeuralNet(char *name);	/* Ì§²Ù‚©‚çNN‚ğ“ü—Í‚·‚éŠÖ”		*/
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


extern double	eatha;								/* The convergence coefficient of back propagation. BP‚Ìû‘©ŒW” */
extern double	alpha;								/* The convergence speed coefficient of back propagation. BP‚Ìû‘©‘¬“xŒW” */

/*----------------------------------------------------*/
double	sigmoidFunction(double input);		/* The sigmoid function. ¼¸ŞÓ²ÄŞŠÖ” */


void	logPrintf(_TCHAR *fmt,...);			/* for debug. */

#endif	/* neuralnet_h	*/
