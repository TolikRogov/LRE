#ifndef LRE_ARCHIVER_INCLUDE
#define LRE_ARCHIVER_INCLUDE

#define LRE_CHECK_ERROR()																	 \
	if (errno != LRE_NO_ERROR) {															\
		fprintf(stderr, "\n STATUS CODE ERROR: %d, File: %s, Line: %d, Function: %s\n\n", 	\
				errno, __FILE__, __LINE__, __PRETTY_FUNCTION__);							\
		return (StatusCode)errno;															\
	}

enum StatusCode {
	LRE_NO_ERROR,
	LRE_ALLOC_ERROR,
	LRE_FILE_ERROR
};

struct StringArray {
	size_t size;
	char* buffer;
	size_t count;
	size_t* addr;
	int archived;
};

StatusCode NewLineCheck(FILE* file, size_t* cur_str, size_t** exp_str);
StatusCode BufferUnarchiver(StringArray* array);
StatusCode AddrFill(StringArray* array);
StatusCode Fprint(FILE* file, size_t cnt, char el);
StatusCode FileSize(FILE* file, size_t* size);
StatusCode BufferArchiver(StringArray* array);
StatusCode BufferFill(StringArray* array);
StatusCode StringArrayDestruct(StringArray* array);

#endif //LRE_ARCHIVER_INCLUDE
