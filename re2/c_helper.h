#ifdef __cplusplus
extern "C" {
#endif

void* get_re2_object(char* pattern);
void free_re2_object(void* obj);

int search_all_re2(void* obj, char* subject, int subject_len);

#ifdef __cplusplus
}
#endif
