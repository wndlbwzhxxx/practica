#include "fibonacci.h"

unsigned long long fibonacci_recursive(int n) {
	if (n <= 0) return 0;
	if (n == 1) return 1;
	return  fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}
