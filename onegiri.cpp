#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "onegiri.h"
#include "onegiri_helpers.h"

int main() {
    srand((unsigned int) time(NULL));
    string src = "Пушкин Александр. Евгений Онегин - royallib.ru.txt";
    string dst = "онегин.txt";
    string file = "file.txt";
    FILE * fp = fopen(file, "w");

    if (!is_file_exists(dst)) {
        fprintf(stderr, "please reinitialize poem\n"
                        "with onegiri_initialize_file.exe program");
        return ERROR_CODE;
    }
    
    char *onegin_buffer = init_and_read_into_onegin_buffer(dst);

    size_t num_of_lines = get_num_of_lines(onegin_buffer);
    ptr_line lines[num_of_lines] = {};
    int * start_for_each_rhythm_type = init_lines_and_get_start_for_each_rhythm_type(lines, onegin_buffer, num_of_lines);


    qsort(lines, num_of_lines, sizeof(ptr_line), compare_normal); // sort py alphabetic
    print_sorted_array(lines, fp, num_of_lines);
    putc('\n', fp);

    qsort(lines, num_of_lines, sizeof(ptr_line), compare_reverse); // sort by reverse alphabetic
    print_sorted_array(lines, fp, num_of_lines);
    putc('\n', fp);

    print_unsorted_array(onegin_buffer, fp, num_of_lines);
    putc('\n', fp);

    bubble_sort(lines, compare_reverse_transcription, num_of_lines);
    bubble_sort(lines, compare_by_rhythm_type, num_of_lines);

    print_sorted_array(lines, fp, num_of_lines);
    putc('\n', fp);
    for (int i = 0; i < 5; i++) {
        size_t random_strofa_indexes[LINES_IN_STROFA] = {};
        fill_strofa_with_random_indexes(lines, random_strofa_indexes, start_for_each_rhythm_type, num_of_lines);

        print_my_strofa(lines, random_strofa_indexes, fp);
    }
    return 0;
}