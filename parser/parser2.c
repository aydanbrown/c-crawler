#include <stdio.h>

FILE *in;
FILE *out;
short c;

int main(void)
{
	in = fopen("files/in.html");
	out = fopen("files/out.txt");

	while(1)
	{
		
	}

	return 0;
}

int nextchar()
{
	c = fgetc(in);
	return c;
}



int ()
{
	if(c == 60)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Returns a number representing a tag, 0=none 1=h1 2=img 3=div 4=time
int gettag()
{
	nextchar();
	if(c == 104) // h
	{
		if(nextchar() == 49) // 1
		{
			return 1; // h1
		}
	}
	else if(c == 105) // i
	{
		if(nextchar() == 109 && nextchar() == 103) // m and g
		{
			return 2; // img
		}
	}
	else if(c == 100) // d
	{
		if(nextchar() == 105 && nextchar() == 118) // i and v
		{
			return 3; // div
		}
	}
	else if(c == 116) // t
	{
		if(nextchar() == 105 && nextchar() == 109 && nextchar() == 101) // i and m and e
		{
			return 4; // time
		}
	}

	return 0;
}

