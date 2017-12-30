/*
 * day01a.c
 *
 *  Created on: Dec 29, 2017
 *      Author: ggrzybek
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

const char *program_name;

static const struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'v' },
		{ "input", required_argument, NULL, 'i' }
};

static void print_help(void);

int main(int argc, char *argv[]) {

	program_name = argv[0];

	int optc, lose = 0;
	const char *input = NULL;

	while ((optc = getopt_long(argc, argv, "hvi:", longopts, NULL)) != -1) {
		switch (optc) {
		case 'i':
			input = optarg;
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

	int sum = 0;
	size_t len = strlen(input);
	for (int i = 0; i < len; i++) {
		if (input[i] == input[(i + 1) % len])
			sum += input[i] - (int)'0';
	}
	printf("Answer 1: %d\n", sum);

	sum = 0;
	for (int i = 0; i < len; i++) {
		if (input[i] == input[(i + (len / 2)) % len])
			sum += input[i] - (int)'0';
	}
	printf("Answer 2: %d\n", sum);

	exit(EXIT_SUCCESS);
}

static void print_help(void) {
	printf("Usage: %s [OPTION]...\n", program_name);

	fputs("Day 01, part 1.\n", stdout);

	puts("");
	fputs("\
  -h, --help              display this help and exit\n\
  -v, --version           display version information and exit\n",
			stdout);

	puts("");
	fputs("\
  -i, --input=SEQUENCE    puzzle input\n", stdout);

	printf("\n");
	printf("Report bugs to <%s>.\n", PACKAGE_BUGREPORT);
}
