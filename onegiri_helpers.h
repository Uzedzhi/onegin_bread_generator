#ifndef HELPERS
#define HELPERS


#define get_close_index(range) fix_out_of_bounds(rand_numbers[num_of_rhyme] + range, (range > 0) ? range : -range, num_of_lines);
#define swap(first, second)    \
    ptr_line temp = first;  \
    first = second;     \
    second = temp;         \

// transcription
inline int is_glasnaya(char ch);
inline char changed_yoti(char ch);
inline int is_yoti(char ch);
inline char lower(char ch);
int is_alpha_russian(char ch);
char * get_last_word_transcription(size_t strlen_str, const char * str);

// initialize lines array
size_t get_num_of_lines(char * onegin_buffer);
int *init_lines_and_get_start_for_each_rhythm_type(ptr_line lines[], char * onegin_buffer, size_t num_of_lines);
char * init_and_read_into_onegin_buffer(const char * file_name_src);

// comparators and sort
int compare_normal(const void * first, const void * second);
int compare_reverse(const void * first, const void * second);
int compare_reverse_transcription(const void * first, const void * second);
int compare_by_rhythm_type(const void * first, const void * second);
void bubble_sort(ptr_line * lines, int(* func)(const void *, const void *), size_t num_of_lines);

// random strofa generator
void fill_with_different_rand_numbers(int rand_numbers[], int num_of_lines);
void fill_strofa_with_random_indexes(ptr_line * lines, size_t *final_random_indexes, int * count_for_each_rhythm_type, int num_of_lines);
void place_final_random_indexes(size_t final_random_indexes[], const int onegin_strofa_structure[],
                                size_t num_of_rhyme, int rhyme_index, int close_rhyme_index);
int fix_out_of_bounds(int element, int x, int num_of_lines);
size_t get_num_of_equal_symbols_from_back(char *str1, char *str2);

// print funcs
void print_sorted_array(ptr_line lines[], FILE * fp, size_t num_of_lines);
void print_unsorted_array(char * onegin_buffer, FILE * fp, size_t num_of_lines);
void print_my_strofa(ptr_line lines[], size_t random_strofa_indexes[], FILE *fp);

// onegiri_initialize_file.cpp
size_t get_file_size(FILE * fp);
int is_file_exists(const char * file_name);
void exclude_explanations_and_non_alpha(char str[]);
void nullify_anything_extra(char * onegin_buffer, size_t file_size, size_t actually_read);
void init_onegin_file(const char * file_name_src, const char * file_name_dst, char * format_string);


#endif // HELPERS_H