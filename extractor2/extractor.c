#include <stdio.h>

const char prefix[27] = "https://www.theguardian.com";
const char url_ranges[3][2] = {
	{ 'a', 'z' },
	{ 'A', 'Z' },
	{ '0', '9' }
};
const char url_chars[4] = "/._-";
FILE *in;
char c;
int idx = 0;

int next () {
	return c = fgetc(in);
}

char is_url_char () {	
	for (int rng = 0; rng < 3; rng++) {
		if (c >= url_ranges[rng][0] && c <= url_ranges[rng][1]) {
			return 1;
		}
	}

	for (int ch = 0; ch < 4; ch++) {
		if (c == url_chars[ch]) {
			return 1;
		}
	}

	return 0;
}

int next_line_break (char ch, FILE *file) {
	int i = 0;
	while(1) {
		if(feof(file)) { break; }
		if(ch == '\\') {
			ch = fgetc(file);
			if(ch == 'n') { break; }
		}
		ch = fgetc(file);
		i++;
	}
	return i;
}

void write_url (char *path, int size) {
	FILE *read = fopen("urls.txt", "r");
	FILE *write = fopen("urls.txt", "a");
	char ch;
	int i = 0;

	while (1) {
		if (feof(read)) {
			for(int i = 0; i < size; i++) {
				fprintf(write, "%c", path[i]);
			}
			fprintf(write, "\n");
			break;
		}

		if (i == size) {
			if(next_line_break(ch, read) == 0) {
				i = 0;
				printf("duplicate\n");
				return;
			}
		} else if (path[i] != ch) {
			next_line_break(ch, read);
			i = 0;
		} else {
			ch = fgetc(read);
			i++;
		}
	}

	fclose(read);
	fclose(write);
}

void process_path () {
	int start = ftell(in);
	while (is_url_char() == 1) {
		next();
		if(feof(in)) { break; }
	}
	int size = ftell(in) - start - 1;
	if(size > 1 && in != 0) {
		char path[size];
		fseek(in, -size - 1, 1);
		fread(path, 1, size, in);
		//for(int i = 0; i < size; i++) { printf("%c", path[i]); }
		//printf("\n");
		write_url(path, size);
	}
}

int main (void) {
	in = fopen("page.html", "r");
	
	while (1) {
		next();

		if (feof(in)) {	break; }

		if (idx < 27) {
			if (c == prefix[idx]) {
				idx++;
			} else {
				idx = 0;
			}
		} else {
			process_path();
			idx = 0;
		}
	}

	fclose(in);

	return 0;
}


