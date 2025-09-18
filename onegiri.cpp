#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "onegiri.h"
#include "onegiri_helpers.h"

void print_part_of_sorted_array(ptr_line lines[]) {
    for (size_t i = 0; i < 10; i++) { // print part of sorted array
        printf("(%s)\n", lines[i].ptr_line);
    }
}

int main() {
    srand(time(NULL));
    string src = "Пушкин Александр. Евгений Онегин - royallib.ru.txt";
    string dst = "онегин.txt";
    init_onegin_file(src, dst, "     ");

    char *onegin_buffer = init_and_read_into_onegin_buffer(dst);
    size_t num_of_lines = get_num_of_lines(onegin_buffer);

    ptr_line lines[num_of_lines] = {};
    init_lines(lines, onegin_buffer, num_of_lines);
    
    print_part_of_sorted_array(lines);
    putchar('\n');

    bubble_sort(lines, compare_normal, num_of_lines); // sort py alphabetic
    print_part_of_sorted_array(lines);
    putchar('\n');

    bubble_sort(lines, compare_reverse, num_of_lines); // sort by reverse alphabetic
    print_part_of_sorted_array(lines);
    putchar('\n');

    bubble_sort(lines, compare_reverse_transcription, num_of_lines); // sort by reverse transcription
    size_t random_strofa_indexes[LINES_IN_STROFA] = {};
    fill_strofa_with_random_indexes(lines, random_strofa_indexes, num_of_lines);
    
    for (size_t i = 0; i < 14; i++) {
        printf("%s, %s\n", lines[random_strofa_indexes[i]].ptr_last_word_transcription, lines[random_strofa_indexes[i]].ptr_line);
    }
    return 0;
}