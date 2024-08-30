#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/LRE_archiver.hpp"

// READ:
//	1) variadic functions, GENERIC
//	3) in/out stream destination

int main() {

	StatusCode status = LRE_NO_ERROR;

	StringArray array = {};

	status = BufferArchiver(&array);
	LRE_CHECK_ERROR(status);

	status = BufferUnarchiver(&array);
	LRE_CHECK_ERROR(status);

	status = StringArrayDestruct(&array);
	LRE_CHECK_ERROR(status);

	return 0;
}
