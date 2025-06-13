#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>

#define LIB_PATH "./libfibonacci.so"
#define TEST_COUNT 10
#define FIB_NUM 40

typedef unsigned long long (*fib_func)(int);

void test_lib(void *handle, const char *func_name) {
	fib_func fib = (fib_func)dlsym(handle, func_name);
	if (!fib) {
		fprintf(stderr, "Error: %s\n", dlerror());
		return;
	}

	clock_t start = clock();
	for (int i = 0; i < TEST_COUNT; i++) {
		fib(FIB_NUM);
	}
	clock_t end = clock();
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%s average time: %.7f seconds\n", func_name, time_spent / TEST_COUNT);
}

int main() {
	void *handle = dlopen(LIB_PATH, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "Error: %s\n", dlerror());
		return 1;
	}

	printf("Testing Fibonacci(%d) %d times with dlopen\n", FIB_NUM, TEST_COUNT);
	test_lib(handle, "fibonacci_iterative");
	test_lib(handle, "fibonacci_recursive");

	dlclose(handle);
	return 0;
}
