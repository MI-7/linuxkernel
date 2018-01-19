#include "maths.h"

int pow(int base, int power)
{
	while (power > 1)
	{
		base = base * base;
		power--;
	}

return base;
}
