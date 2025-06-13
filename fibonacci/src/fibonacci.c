#include "fibonacci.h"


unsigned long long fibonacci_iterative(int n) {
	if (n <= 0) return 0;
	if (n == 1) return 1;

	unsigned long long a = 0, b = 1, c;
	for (int i = 2; i <= n; i++) {
		c = a + b;
		a = b;
		b = c;
	}
	return b;
}
