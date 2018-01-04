#include "duhttp.h"
#include "stdio.h"

void DuHttp_ELOG(const char* str) {
	fprintf(stderr, "%s\n", str); // or just ignore it!
}