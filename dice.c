#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static bool is_digit(char c) {
	return '0' <= c && c <= '9';
}

bool _atoi(char *s, size_t len, int *n) {
	if (len == 0) {
		return true;
	}

	int factor = 1;
	size_t end = 0;
	switch (s[end]) {
	case '-':
		factor = -1;
		end++;
		break;
	case '+':
		break;
		end++;
	default:
		break;
	}
	if (end >= len) {
		return false;
	}

	*n = 0;
	for (size_t i = len; i > end; i--) {
		char c = s[i - 1];
		if (!is_digit(c)) {
			return false;
		}
		*n += factor * (int)(c - '0');
		factor *= 10;
	}

	return true;
}

bool parse_dice(char *s, size_t len, int *dice, int *faces) {
	char *sep = strchr(s, 'd');
	if (sep == NULL) {
		return false;
	}

	if (!_atoi(s, sep - s, dice)) {
		return false;
	}

	return _atoi(sep + 1, len - ((sep + 1) - s), faces);
}

bool parse_interval(char *s, size_t len, int *start, int *end) {
	char *sep = strchr(s, '.');
	if (sep == NULL || s + len <= sep + 2 || *(sep + 1) != '.') {
		return false;
	}

	if (!_atoi(s, sep - s, start)) {
		return false;
	}

	return _atoi(sep + 2, len - ((sep + 2) - s), end);
}

int main(int argc, char **argv) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srandom((unsigned int)ts.tv_nsec);

	if (argc == 1) {
		printf("%d\n", ((int)random() % 6) + 1);
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		size_t len = strlen(argv[i]);

		int dice, faces;
		if (parse_dice(argv[i], len, &dice, &faces)) {
			for (; dice > 0; dice--) {
				printf("%d\n", ((int)random() % faces) + 1);
			}
			continue;
		}

		int start, end;
		if (parse_interval(argv[i], len, &start, &end)) {
			printf("%d\n", ((int)random() % ((end + 1) - start)) + start);
			continue;
		}

		error(0, 0, "invalid argument \"%s\"; argument must be a dice roll (XdX) or an interval (X..X)", argv[i]);
	}

	return 0;
}
