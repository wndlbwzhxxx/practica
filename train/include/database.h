#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>
#include <time.h>

#define SIGNATURE "KOST"
#define MAX_DESTINATION 50

typedef struct {
	char destination[MAX_DESTINATION];
	int train_number;
	time_t departure_time;
} TRAIN;

typedef struct {
	char signature[4];
	uint32_t transaction;
	uint32_t count;
	uint32_t checksum;
} DatabaseHeader;

uint32_t calculate_checksum(const void *data, size_t length);

#endif //DATABASE_H
