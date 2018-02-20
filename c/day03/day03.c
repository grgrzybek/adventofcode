/*
 * day03.c
 *
 *  Created on: Feb 20, 2018
 *      Author: ggrzybek
 */

#define _GNU_SOURCE

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
		{ "input", required_argument, NULL, 'i' }
};

static int input = 312051;

static void print_help(void);

int main(int argc, char *argv[]) {

	program_name = argv[0];

	int optc, lose = 0;

	while ((optc = getopt_long(argc, argv, "hvi:", longopts, NULL)) != -1) {
		switch (optc) {
		case 'i':
			input = (int) strtol(optarg, NULL, 10);
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

	if (lose || optind < argc) {
		if (optind < argc) {
			fprintf(stderr, "%s: extra operand: %s\n", program_name, argv[optind]);
		}
		fprintf(stderr, "Try `%s --help' for more information.\n", program_name);
		exit(EXIT_FAILURE);
	}

	// find the size of square which includes input number
	int size = 1;
	while (1) {
		if (size * size >= input)
			break;
		size += 2;
	}
//	printf("size: %d\n", size);

	int br = size * size;
	int bl = br - size + 1;
	int tl = bl - size + 1;
	int tr = tl - size + 1;
	int br2 = tr - size + 2; // a bit above br
//	printf("%d/%d/%d/%d/%d\n", br2, tr, tl, bl, br);

	// first manhattan coordinate is always half of the size - 1
	int c1 = (size - 1) / 2;

	// second manhattan coordinate is the distance from the center of the edge
	// finding correct edge of square
	int a = br2, b = tr;
	while (1) {
		if (!(a <= input && input <= b))
			a = tr, b = tl;
		else
			break;
		if (!(a <= input && input <= b))
			a = tl, b = bl;
		else
			break;
		if (!(a <= input && input <= b))
			a = bl, b = br;
		else
			break;
		if (!(a <= input && input <= b)) {
			fprintf(stderr, "Can't find location for %d!\n", input);
			exit(EXIT_FAILURE);
		}
		break;
	}

	// to make right edge equal to others
	if (a == br2)
		a--;
//	printf("a=%d, b=%d\n", a, b);

	int half = (size + 1) / 2;
	int middle = a + half - 1;
	int c2 = 0;
	if (input < middle)
		c2 = middle - input;
	else if (input > middle)
		c2 = input - middle;

	printf("Answer 1: %d:%d=%d\n", c1, c2, c1 + c2);
//	printf("Answer 2: %d\n", checksum2);

	exit(EXIT_SUCCESS);
}

static void print_help(void) {
	printf("Usage: %s [OPTION]...\n", program_name);

	puts("Day 03.");

	puts("");
	puts("\
  -h, --help              display this help and exit\n\
  -v, --version           display version information and exit\n");

	printf("Report bugs to <%s>.\n", PACKAGE_BUGREPORT);
}
