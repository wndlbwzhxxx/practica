#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <zlib.h>
#include "database.h"

#define FILENAME "trains.db"
#define MAX_TRAINS 100

// функция для отображения меню
void show_menu() {
	clear();
	printw("=== Train Database Managment ===\n");
	printw("1. Create new database\n");
	printw("2. Find trains after specified time\n");
	printw("3. Add new train record\n");
	printw("4. Exit\n");
	printw("Select option: ");
	refresh();
}

// функция для ввода строки
void input_string(const char* prompt, char* buffer, int max_len) {
	echo();
	printw("%s", prompt);
	refresh();
	getnstr(buffer, max_len - 1);
	noecho();
}

// функция для ввода целого числа через ncurses
int input_int(const char* prompt) {
	echo();
	printw("%s", prompt);
	refresh();
	char buffer[20];
	getnstr(buffer, sizeof(buffer) - 1);
	noecho();
	return atoi(buffer);
}

// функция для ввода даты и времени через ncurses
time_t input_datetime(const char* prompt) {
	echo();
	printw("%s (format: YYYY-MM-DD HH:MM): ", prompt);
	refresh();

	char buffer[20];
	getnstr(buffer, sizeof(buffer) - 1);
	noecho();

	struct tm tm = {0};
	if (strptime(buffer, "%Y-%m-%d %H:%M", &tm) == NULL) {
		return -1;
	}

	return mktime(&tm);
}

// функция для создания новой базы данных
void create_database() {
	int count = input_int("Enter number of trains: ");
	if (count <= 0 || count > MAX_TRAINS) {
		printw("Invalid number of trains (1-%d)\n", MAX_TRAINS);
		getch();
		return;
	}

	TRAIN trains[MAX_TRAINS];

	for (int i = 0; i < count; i++) {
		clear();
		printw("=== Entering train #%d ===\n", i + 1);

		input_string("Destination: ", trains[i].destination, MAX_DESTINATION);
		trains[i].train_number = input_int("Train number: ");
		trains[i].departure_time = input_datetime("Departure time");

		if (trains[i].departure_time == -1) {
			printw("Invalid date/time format!\n");
			getch();
			i--;
			continue;
		}
	}

	// создание заголовка
	DatabaseHeader header;
	memcpy(header.signature, SIGNATURE, 4);
	header.transaction = 1;
	header.count = count;
	header.checksum = calculate_checksum(trains, count * sizeof(TRAIN));

	// запись в файл
	FILE* file = fopen(FILENAME, "wb");
	if (!file) {
		printw("Ошибка создания файла");
		getch();
		return;
	}

	fwrite(&header, sizeof(DatabaseHeader), 1, file);
	fwrite(trains, sizeof(TRAIN), count, file);
	fclose(file);

	printw("Database created successfully with %d records!\n", count);
	getch();
}

// функция для чтения базы данных
int read_database(DatabaseHeader* header, TRAIN* trains, int max_trains) {
	printf("Открытие файла: %s\n", FILENAME);
	FILE* file = fopen(FILENAME, "rb");
	if (!file) {
		perror("Ошибка fopen");
		return -1;
	}

	// чтение заголовка
	if (fread(header, sizeof(DatabaseHeader), 1, file) != 1) {
		perror("Ошибка чтения заголовка");
		fclose(file);
		return -1;
	}

	// чтение данных
	if (fread(trains, sizeof(TRAIN), header->count, file) != header->count) {
		perror("Ошибка чтения данных");
		fclose(file);
		return -1;
	}

	// проверка контрольной суммы
	uint32_t checksum = calculate_checksum(trains, header->count * sizeof(TRAIN));
	if (checksum != header->checksum) {
		fclose(file);
		return -1;
	}

	fclose(file);
	return header->count;
}

// функция для добавления записи в базу данных
void add_record() {
	DatabaseHeader header;
	TRAIN trains[MAX_TRAINS];

	int count = read_database(&header, trains, MAX_TRAINS - 1);
	if (count < 0) {
		printw("Error reading database or database doesn't exist!\n");
		getch();
		return;
	}

	clear();
	printw("=== Adding new train record ===\n");

	input_string("Destination: ", trains[count].destination, MAX_DESTINATION);
	trains[count].train_number = input_int("Train number: ");
	trains[count].departure_time = input_datetime("Departure time");

	if (trains[count].departure_time == -1) {
		printw("Invalid date/time format!\n");
		getch();
		return;
	}

	header.count++;
	header.transaction++;
	header.checksum = calculate_checksum(trains, (count + 1) * sizeof(TRAIN));

	// запись в файл
	FILE* file = fopen(FILENAME, "r+b");
	if (!file) {
		printw("Error opening file for writing\n");
		getch();
		return;
	}

	fwrite(&header, sizeof(DatabaseHeader), 1, file);
	fwrite(trains, sizeof(TRAIN), count + 1, file);
	fclose(file);

	printw("Record added successfully! Total records: %d\n", count + 1);
	getch();
}

// функция для поиска поездов после указанного времени
void find_trains_after_time() {
	DatabaseHeader header;
	TRAIN trains[MAX_TRAINS];

	int count = read_database(&header, trains, MAX_TRAINS);
	if (count < 0) {
		printw("Error reading database or database doesn't exist!\n");
		getch();
		return;
	}

	clear();
	time_t search_time = input_datetime("Enter time to search after");
	if (search_time == -1) {
		printw("Invalid date/time format!\n");
		getch();
		return;
	}

	clear();
	printw("=== Trains departing after %s ===\n", ctime(&search_time));

	int found = 0;
	for (int i = 0; i < count; i++) {
		if (trains[i].departure_time > search_time) {
			printw("Train #%d: %s to %s, departs at %s",
				trains[i].train_number,
				trains[i].destination,
				ctime(&trains[i].departure_time));
			found++;
		}
	}

	if (!found) {
		printw("No trains found departing after specified time.\n");
	}

	getch();
}

// главная функция
int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int choice;
	do {
		show_menu();
		choice = input_int("");

		switch (choice) {
			case 1:
				create_database();
				break;
			case 2:
				find_trains_after_time();
				break;
			case 3:
				add_record();
				break;
			case 4:
				break;
			default:
				printw("Invalid choice! Press any key to continue...\n");
				getch();
		}
	} while (choice !=4);

	endwin();
	return 0;
}

// функция для расчета контрольной суммы
uint32_t calculate_checksum(const void *data, size_t length) {
	return crc32(0L, (const Bytef *)data, length);
}
