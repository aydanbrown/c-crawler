#include <stdio.h>

FILE *html;
FILE *out;
int c;

int main(void)
{

	

	return 0;
}

int next()
{
	c = fgetc(html);

	return c;
}

// itterate until next tag returning the tag type
int next_open_tag()
{
	// if current character isn't < then itterate until it is
	if(c != 60)
	{
		while(1)
		{
			// end if character is < or file has ended
			if(next() == 60)
			{
				break;
			}
			// return if the file has ended
			else if(feof(html))
			{
				return -1;
			}
		}
	}

	switch(next)
	{
		case 104: // h
			if(next() == 49) // 1
			{
				return 1; // h1
			}
		case 105: // i
			if(next() == 109 && next() == 103) // m and g
			{
				return 2; // img
			}
		case 100: // d
			if(next() == 105 && next() == 118) // i and v
			{
				return 3; // div
			}
		case 116: // t
			if(next() == 105 && next() == 109 && next() == 101)
			{
				return 4; // time
			}
	}

	return 0;
}

// itterate until the end of a specific tag
int next_end_tag(char tag[])
{
	// if current character isn't < then itterate until it is
	if(c != 0)
	{
		while(1)
		{
			// stop if character is <
			if(next == 60)
			{
				break;
			}
			// return if the file has ended
			else if(feof(html))
			{
				return -1;
			}
		}
	}

	// itterate until character end tag
	while(1)
	{
		if(next() == 47) // /
		{
			int matched = 1;
			// itterate tag checking for a match
			for(int i = 0; i < sizeof(tag) / 4; i++)
			{
				if(next() != tag[i])
				{
					matched = 0;
					break;
				}
			}
			// if the string matched
			if(matched == 1)
			{
				return 0;
			}
		}
	}
	return 0;
}

// itterate until the end of the currently open tag
int end_open_tag()
{
	// return if the current character is >
	if(c == 62)
	{
		return 0;
	}

	// itterate until the end of the tag
	while(1)
	{
		// done if character is >
		if(next() == 62)
		{
			return 0;
		}
		// handle quotation mark
		else if(c == 34)
		{
			// itterate until end of string, return if end of file
			if(end_string() == -1)
			{
				return -1;
			}
		}
		// return if end of file
		else if(feof(html))
		{
			return -1;
		}
	}
}

// itterate until the end of the current string
int end_string()
{
	// return if the current character is a quotation mark
	if(c == 34)
	{
		return 0;
	}

	while(1)
	{
		if(next() == 34)
		{
			return 0;
		}
		// if the character is a backslash (escape) ignore the next character
		else if(c == 92)
		{
			next();
		}
		else if(feof(html))
		{
			return -1;
		}
	}

	return 0;
}

// match a sequence of characters
int match_chars(char chars[])
{
	// itterate the characters in the chars array
	for(int i = 0; i < sizeof(chars) / 4; i++)
	{
		if(c != chars[i])
		{
			return 0;
		}
		next();
	}

	return 1;
}
