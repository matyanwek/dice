#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Void void
typedef bool Bool;
typedef unsigned char Byte;
typedef ssize_t Int;

typedef struct {
	Byte *data;
	Int len;
} String;

#define string(s) ((String){.data = s, .len = strlen(s)})

String trim(String str, Int n) {
	if (n < 0) {
		n = 0;
	} else if (n > str.len) {
		n = str.len;
	}
	str.data += n;
	str.len -= n;
	return str;
}

Bool hasPrefix(String str, String prefix) {
	return str.len >= prefix.len && memcmp(str.data, prefix.data, prefix.len) == 0;
}

Int indexSubstr(String str, String substr) {
	if (str.len < 1) {
		return -1;
	} else if (substr.len < 1) {
		return 0;
	}

	Int idx = 0;
	while (str.len >= substr.len) {
		Byte *ptr = memchr(str.data, substr.data[0], str.len - substr.len);
		if (ptr == NULL) {
			break;
		}

		Int n = ptr - str.data;
		idx += n;
		str = trim(str, n);
		if (hasPrefix(str, substr)) {
			return idx;
		}

		idx++;
		str = trim(str, 1);
	}

	return -1;
}

Bool cutSubstr(String str, String substr, String *before, String *after) {
	Int idx = indexSubstr(str, substr);
	if (idx < 0) {
		if (before != NULL) {
			*before = str;
		}
		if (after != NULL) {
			*after = (String){.data = str.data + str.len, .len = 0};
		}
		return false;
	} else {
		if (before != NULL) {
			*before = (String){.data = str.data, .len = idx};
		}
		if (after != NULL) {
			idx += substr.len;
			*after = (String){.data = str.data + idx, .len = str.len - idx};
		}
		return true;
	}
}

Bool parseDec(String str, Int *num) {
	if (str.len < 1) {
		return false;
	}

	Int neg = 1;
	if (str.data[0] == '-') {
		neg = -1;
		str = trim(str, 1);
		if (str.len < 1) {
			return false;
		}
	}

	*num = 0;
	for (Int i = 0; i < str.len; i++) {
		if (!isdigit(str.data[i])) {
			return false;
		}
		*num *= 10;
		*num += str.data[i] - '0';
	}

	*num *= neg;
	return true;
}

Bool parsePair(String str, String sep, Int *before, Int *after) {
	String beforeStr, afterStr;
	Bool ok = cutSubstr(str, sep, &beforeStr, &afterStr);
	if (!ok) {
		return false;
	}

	ok = parseDec(beforeStr, before);
	if (!ok) {
		return false;
	}

	ok = parseDec(afterStr, after);
	if (!ok) {
		return false;
	}

	return true;
}

#define help \
	"Usage: %s [SPEC]...\n" \
	"Return a newline-separated list of random numbers as specified by SPECS.\n" \
	"\n" \
	"SPECS can be dice notation (e.g. 3d6 -- roll a 6-sided die 3 times)\n" \
	"or interval notation (e.g. -5..5 -- one number between -5 and 5 inclusive).\n" \
	"\n" \
	"\t-h\tprint this Help and exit\n" \

int main(int argc, char **argv) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srandom((unsigned int)ts.tv_nsec);

	if (argc < 2) {
		printf("%ld\n", (random() % 6) + 1);
		return 0;
	} else if (strcmp(argv[1], "-h") == 0) {
		printf(help, argv[0]);
		return 0;
	}

	Int ret = 0;
	for (Int i = 1; i < argc; i++) {
		String arg = string(argv[i]);
		Bool ok;

		// dice
		Int count, faces;
		ok = parsePair(arg, string("d"), &count, &faces);
		if (ok && count > 0 && faces > 0) {
			while (count-- > 0) {
				printf("%ld\n", (random() % faces) + 1);
			}
			continue;
		}

		// interval
		Int start, end;
		ok = parsePair(arg, string(".."), &start, &end);
		if (ok) {
			printf("%ld\n", (random() % ((end + 1) - start)) + start);
			continue;
		}

		// invalid
		fprintf(stderr, "%s: invalid spec \"%s\", try -h for help\n", argv[0], argv[i]);
		ret++;
	}

	return ret;
}
