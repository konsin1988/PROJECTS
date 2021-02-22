/* return 0, if there are any parameters (the function takes no parameters)*/

#include "std.h"

int in_param(int argc, char** argv)
{
	if( argc > 1 )
	{
		printf("%s requires no parameters", argv[0]);
		return 0;
	}
	return 1;
}
