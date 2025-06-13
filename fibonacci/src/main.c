#include <stdio.h>
#include "fibonacci.h"

int main() {
	int n;
	printf("Enter a number: ");
	scanf("%d", &n);

	printf("Iterative Fibonacci(%d) = %llu\n", n, fibonacci_iterative(n));
	printf("Recursive Fibonacci(%d) = %llu\n", n, fibonacci_recursive(n));

	return 0;
}
