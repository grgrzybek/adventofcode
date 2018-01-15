/*
 * day01.c
 *
 *  Created on: Dec 29, 2017
 *      Author: ggrzybek
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <libgen.h>

#include <linux/limits.h>
#include <sys/mman.h>
#include <sys/stat.h>

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
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		case 'v':
			printf("%s\n", PACKAGE_STRING);
			return EXIT_SUCCESS;
		case 'i':
			input = optarg;
			break;
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

	size_t len;
	if (!input) {
		// collect path components inside path/cpath
		char *path = calloc(PATH_MAX, sizeof(char));
		char *cpath = calloc(PATH_MAX, sizeof(char));

		const char *file_name = "/day01.txt";
		int file_name_length = strlen(file_name);

		// check dirname of executable
		char *path_copy = strdup(argv[0]);
		// `man 3 dirname`: These functions may return pointers to statically allocated memory which
		// may be overwritten by subsequent calls.
		char *dir = dirname(path_copy);
		if (*dir == '/') {
			// absolute, no need to check PWD, we need space for "/day01.txt"
			int n = strlen(dir);
			if (n + file_name_length >= PATH_MAX - 1) {
				fprintf(stderr, "Path too long\n");
				exit(1);
			}
			strncpy(path, dir, n);
			strncpy(path + n, file_name, file_name_length);
			printf("Absolute path to file: %s\n", path);
		} else {
			// relative, prepend with PWD
			if (path != getcwd(path, PATH_MAX)) {
				error(1, errno, "Error getting CWD");
			}
			int pwd_length = strlen(path);
			int rel_dir_length = strlen(dir);
			if (pwd_length + 1 + rel_dir_length + file_name_length >= PATH_MAX - 1) {
				fprintf(stderr, "Path too long\n");
				exit(1);
			}
			int n = pwd_length;
			*(path + n++) = '/';
			strncpy(path + n, dir, rel_dir_length);
			n += rel_dir_length;
			strncpy(path + n, file_name, file_name_length);
			printf("Absolute path to file: %s\n", path);
		}
		free(path_copy);

		printf("PWD: %s\n", path);

		if (cpath != realpath(path, cpath)) {
			error(1, errno, "Can't canonicalize path");
		}
		free(path);
		printf("Canonical Path: %s\n", cpath);

		FILE *file = fopen(cpath, "r");
		if (!file) {
			error(1, errno, "Can't open file");
		}

		// read filecontent to input using mmap
		struct stat st;
		if (stat(cpath, &st) != 0) {
			error(1, errno, "Problems invoking stat()");
		}
		printf("File size: %d\n", st.st_size);
		input = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, file->_fileno, 0);
		if (input == MAP_FAILED) {
			error(1, errno, "Problems calling mmap()");
		}
		len = st.st_size;
		while (input[len - 1] < (int)'0' || input[len - 1] > (int)'9') {
			len--;
		}

		fclose(file);
		free(cpath);
	} else {
		len = strlen(input);
	}

	int sum = 0;
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

	puts("Day 01, part 1.");

	puts("");
	puts("\
  -h, --help              display this help and exit\n\
  -v, --version           display version information and exit\n");

	puts("\
  -i, --input=SEQUENCE    puzzle input\n");

	printf("Report bugs to <%s>.\n", PACKAGE_BUGREPORT);
}
