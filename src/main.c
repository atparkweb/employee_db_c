#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
	printf("Usage: %s -n -f <database file>\n", argv[0]);
	printf("\t -n - create a new file\n");
	printf("\t -f (required) - path to database file\n");
	return;
}

int main(int argc, char *argv[]) {
	char *filepath = NULL;
	bool newfile = false;
	int opt = 0;

	// Database file descriptor
	int dbfd = -1;

	if (argc < 2) {
		print_usage(argv);
	}

	while ((opt = getopt(argc, argv, "nf:")) != -1) {
		switch (opt) {
			case 'n':
				newfile = true;
				break;
			case 'f':
				filepath = optarg;
				break;
			case '?':
				printf("Unknown option -%c\n", opt);
				break;
			default:
				return -1;
		}
	}

	if (filepath == NULL) {
		printf("Filepath is a required argument\n");
		print_usage(argv);
		return 0;
	}

	if (newfile) {
		dbfd = create_db_file(filepath);
		if (dbfd == STATUS_ERROR) {
			printf("Unable to create database file\n");
			return -1;
		}
		printf("Created DB file\n");
	} else {
		dbfd = open_db_file(filepath);
		if (dbfd == STATUS_ERROR) {
			printf("Unable to open database file\n");
			return -1;
		}
	}

	printf("Newfile: %d\n", newfile);
	printf("Filepath: %s\n", filepath);

	return 0;
}
