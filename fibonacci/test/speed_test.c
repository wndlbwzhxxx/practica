#include <stdio.h>
#include <time.h>
#include "fibonacci.h"

#define TEST_COUNT 10
#define FIB_NUM 40

void test_iterative() {
	clock_t start = clock();
	for (int i = 0; i < TEST_COUNT; i++) {
		fibonacci_iterative(FIB_NUM);
	}
	clock_t end = clock();
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Iterative version average time: %.7f seconds\n", time_spent / TEST_COUNT);
}

void  test_recursive() {
	clock_t start = clock();
	for (int i = 0; i < TEST_COUNT; i++) {
		fibonacci_recursive(FIB_NUM);
	}
	clock_t end = clock();
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Recursive version average time: %.7f seconds\n", time_spent / TEST_COUNT);
}

int main() {
	printf("Testing Fibonacci(%d) %d times\n", FIB_NUM, TEST_COUNT);
	test_iterative();
	test_recursive();
	return 0;
}
