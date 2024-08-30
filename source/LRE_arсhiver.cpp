#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/LRE_archiver.hpp"

StatusCode BufferUnarchiver(StringArray* array) {

	LRE_CHECK_ERROR(BufferFill(array));

	FILE* out = fopen("data/unarchived.txt", "w");
	if (!out)
		return LRE_FILE_ERROR;

	size_t cnt = 0;
	size_t* cur_str = array->addr;
	for (size_t i = 0; i < array->size; i++) {

		char* ch = (array->buffer + i);

		if (*cur_str == i + 1) {
			fprintf(out, "\n");
			cur_str++;
		}

		if (isdigit(*ch))
			cnt = cnt * 10 + (size_t)(*ch - '0');
		else {
			if (!cnt)
				fprintf(out, "%c", *ch);
			else {
				for (size_t j = 0; j < cnt; j++)
					fprintf(out, "%c", *ch);
				cnt = 0;
			}
		}
	}

	fclose(out);

	printf("\n Unarchiving complete \n\n");

	return LRE_NO_ERROR;
}

StatusCode BufferArchiver(StringArray* array) {

	LRE_CHECK_ERROR(BufferFill(array));

	array->archived = 1;

	FILE* out = fopen("data/archived.txt", "w");
	if (!out)
		return LRE_FILE_ERROR;

	char prev = *array->buffer;
	size_t cnt = 1;
	for (size_t i = 1; i < array->size; i++){

		if(*(array->buffer + i) == '\0')
			continue;

		if (prev == *(array->buffer + i))
			cnt++;
		else {
			Fprint(out, cnt, prev);
			prev = *(array->buffer + i);
			cnt = 1;
		}
	}
	Fprint(out, cnt, prev);

	fclose(out);

	printf("\n Archiving complete \n\n");

	printf("%zu \n", array->count);
	for (size_t i = 0; i < array->count; i++) {
		printf("%zu ", *(array->addr + i));
	}

	return LRE_NO_ERROR;
}

StatusCode BufferFill(StringArray* array) {

	FILE* in;

	if (!array->archived)
		in = fopen("data/input.txt", "r");
	else {
		in = fopen("data/archived.txt", "r");
		free(array->buffer);
		array->buffer = NULL;
	}

	if (!in)
		return LRE_FILE_ERROR;

	FileSize(in, array);

	array->buffer = (char*)calloc(1, array->size);
	if (!array->buffer)
		return LRE_ALLOC_ERROR;

	fread(array->buffer, sizeof(char), array->size, in);

	fclose(in);

	if (!array->archived)
		AddrFill(array);

	printf("%p \n", array->addr);
	printf("%zu \n", array->count);
	for (size_t i = 0; i < array->count; i++) {
		printf("%zu %zu \n", *(array->addr + i), i);
	}

	return LRE_NO_ERROR;
}

StatusCode AddrFill(StringArray* array) {

	for (size_t i = 0; i < array->size; i++) {
		if (*(array->buffer + i) == '\n') {
			*(array->buffer + i) = '\0';
			array->count++;
		}
	}

	array->addr = (size_t*)calloc(array->count, sizeof(size_t));
	if (!array->addr)
		return LRE_ALLOC_ERROR;

	printf("%p \n", array->addr);
	size_t cnt = 0;
	for (size_t i = 0; i < array->size; i++) {
		cnt++;
		if (*(array->buffer + i) == '\0') {
			*(array->addr + i) = --cnt;
			printf("%zu ", *(array->addr + i));
			cnt = 0;
		}
	}

	return LRE_NO_ERROR;
}

inline StatusCode Fprint(FILE* file, size_t cnt, char el) {
	if (cnt > 1)
		fprintf(file, "%zu%c", cnt, el);
	else
		fprintf(file, "%c", el);

	return LRE_NO_ERROR;
}

inline StatusCode FileSize(FILE* file, StringArray* array) {

	fseek(file, 0, SEEK_END);
	array->size = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	return LRE_NO_ERROR;
}

StatusCode StringArrayDestruct(StringArray* array) {

	array->count = 0;

	free(array->buffer);
	array->buffer = NULL;

	free(array->addr);
	array->addr = NULL;

	return LRE_NO_ERROR;
}
