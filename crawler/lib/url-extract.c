#include <stdio.h>
#include <string.h>

const char *prefix;// = "https://www.theguardian.com";

const char url_ranges[3][2] = {
	{ 'a', 'z' },
	{ 'A', 'Z' },
	{ '0', '9' }
};

const char url_chars[4] = "/._-";

FILE *in;
FILE *out;

char c;
int idx = 0;

char is_url_char();
int next_line_break(char ch, FILE *file);
void write_url(char *path, int size);
void process_path();

int next()
{
	return c = fgetc(in);
}

// url-extract https://domain.prefix input_file output_file
int main(int argc, char *argv[])
{
	prefix = argv[1];
	in = fopen(argv[2], "r");
	out = fopen(argv[3], "w");
	int len = strlen(prefix);

	while(1)
	{
		next();

		if(feof(in)) {	break; }

		if(idx < len)
		{
			if (c == prefix[idx])
			{
				idx++;
			}
			else
			{
				idx = 0;
			}
		}
		else
		{
			process_path();
			idx = 0;
		}
	}

	fclose(in);
	fclose(out);

	return 0;
}

char is_url_char()
{
	for (int rng = 0; rng < 3; rng++)
	{
		if (c >= url_ranges[rng][0] && c <= url_ranges[rng][1])
		{
			return 1;
		}
	}

	for (int ch = 0; ch < 4; ch++)
	{
		if (c == url_chars[ch])
		{
			return 1;
		}
	}

	return 0;
}

int next_line_break(char ch, FILE *file)
{
	int i = 0;
	while(1) {
		if(feof(file)) { break; }
		if(ch == '\n')
		{
			break;
		}
		ch = fgetc(file);
		i++;
	}
	return i;
}

void process_path()
{
	while (is_url_char() == 1)
	{
		fprintf(out, "%c", c);
		next();
		if(feof(in)) { break; }
	}
	fprintf(out, "\n");
}
