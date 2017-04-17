#include <stdio.h>

#define element_count 1

/*
char *elements[element_count] = {
	"itemprop=\"contentUrl\"",
	"itemprop=\"articleBody\"",
	"itemprop=\"datePublished\""
};
*/
char *elements[element_count] = {
	"<meta property=\"og:type\" content=\"article\"/>"
}

int check[element_count];

FILE *file;

char c;

void next()
{
	c = fgetc(file);
}

int match()
{
	file = fopen("page.html", "r");

	next();

	while(!feof(file))
	{
		int matched = 0;
		for(int i = 0; i < element_count; i++)
		{
			if(check[i] != -1)
			{
				if(elements[i][check[i]] == 0)
				{
					check[i] = -1;
					printf("Matched: %s\n", elements[i]);
					matched++;
				}
				else if(elements[i][check[i]] == c)
				{
					check[i]++;
				}
				else
				{
					check[i] = 0;
				}
			}
			else
			{
				matched++;
			}
		}

		if(matched == element_count) { return 0; }

		next();
	}

	return 1;
}

int main(void)
{
	if(match() == 0)
	{
		printf("Verified\n");
	}
	else
	{
		printf("Invalid\n");
	}

	return 0;
}

