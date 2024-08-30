#ifndef LRE_ARCHIVER_INCLUDE
#define LRE_ARCHIVER_INCLUDE

#define LRE_CHECK_ERROR(error)																							 \
	if (error != LRE_NO_ERROR) {																						\
		fprintf(stderr, "\n STATUS CODE ERROR: %d, Function: %s, Line: %d\n\n", error, __PRETTY_FUNCTION__, __LINE__);	\
		return error;																									\
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
	char** addr;
	int archived;
};

StatusCode BufferUnarchiver(StringArray* array);
StatusCode AddrFill(StringArray* array);
inline StatusCode Fprint(FILE* file, size_t cnt, char el);
inline StatusCode FileSize(FILE* file, StringArray* array);
StatusCode BufferArchiver(StringArray* array);
StatusCode BufferFill(StringArray* array);
StatusCode StringArrayDestruct(StringArray* array);

#endif //LRE_ARCHIVER_INCLUDE
