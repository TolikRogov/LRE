#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include "../include/LRE.hpp"

StatusCode BufferUnarchiver(StringArray* array) {

	BufferFill(array);
	LRE_CHECK_ERROR();

	FILE* out = fopen("data/unarchived.txt", "w");
	if (!out)
		return LRE_FILE_ERROR;

	size_t cnt = 0;
	size_t* exp_str_cnt = array->addr, cur_str_cnt = 0;
	for (size_t i = 0; i < array->size; i++) {

		char* ch = (array->buffer + i);

		if (isdigit(*ch))
			cnt = cnt * 10 + (size_t)(*ch - '0');
		else {
			size_t j = 0;
			do {
				fprintf(out, "%c", *ch);
				cur_str_cnt++;
				NewLineCheck(out, &cur_str_cnt, &exp_str_cnt);
				LRE_CHECK_ERROR();
				j++;
			} while (j < cnt);
			cnt = 0;
		}
	}

	fclose(out);

	printf(" Unarchiving complete \n\n");

	return LRE_NO_ERROR;
}

StatusCode BufferArchiver(StringArray* array) {

	BufferFill(array);
	LRE_CHECK_ERROR();

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
			LRE_CHECK_ERROR();

			prev = *(array->buffer + i);
			cnt = 1;
		}
	}
	Fprint(out, cnt, prev);
	LRE_CHECK_ERROR();

	fclose(out);

	printf(" Archiving complete \n\n");

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

	FileSize(in, &array->size);
	LRE_CHECK_ERROR();

	array->buffer = (char*)calloc(1, array->size);
	if (!array->buffer)
		return LRE_ALLOC_ERROR;

	fread(array->buffer, sizeof(char), array->size, in);

	fclose(in);

	if (!array->archived) {
		AddrFill(array);
		LRE_CHECK_ERROR();
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

	size_t cnt = 0, j = 0;
	for (size_t i = 0; i < array->size; i++) {
		cnt++;
		if (*(array->buffer + i) == '\0') {
			*(array->addr + j) = --cnt;
			j++;
			cnt = 0;
		}
	}
	printf("\n");

	return LRE_NO_ERROR;
}

StatusCode Fprint(FILE* file, size_t cnt, char el) {
	if (cnt > 1)
		fprintf(file, "%zu%c", cnt, el);
	else
		fprintf(file, "%c", el);

	return LRE_NO_ERROR;
}

StatusCode FileSize(FILE* file, size_t* size) {

	int cur_place = (int)ftell(file);
	fseek(file, 0, SEEK_END);
	*size = (size_t)ftell(file);
	fseek(file, 0, cur_place);

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

StatusCode NewLineCheck(FILE* file, size_t* cur_str_cnt, size_t** exp_str_cnt) {
	if (**exp_str_cnt == *cur_str_cnt) {
		fprintf(file, "\n");
		(*exp_str_cnt)++;
		*cur_str_cnt = 0;
	}
	return LRE_NO_ERROR;
}
