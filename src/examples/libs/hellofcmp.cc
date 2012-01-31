#include <stdio.h> // for printf(3)
#include <fcmp.h> // for fcmp(3)

/*
 * This is an example showing how to use the libfcmp for floating point comparison.
 *
 * 	Mark Veltzer
 *
 * EXTRA_LIBS=-lfcmp
 */
int main(int argc, char **argv) {
	printf("first result is %d\n", fcmp(0.5, 0.7, 0.1));
	printf("second result is %d\n", fcmp(0.5, 0.500000001, 0.2));
	return(0);
}