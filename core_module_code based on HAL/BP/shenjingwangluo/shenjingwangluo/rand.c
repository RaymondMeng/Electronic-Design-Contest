#include"rand.h"
static unsigned int RAND_SEED=1;

unsigned int random(void)
{
	RAND_SEED = (RAND_SEED * 123 + 59) % 65536;
	return(RAND_SEED);
}

