#include "re2/re2.h"
#include "re2/stringpiece.h"
#include "c_helper.h"

#include <stdio.h>
#include <string.h>

void* get_re2_object(char* pattern)
{
	RE2* obj;

	obj = new RE2(pattern, RE2::Latin1);
	if (!obj->ok()) {
		printf("pattern error!\n");
		delete obj;
		obj = NULL;
	}
	return obj;
}

void free_re2_object(void* obj)
{
	delete (RE2*)obj;
}

int search_all_re2(void* obj, char* subject, int subject_len)
{
	re2::StringPiece input(subject, subject_len);
	//re2::StringPiece result;
	int found = 0;

	while (RE2::FindAndConsume(&input, *(RE2*)obj)) {
		// printf("match: %d %d @%d\n", result.data() - subject, result.size(), input.data() - subject);
		found++;
	}
	return found;
}

namespace re2 {

int RunningOnValgrind()
{
	return 0;
}

};
