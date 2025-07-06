#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "common.h"
#include "parse.h"


int validate_db_header(int fd, struct dbheader_t **headerOut) {
	if (fd < 0) {
		printf("Got a bad fd from the user\n");
		return STATUS_ERROR;
	}

	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == -1) {
		printf("Malloc failed to create dbheader\n");
		return STATUS_ERROR;
	}

	if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
		perror("read");
		free(header);
		return STATUS_ERROR;
	}

	// Unpack data to host endianess
	header->version = ntohs(header->version);
	header->count = ntohs(header->count);
	header->magic = ntohl(header->magic);
	header->filesize = ntohl(header->filesize);

	if (header->magic != HEADER_MAGIC) {
		printf("Improper header magic\n");
		free(header);
		return -1;
	}

	if (header->version != 1) {
		printf("Improper header version\n");
		free(header);
		return -1;
	}

	struct stat dbstat = {0};
	fstat(fd, &dbstat);
	if (header->filesize != dbstat.st_size) {
		printf("Corrupted database\n");
		free(header);
		return -1;
	}

	return 0;
};

int create_db_header(int fd, struct dbheader_t **headerOut) {
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == -1) {
		printf("Malloc failed to create dbheader\n");
		return STATUS_ERROR;
	}

	header->version = 0x1;
	header->count = 0;
	header->magic = HEADER_MAGIC;
	header->filesize = sizeof(struct dbheader_t);

	*headerOut = header;

	return STATUS_SUCCESS;
};
int read_employees(int fd, struct dbheader_t *, struct employee_t **employeesOut) {
	return 0;
};
