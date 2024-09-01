#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/LRE.hpp"

// READ:
//	1) variadic functions, GENERIC
//	3) in/out stream destination

int main() {

	errno = 0;

	StringArray array = {};

	BufferArchiver(&array);
	LRE_CHECK_ERROR();

	BufferUnarchiver(&array);
	LRE_CHECK_ERROR();

	StringArrayDestruct(&array);
	LRE_CHECK_ERROR();

	return 0;
}
