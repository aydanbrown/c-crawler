#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TAG_COUNT 2
#define MAX_TAG_SIZE 3

char tags[TAG_COUNT][MAX_TAG_SIZE] = {
	"h1", "h2"
};

FILE *read_file;
FILE *write_files[TAG_COUNT];

char c;

void next();
int start();
int match_tag(int *tag);
int match_end_tag(int tag);
int next_open_tag();
int close_tag();
int close_string();
int write(int tag);

// content-extract input_file output_folder
int main(int argc, char *argv[])
{
	read_file = fopen(argv[1], "r");
	//char output_path[100];
	//sprintf(output_path, "%s/content", argv[2]);
	//mkdir(output_path, 777);

	// printf("%s\n", output_path);

	for(int i = 0; i < TAG_COUNT; i++)
	{
		char dir[100];
		sprintf(dir, "%s/content/%s", argv[2], tags[i]);
		write_files[i] = fopen(dir, "a");
	}

	start();

	return 0;
}

void next()
{
	c = fgetc(read_file);
}

int start()
{
	while(!feof(read_file))
	{
		if(next_open_tag() == -1) { return -1; }

		int tag = 1;

		switch(match_tag(&tag))
		{
		case -1:
			return 0;

		case 0:
			if(close_tag() == -1 || write(tag) == -1) { return 0; }
			break;

		default:
			close_tag();

		}
	}

	return 0;
}

int match_tag(int *tag)
{
	if(c == '/') { return 1; }

	int matched_tags[TAG_COUNT];
	for(int t = 0; t < TAG_COUNT; t++)
	{
		matched_tags[t] = 0;
	}

	int tag_idx = 0;
	while(c != ' ' && c != '>')
	{
		if(feof(read_file)) { return -1; }
		if(tag_idx >= TAG_COUNT) { return 1; }

		for(int t = 0; t < TAG_COUNT; t++)
		{
			if(matched_tags[t] == 0)
			{
				if(tags[t][tag_idx] != c)
				{
					matched_tags[t] = 1;
				}
			}
		}

		tag_idx++;
		next();
	}

	for(int t = 0; t < TAG_COUNT; t++)
	{
		if(matched_tags[t] == 0 && tags[t][tag_idx] == 0)
		{
			*tag = t;
			return 0;
		}
	}

	return 1;
}

int match_end_tag(int tag)
{
	if(c != '/') { return 1; }

	next();

	int tag_idx = 0;
	while(c != ' ' && c != '>')
	{
		if(feof(read_file)) { return -1; }
		if(tag_idx >= MAX_TAG_SIZE) { return 1; }
		if(tags[tag][tag_idx] != c) { return 1; }

		tag_idx++;
		next();
	}

	if(tags[tag][tag_idx] == 0) { return 0; }

	return 1;
}

int next_open_tag()
{
	while(c != '<')
	{
		if(feof(read_file)) { return -1; }

		next();
	}

	next();

	return 0;
}

int close_tag()
{
	while(c != '>')
	{
		if(feof(read_file)) { return -1; }

		if(c == '"')
		{
			next();
			if(close_string() == -1) { return -1; }
		}
		else
		{
			next();
		}
	}

	next();

	return 0;
}

int close_string()
{
	while(c != '"')
	{
		if(feof(read_file)) { return -1; }

		if(c == '\\') { next(); }

		next();
	}

	next();

	return 0;
}

int write(int tag)
{
	while(!feof(read_file))
	{
		if(c == '<')
		{
			next();
			fprintf(write_files[tag], "\n");
			int start_pos = ftell(read_file);

			switch(match_end_tag(tag))
			{
			case -1:
				return -1;

			case 0:
				close_tag();
				return 0;

			default:
				fseek(read_file, ftell(read_file) - start_pos - 1, SEEK_CUR);
				next();
				int match = 0;
				switch(match_tag(&match))
				{
				case -1:
					return -1;

				case 0:
					close_tag();
					if(write(match) == -1) { return -1; }
					break;

				default:
					close_tag();
				}
			}
		}
		else
		{
			fprintf(write_files[tag], "%c", c);
			next();
		}
	}

	return 0;
}
