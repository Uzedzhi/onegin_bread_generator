#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "onegiri.h"
#include "onegiri_helpers.h"

int main() {
    srand(time(NULL));
    string src = "Пушкин Александр. Евгений Онегин - royallib.ru.txt";
    string dst = "онегин.txt";
    string file = "file.txt";
    FILE * fp = fopen(file, "w");

    if (!is_file_exists(dst)) {
        fprintf(stderr, "please reinitialize poen\n"
                        "with onegiri_initialize_file.exe program");
        return ERROR_CODE;
    }
    
    char *onegin_buffer = init_and_read_into_onegin_buffer(dst);

    size_t num_of_lines = get_num_of_lines(onegin_buffer);
    ptr_line lines[num_of_lines] = {};
    init_lines(lines, onegin_buffer, num_of_lines);


    qsort(lines, num_of_lines, sizeof(ptr_line), compare_normal);
    // bubble_sort(lines, compare_normal, num_of_lines); // sort py alphabetic
    print_part_of_sorted_array(lines, fp, num_of_lines);
    putc('\n', fp);

    bubble_sort(lines, compare_reverse, num_of_lines); // sort by reverse alphabetic
    print_part_of_sorted_array(lines, fp, num_of_lines);
    putc('\n', fp);

    print_unsorted_array(onegin_buffer, fp, num_of_lines);
    putc('\n', fp);


    qsort(lines, num_of_lines, sizeof(ptr_line), compare_reverse_transcription);
    size_t random_strofa_indexes[LINES_IN_STROFA] = {};
    fill_strofa_with_random_indexes(lines, random_strofa_indexes, num_of_lines);
    
    for (size_t i = 0; i < 14; i++) {
        fprintf(fp, "%s, %s\n", lines[random_strofa_indexes[i]].ptr_last_word_transcription, lines[random_strofa_indexes[i]].ptr_line);
    }
    return 0;
}