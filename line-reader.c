#include <stdio.h>

#define LEN 100

int main()
{
	FILE *fp;
	char str[LEN];

	fp = fopen("urls.txt" , "r");
	while( fgets (str, LEN, fp)!=NULL )
	{
		for(int i = 0; i < LEN; i++)
		{
			if(str[i] == '\n')
			{
				str[i] = 0;
				break;
			}
		}
		puts(str);
	}
	fclose(fp);

	return(0);
}
