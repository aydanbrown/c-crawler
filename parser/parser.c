#include <stdio.h>

static const short TAG_OPEN = 60; // <
static const short TAG_CLOSE = 62; // >
static const short QUOTE = 34; // "
static const short SLASH = 47; // /
static const short ESCAPE = 92; // \"

FILE *in;
FILE *out;
short c;
short idx = 0;

short main(void)
{

	
	in = fopen("files/in.html", "r");
	out = fopen("files/out.txt", "a");

	while(1)
	{
		c = fgetc(in);
		// finished if at the end of the file
		if(feof(in))
		{
			break;
		}
		// switch the interpretation based on the current state
		switch(state)
		{
			case 0:// no state
				if(c == TAG_OPEN)
				{
					state = 1;
				}
				break;
			case 1:// tag opened
				if(c == 112 || c == 67) // p, P
				{
					state = 2;
	}

	return 0;
}

void next
