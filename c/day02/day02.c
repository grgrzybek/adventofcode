/*
 * day02.c
 *
 *  Created on: Jan 16, 2018
 *      Author: ggrzybek
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>

const char *program_name;

static const struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'v' },
		{ "file", required_argument, NULL, 'f' }
};

static void print_help(void);

int main(int argc, char *argv[]) {

	program_name = argv[0];

	int optc, lose = 0;
	FILE *input = NULL;

	while ((optc = getopt_long(argc, argv, "hvf:", longopts, NULL)) != -1) {
		switch (optc) {
		case 'f':
			input = fopen(optarg, "r");
			break;
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		case 'v':
			printf("%s\n", PACKAGE_STRING);
			return EXIT_SUCCESS;
		default:
			lose = 1;
		}
	}

	if (lose || optind < argc || !input) {
		if (optind < argc) {
			fprintf(stderr, "%s: extra operand: %s\n", program_name, argv[optind]);
		}
		fprintf(stderr, "Try `%s --help' for more information.\n", program_name);
		exit(EXIT_FAILURE);
	}

	// split by whitespace
	const char delimiters[] = " \t\n";

	// read by line from file
	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;

	int checksum1 = 0, checksum2 = 0;
	while (!feof(input)) {
		// read - number of characters in line, len - size of allocated buffer
		read = getline(&line, &len, input);
		if (read < 0)
			continue;
		char *token = NULL;
		char *_line = strdup(line);
		// we have to free _line, but it'll be strtoked in the meantime
		char *__line = _line;
		size_t size = 0;
		while ((token = strtok(_line, delimiters)) != NULL) {
			if (strlen(token))
				size++;
			_line = NULL;
		}
		free (__line);
		if (size > 0) {
			// another copy
			__line = _line = strdup(line);
			int numbers[size], idx = 0, idx2 = 0;
			while ((token = strtok(_line, delimiters)) != NULL) {
				numbers[idx++] = (int)strtol(token, NULL, 10);
				_line = NULL;
			}
			int min = INT_MAX, max = INT_MIN;
			int n1 = 0, n2 = 0;
			idx = 0;
			while (idx < size) {
				if (numbers[idx] > max)
					max = numbers[idx];
				if (numbers[idx] < min)
					min = numbers[idx];

				idx2 = 0;
				while (idx2 < size) {
					if (numbers[idx] % numbers[idx2] == 0 && idx != idx2) {
						n1 = numbers[idx];
						n2 = numbers[idx2];
					}
					idx2++;
				}
				idx++;
			}
			checksum1 += (max - min);
			checksum2 += (n1 / n2);
			free(__line);

		}
		free(line);
		line = NULL;
	}

	printf("Answer 1: %d\n", checksum1);
	printf("Answer 2: %d\n", checksum2);

	fclose(input);

	exit(EXIT_SUCCESS);
}

static void print_help(void) {
	printf("Usage: %s [OPTION]...\n", program_name);

	puts("Day 02.");

	puts("");
	puts("\
  -h, --help              display this help and exit\n\
  -v, --version           display version information and exit\n");

	puts("\
  -f, --file=FILE         puzzle input (file)\n");

	printf("Report bugs to <%s>.\n", PACKAGE_BUGREPORT);
}
