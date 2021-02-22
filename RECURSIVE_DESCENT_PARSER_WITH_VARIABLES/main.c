#include "std.h"

char *prog;

int main(int argc, char** argv)
{
	double answer;
	char *p;

	if( in_param(argc, argv) )
	{
		p = (char*)malloc(100);
		if( !p )
		{
			printf("Memory allocation error\n");
			exit(1);
		}

		/* evaluating expressions before entering an empty string */
		do 
		{
			prog = p;
			printf("Enter expression: ");
			fgets(prog, 100, stdin);
			if( !*prog ) break;
			eval_exp( &answer );
			printf( "Result: %.2f\n", answer );
		} while(*p);
	}
	return 0;
}
