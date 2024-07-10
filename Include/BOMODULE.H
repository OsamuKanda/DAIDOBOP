#ifndef	bomodule_h
# define	bomodule_h

#include	"../include/bodef.h"
#include	"../include/neuralne.h"

/*----------------------------------------------------*/
/* prepare the input vectors for neural networks
*/
int	prepareBOInputVector(
		SampledData far	*dataBuf,
		int				bufSize,
		int				startIdx,
		int				inputSize,
		DataFileHeader	*header,
		TCCombination	*combi,
		double			*patternVec,
		double			*corrVec
);

/*----------------------------------------------------*/
/* execute the B.O. prediction neural networks.
*/
double	doBONeuralNet(
		NeuralNet	*patNet,
		NeuralNet	*corrNet,
		double		*patternVec,
		double		*corrVec,
		double		*patternOut,
		double		*corrOut
);

#endif	/* bomodule_h	*/
