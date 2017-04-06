#include <stdio.h>
#include <dirent.h>

int main(void)
{
	while(1)
	{
		DIR *dp;
		struct dirent *ep;

		dp = opendir("/downloads");
		if(dp != NULL)
		{
			while((ep = readdir(dp)))
			{
				printf("%s\n", ep->d_name);
			}
			closedir(dp);
		}
		else
		{
			printf("Couldn't open the directory\n");
		}
	}

	return 0;
}
