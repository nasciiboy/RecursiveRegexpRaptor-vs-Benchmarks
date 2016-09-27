#include <stdio.h>

#include "re2/c_helper.h"
#include "main.h"

void re2_find_all(char* pattern, char* subject, int subject_len, int repeat)
{
	void* obj = get_re2_object(pattern);
	TIME_TYPE start, end, resolution;
	int found, time, best_time = 0;

	GET_TIME_RESOLUTION(resolution);

	if (!obj) {
		printf("RE2 compilation failed\n");
		return;
	}

	do {
		GET_TIME(start);
		found = search_all_re2(obj, subject, subject_len);
		GET_TIME(end);
		time = TIME_DIFF_IN_MS(start, end, resolution);
		if (!best_time || time < best_time)
			best_time = time;
	} while (--repeat > 0);
	printResult("re2", best_time, found);

	free_re2_object(obj);
}
