/*
Function return values:
	-1 = End of file
	0 = Success
	1 or more = Error
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define EXT_TAG_COUNT 8
#define EXT_MAX_TAG_SIZE 7

int ext_id = 0;

char ext_tags[EXT_TAG_COUNT][EXT_MAX_TAG_SIZE] = {
	"title", "h1", "h2", "h3", "h4", "h5", "h6", "p"
};

char ext_folder[100];

FILE *ext_read_file;
FILE *ext_write_files[EXT_TAG_COUNT];

char ext_c;

int ext_init(int id, char *path);
void ext_next();
int ext_start();
int ext_match_tag(int *tag);
int ext_match_end_tag(int tag);
int ext_next_open_tag();
int ext_close_tag();
int ext_close_string();
int ext_write(int tag);


int ext_init(int id, char *path)
{
	printf("Starting program %d\n", id);
	ext_id = id;
	ext_read_file = fopen(path, "r");

	sprintf(ext_folder, "output-%d", id);
	printf("Creating folder: %s\n", ext_folder);
	mkdir(ext_folder, 777);

	for(int i = 0; i < EXT_TAG_COUNT; i++)
	{
		char dir[100];
		sprintf(dir, "%s/%s", ext_folder, ext_tags[i]);
		ext_write_files[i] = fopen(dir, "a");
	}

	return 0;
}

void ext_next()
{
	ext_c = fgetc(ext_read_file);
}

// Start the extraction process until the end of the file
int ext_start()
{
	while(!feof(ext_read_file))
	{
		if(ext_next_open_tag() == -1) { return -1; }

		int tag = 1;

		switch(ext_match_tag(&tag)) 
		{
		case -1:
			return 0;

		case 0:
			if(ext_close_tag() == -1 || ext_write(tag) == -1) { return 0; }
			break;

		default:
			ext_close_tag();

		}
	}

	return 0;
}

int ext_match_tag(int *tag)
{
	if(ext_c == '/') { return 1; }

	int matched_tags[EXT_TAG_COUNT];
	for(int t = 0; t < EXT_TAG_COUNT; t++)
	{
		matched_tags[t] = 0;
	}

	int tag_idx = 0;
	while(ext_c != ' ' && ext_c != '>')
	{
		if(feof(ext_read_file)) { return -1; }
		if(tag_idx >= EXT_TAG_COUNT) { return 1; }

		for(int t = 0; t < EXT_TAG_COUNT; t++)
		{
			if(matched_tags[t] == 0)
			{
				if(ext_tags[t][tag_idx] != ext_c)
				{
					matched_tags[t] = 1;
				}
			}
		}

		tag_idx++;
		ext_next();
	}

	for(int t = 0; t < EXT_TAG_COUNT; t++)
	{
		if(matched_tags[t] == 0 && ext_tags[t][tag_idx] == 0)
		{
			*tag = t;
			return 0;
		}
	}

	return 1;
}

int ext_match_end_tag(int tag)
{
	if(ext_c != '/') { return 1; }

	ext_next();

	int tag_idx = 0;
	while(ext_c != ' ' && ext_c != '>')
	{
		if(feof(ext_read_file)) { return -1; }
		if(tag_idx >= EXT_MAX_TAG_SIZE) { return 1; }
		if(ext_tags[tag][tag_idx] != ext_c) { return 1; }

		tag_idx++;
		ext_next();
	}

	if(ext_tags[tag][tag_idx] == 0) { return 0; }

	return 1;
}

int ext_next_open_tag()
{
	while(ext_c != '<')
	{
		if(feof(ext_read_file)) { return -1; }

		ext_next();
	}

	ext_next();

	return 0;
}

int ext_close_tag()
{
	while(ext_c != '>')
	{
		if(feof(ext_read_file)) { return -1; }

		if(ext_c == '"')
		{
			ext_next();
			if(ext_close_string() == -1) { return -1; }
		}
		else
		{
			ext_next();
		}
	}

	ext_next();

	return 0;
}

int ext_close_string()
{
	while(ext_c != '"')
	{
		if(feof(ext_read_file)) { return -1; }

		if(ext_c == '\\') { ext_next(); }

		ext_next();
	}

	ext_next();

	return 0;
}

int ext_write(int tag)
{
	while(!feof(ext_read_file))
	{
		if(ext_c == '<')
		{
			ext_next();
			fprintf(ext_write_files[tag], "\n");
			int start_pos = ftell(ext_read_file);

			switch(ext_match_end_tag(tag))
			{
			case -1:
				return -1;

			case 0:
				ext_close_tag();
				return 0;

			default:
				fseek(ext_read_file, ftell(ext_read_file) - start_pos - 1, SEEK_CUR);
				ext_next();
				int match = 0;
				switch(ext_match_tag(&match))
				{
				case -1:
					return -1;

				case 0:
					ext_close_tag();
					if(ext_write(match) == -1) { return -1; }
					break;

				default:
					ext_close_tag();
				}
			}
		}
		else
		{
			fprintf(ext_write_files[tag], "%c", ext_c);
			ext_next();
		}
	}

	return 0;
}

int main(void)
{
	ext_init(42, "page.html");
	ext_start();
}
