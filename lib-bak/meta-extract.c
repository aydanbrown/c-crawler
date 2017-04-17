/*
Function return values:
	-1 = End of file
	0 = Success
	1 or more = Error
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define META_COUNT 5
#define MAX_META_SIZE 25

char attributes[META_COUNT][MAX_META_SIZE] = {
	"author",
  "description",
  "keywords",
  "article:section",
  "article:published_time"
};

FILE *read_file;
int extracted[META_COUNT];

char c;

int init(char *path);
void next();
int next_open_tag();
int match_meta_tag();
int close_tag();
int next_attribute();
int get_name_attribute();
int write_value_attribute();
int next_open_string();
int match_string();
int close_string();

int main(void)
{
	init("page.html");

	while(!feof(read_file))
	{
		if(next_open_tag() == -1)
		{
			return 0;
		}

		switch(match_meta_tag())
		{
		case -1:
			return 0;
		case 1:
			close_tag();
			break;
		case 0:
			if(next_attribute() != 0) { break; }

			int name = 0;
			if(get_name_attribute(&name) == 0)
			{
				if(write_value_attribute(name) == 0)
				{
					printf("Extracted: %s\n", attributes[name]);
					extracted[name] = 1;

					int total = 0;

					for(int i = 0; i < META_COUNT; i++)
					{
						if(extracted[i] == 1) { total++; }
					}

					if(total == META_COUNT) {
						printf("Extracted all meta tags\n");
						return 0;
					}
				}
			}

			next();

			break;
		}
	}

	printf("File ended\n");

	return 0;
}

int init(char *path)
{
	read_file = fopen(path, "r");
	mkdir("meta", 775);

	return 0;
}

void next()
{
	c = fgetc(read_file);
}

char next_char()
{
	return c = fgetc(read_file);
}

int next_match(char match)
{
	while(c != match)
	{
		if(feof(read_file)) { return -1; }
		next();
	}

	next();

	return 0;
}

int next_match_string(char * str)
{
	int ch = 0;
	while(str[ch] == c && str[ch] != 0)
	{
		if(feof(read_file)) { return -1; }
		ch++;
		next();
	}

	if(str[ch] == 0) { return 0; }

	return 1;
}

int next_open_tag()
{
	while(c != '<')
	{
		if(feof(read_file)) { return -1; }
		if(c == '"')
		{
			close_string();
		}
		else
		{
			next();
		}
	}

	next();

	return 0;
}

int match_meta_tag()
{
	char *tag = "meta";

	for(int i = 0; i < 4; i++)
	{
		if(c != tag[i]) { return 1; }
		next();
	}

	if(c != ' ') { return 1; }

	return 0;
}

int close_tag()
{
	while(!feof(read_file))
	{
		if(c == '>')
		{
			return 0;
		}
		else if(c == '"')
		{
			next();
			close_string();
		}

		next();
	}

	return -1;
}

int next_attribute()
{
	while(!feof(read_file))
	{
		if(c == ' ')
		{
			next();
			return 0;
		}
		else if(c == '>')
		{
			next();
			return 1;
		}

		next();
	}

	return -1;
}

int next_open_string()
{
	while(!feof(read_file))
	{
		if(c == '"')
		{
			next();
			return 0;
		}
		else if(c == '>')
		{
			next();
			return 1;
		}
		next();
	}

	return -1;
}

int get_name_attribute(int * attribute)
{
	while(!feof(read_file))
	{
		while(c == ' ') { next(); }
		if(c == '>') { return 1; }
		
		if(next_match_string(c == 'n' ? "name=\"" : "property=\"") == 0)
		{
			int ch = 0;
			int matches[META_COUNT];

			for(int i = 0; i < META_COUNT; i++) { matches[i] = 0; }

			while(c != '"')
			{
				if(feof(read_file)) { return -1; }
				
				int match_count = 0;

				for(int i = 0; i < META_COUNT; i++)
				{
					if(matches[i] == 0)
					{
						if(c != attributes[i][ch])
						{
							matches[i] = 1;
						}
						else
						{
							match_count++;
						}
					}
				}

				if(match_count == 0)
				{
					close_string();
					break;
				}

				next();
				ch++;
			}

			for(int i = 0; i < META_COUNT; i++)
			{
				if(matches[i] == 0 && attributes[i][ch] == 0)
				{
					*attribute = i;
					return 0;
				}
			}

			return 1;
		}

		while(c != ' ')
		{
			if(feof(read_file)) { return -1; }
			if(c == '"')
			{
				next();
				close_string();
			}

			next();
		}
	}

	return -1;
}

int write_value_attribute(int file)
{
	while(!feof(read_file))
	{
		while(c == ' ') { next(); }
		if(c == '>') { return 1; }
		
		if(next_match_string("content=\"") == 0)
		{
			char dir[100];
			sprintf(dir, "meta/%s", attributes[file]);
			FILE *write_file = fopen(dir, "a");
			while(!feof(read_file))
			{
				if(c == '\\')
				{
					fprintf(write_file, "%c", c);
					next();
				}
				else if(c == '"')
				{
					fprintf(write_file, "%c", '\n');
					return 0;
				}
				fprintf(write_file, "%c", c);
				next();
			}
			fclose(write_file);
		}

		next();
	}

	return -1;
}

int match_string(char *string)
{
	int i = 0;
	while(!feof(read_file))
	{
		if(c == '\\') { next(); }

		if(string[i] == 0)
		{
			if(c == '"')
			{
				next();
				return 0;
			}
			else
			{
				next();
				return 1;
			}
		}
		else if(c == '"')
		{
			next();
			return 1;
		}
		next();
		i++;
	}

	return -1;
}	

int close_string()
{
	while(!feof(read_file))
	{
		if(c == '\\') { next(); }
		if(c == '"') {
			next();
			return 0;
		}
		next();
	}

	return -1;
}
