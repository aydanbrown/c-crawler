#include <stdio.h>
#include <stdbool.h>
#include <extract.h>

int main(void)
{
	FILE *in;
	FILE *out;
	int c;
	int idx;
	
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "a");
	while(1)
	{
		c = fgetc(in);
		if(feof(in))
		{
			break;
		}
		// check if the character is part of an https link
		if(parsehttps(c, idx) == true)
		{
			// if the index has passed "https://" append the character to the output file
			if(idx > 7)
			{
				fprintf(out, "%c", c);
			}
			idx++;
		}
		else
		{
			// if the index has passed "https://" append a new line to the output file
			if(idx > 7)
			{
				fprintf(out, "%s", "\n");
			}
			idx = 0;
		}
		//printf("%c", c);
	}
	fclose(in);
	fclose(out);

	return(0);
}

