#include "parser.h"
#include "std.h"

/* the module contains a simple parser that does not recognize variables */

/* contains the parsed expression */
extern char *prog;

char token[80];
char tok_type;

/* analyzer entry point */
void eval_exp( double *answer )
{
	get_token();
	if( !*token )
	{
		serror(2);
		return;
	}

	eval_exp2( answer );

	if( *token )
	{
		/* the last token must be zero */
		serror(0);
	}
}

/* addition or substraction of two terms */
void eval_exp2( double *answer )
{
	register char op;
	double temp;

	eval_exp3( answer );
	while( (op = *token) == '+' || op == '-' )
	{
		get_token();
		eval_exp3( &temp );
		switch( op )
		{
			case '-':
				*answer = *answer - temp;
				break;
			case '+':
				*answer = *answer + temp;
				break;
		}
	}
}

/* multiplication or division of two terms */
void eval_exp3( double *answer )
{
	register char op;
	double temp;

	eval_exp4( answer );
	while( (op = *token) == '*' || op == '/' || op == '%' )
	{
		get_token();
		eval_exp4( &temp );
		switch( op ) 
		{
			case '*':
				*answer = *answer * temp;
				break;
			case '/':
				if( temp == 0.0 )
				{
					/* division by zero */
					serror(3);
					*answer = 0.0;
				}
				else 
					*answer = *answer / temp;
				break;
			case '%':
				*answer = (int) *answer % (int) temp;
				break;
		}
	}
}

/* exponentiation */
void eval_exp4( double *answer )
{
	double temp, ex;
	register int t;

	eval_exp5( answer );

	if( *token == '^' )
	{
		get_token();
		eval_exp4( &temp );
		ex = *answer;
		if( temp == 0.0 )
		{
			*answer = 1.0;
			return;
		}
		for( t=temp; t>1; --t)
			*answer = (*answer) * (double)ex;
	}
}

/* multiplication of unary operators + and - */
void eval_exp5( double *answer )
{
	register char op;

	op=0;
	if( (tok_type == DELIMITER) && *token=='+' || *token == '-' )
	{
		op = *token;
		get_token();
	}
	eval_exp6( answer );
	if( op == '-')
		*answer = -(*answer);
}

/* evaluating an expression in brackets */
void eval_exp6( double *answer )
{
	if( *token == '(' ) 
	{
		get_token();
		eval_exp2( answer );
		if( *token != ')' )
			serror(1);
		get_token();
	}
	else
		atom( answer );
}

/* getting the value in parentheses */
void atom( double *answer )
{
	if( tok_type == NUMBER )
	{
		*answer = atof(token);
		get_token();
		return;
	}
	/* otherwise syntax error in expression */
	serror(0);
}

/* token expression into input stream */
void putback( void )
{
	char *t;

	t = token;
	for( ; *t; t++ ) prog--;
}

/* error message display */
void serror( int error )
{
	static char *e[] = 
	{
		"Syntax error",
		"Unbalanced brackets",
		"No expression",
		"Division by zero",
	};
	printf( "%s\n", e[error] );
}

/* return the next token */
void get_token(void)
{
	register char *temp;

	tok_type=0;
	temp=token;
	*temp = '\0';
	/* end of expression */
	if( !*prog )return;

	/* skip spaces, tabs, and blank lines */
	while( isspace(*prog) ) 
		++prog;

	if( strchr("+-*/%^=()", *prog) )
	{
		tok_type = DELIMITER;

		/* move to the next char */
		*temp++ = *prog++;
	}
	else if( isalpha(*prog) )
	{
		while( !isdelim(*prog) )
			*temp++ = *prog++;
		tok_type = VARIABLE;
	}

	else if( isdigit(*prog) )
	{
		while( !isdelim(*prog) )
			*temp++ = *prog++;
		tok_type = NUMBER;
	}

	*temp = '\0';
}

/* Returns true, if c is delimiter */
int isdelim( char c )
{
	if( strchr("+-/*%^=()", c) || c==9 || c=='\r' || c==0 )
		return 1;
	return 0;
}
