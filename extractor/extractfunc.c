#include <ctype.h>
#include <stdbool.h>

// Checks if the character (based on position is part of an https link)
bool parsehttps(int c, int idx)
{
	switch(idx)
	{
		case 0: // h
			return c == 104;
		case 1:
		case 2: // t
			return c == 116;
		case 3: // p
			return c == 112;
		case 4: // s
			return c == 115;
		case 5: // :
			return c == 58;
		case 6:
		case 7: // /
			return c == 47;
		default:
			// / a-z A-Z 0-9 - _ .
			return c == 47 || (c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57) || c == 45 || c == 95 || c == 46;
	}
}
