#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "onegiri.h"
#include "onegiri_helpers.h"

inline int is_glasnaya(char ch) {
    return strchr("àîåóÿþý", ch) != NULL;
}

inline int is_yoti(char ch) {
    return strchr("åþÿ",     ch) != NULL;
}

inline char changed_yoti(char ch) {
    switch(ch) {
        case 'å':
            return 'ý';
        case 'þ':
            return 'ó';
        case 'ÿ':
            return 'à';
        default:
            return ch;
    }
}

inline char lower(char ch) {
    if (ch >= 'À' && ch <= 'ß')
        return ch + ' ';
    return ch;
}

int is_alpha_russian(char ch) {
    return ch >= 'À' && ch <= 'ÿ';
}

int is_punctuation(char ch) {
    return (ch >= 33 && ch <= 47) || ch == '–' || ch == '«' || ch == '»' || ch == '…' || ch == ':' || ch == ';';
}
void strip_right(char * str, size_t strlen_str) {
    sassert(str != NULL, ERR_PTR_IS_NULL);

    while (!is_alpha_russian(str[strlen_str]))
        strlen_str--;
    str[strlen_str + 1] = '\0';
}

char * get_last_word_transcription(size_t strlen_str, const char * str) {
   sassert(str != NULL, ERR_PTR_IS_NULL);

    while (is_alpha_russian(str[--strlen_str]) == 0)
        ;
    size_t word_length = 1;
    while (is_alpha_russian(str[--strlen_str]))
        word_length++;
    strlen_str++;
    char * last_word_transcription = (char *) calloc(MAX_SIZE, sizeof(char));

    last_word_transcription[0] = '[';
    size_t transctiption_symbols = 1;
    for (size_t i = 0; i < word_length; i++) {
        char ch_before = lower(str[strlen_str + i - 1]);
        char ch_now    = lower(str[strlen_str + i]);
        char ch_after  = lower(str[strlen_str + i + 1]);
        if (i == word_length - 1)
            ch_after = ch_now;
        if (i == 0)
            ch_before = ch_now;
        if (is_yoti(ch_now) && (ch_before == 'ü' || ch_before == 'ú')) {
            last_word_transcription[transctiption_symbols++] = 'é';
            last_word_transcription[transctiption_symbols++] = changed_yoti(ch_now);
        }
        else if (ch_now == 'ü' || ch_now == 'ú') {
            continue;
        }
        else if (!is_glasnaya(ch_now) && ch_now == ch_after) {
            last_word_transcription[transctiption_symbols++] = ch_now;
            i++;
        }
        else if(i == word_length - 1 && strchr(pairs[1], ch_now) != NULL) {
            last_word_transcription[transctiption_symbols++] = pairs[0][strchr(pairs[1], ch_now) - pairs[1]];
        }
        else if(strchr(pairs[0], ch_now) != NULL && strchr(pairs[1], ch_after) != NULL) {
            if (ch_now != 'ò' && ch_after != 'â')
                last_word_transcription[transctiption_symbols++] = pairs[1][strchr(pairs[0], ch_now) - pairs[0]];
            else
                last_word_transcription[transctiption_symbols++] = ch_now;
        }
        else if (!is_glasnaya(ch_now) && (is_yoti(ch_after) || ch_after == 'è' || ch_after == 'ü')) {
            last_word_transcription[transctiption_symbols++] = ch_now;
            last_word_transcription[transctiption_symbols++] = '\'';
        }
        else if (is_yoti(ch_now) && is_glasnaya(ch_before)) {
            last_word_transcription[transctiption_symbols++] = 'é';
            last_word_transcription[transctiption_symbols++] = changed_yoti(ch_now);
        }
        else {
            last_word_transcription[transctiption_symbols++] = ch_now;
        }
            
    }
    last_word_transcription[transctiption_symbols] = ']';
    return last_word_transcription;
    
}

size_t get_num_of_lines(char * onegin_buffer) {
    sassert(onegin_buffer != NULL, ERR_PTR_IS_NULL);

    size_t count = -1;
    while ((onegin_buffer = strchr(onegin_buffer, '\n')) != NULL) {
        *onegin_buffer++ = '\0';
        count++;
    }

    return count;
}

int init_lines(ptr_line lines[], char * onegin_buffer, size_t num_of_lines) {
    sassert(onegin_buffer != NULL, ERR_PTR_IS_NULL);

    size_t i = 0;
    char * next_line = strchr(onegin_buffer, '\0') + 1;
    while (i < num_of_lines) {
        lines[i].ptr_line = onegin_buffer + 5; // +5 to skip numbers at start of string
        lines[i].len_line = next_line - onegin_buffer - 1; // -1 bcs next_line points to \0 + 1
        lines[i].ptr_last_word_transcription = get_last_word_transcription(lines[i].len_line + 1, onegin_buffer);
        lines[i].len_last_word_transcription = strlen(lines[i].ptr_last_word_transcription);
        int num_of_strofa = atoi(onegin_buffer) % 14;
        lines[i].is_nine_slogov = num_of_strofa % 3 == 0 || num_of_strofa == 1 || num_of_strofa == 5; 

        onegin_buffer = next_line;
        next_line = strchr(onegin_buffer, '\0') + 1;
        ++i;
    }

    return 0;
}

char * init_and_read_into_onegin_buffer(const char * file_name_src) {
    sassert(file_name_src != NULL, ERR_PTR_IS_NULL);

    FILE * fp = fopen(file_name_src, "r");
    sassert(fp != NULL, ERR_PTR_IS_NULL);

    size_t file_size = get_file_size(fp);
    char * buffer = (char *) calloc(file_size + 1, sizeof(char));
    sassert(buffer != NULL, ERR_PTR_IS_NULL);

    size_t actually_read = fread(buffer, sizeof(char), file_size, fp);
    while (actually_read < file_size) {
        buffer[actually_read++] = '\0';
    }

    fclose(fp);

    return buffer;
}


size_t get_file_size(FILE * fp) {
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);


    return file_size;
}

size_t get_num_of_equal_symbols_from_back(char *str1, char *str2) {
    sassert(str1 != NULL && str2 != NULL, ERR_PTR_IS_NULL);

    int count = 0;
    int strlen_str1 = strlen(str1), strlen_str2 = strlen(str2);

    while (strlen_str1 >= 0 && strlen_str2 >= 0) {
        if (str1[strlen_str1] != str2[strlen_str2])
            break;
        strlen_str1--;
        strlen_str2--;
        count++;
    }
    return count;
}

int compare_normal(const void * first, const void * second) {
    sassert(first != NULL && second != NULL, ERR_PTR_IS_NULL);

    
    char * str1 = ((ptr_line *)first)->ptr_line;
    char * str2 = ((ptr_line *)second)->ptr_line;
    size_t index_str1 = 0;
    size_t index_str2 = 0;

    while (str1[index_str1] != 0 && str2[index_str2] != 0) {
        if (str1[index_str1] != str2[index_str2])
            break;
        index_str1++;
        index_str2++;
    }

    return str1[index_str1] - str2[index_str2];
        
}

int compare_reverse_transcription(const void * first, const void * second) { // void comparator
    sassert(first != NULL && second != NULL, ERR_PTR_IS_NULL);

    char * str1 = ((ptr_line *)first)->ptr_last_word_transcription;
    char * str2 = ((ptr_line *)second)->ptr_last_word_transcription;
    size_t strlen_str1 = ((ptr_line *)first)->len_last_word_transcription;
    size_t strlen_str2 = ((ptr_line *)second)->len_last_word_transcription;

    while (strlen_str1 >= 0 && strlen_str2 >= 0) {
        if (str1[strlen_str1] != str2[strlen_str2])
            break;
        strlen_str1--;
        strlen_str2--;
    }

    return str1[strlen_str1] - str2[strlen_str2];
        
}

int compare_reverse(const void * first, const void * second) { // void comparator
    sassert(first  != NULL, ERR_PTR_IS_NULL)
    sassert(second != NULL, ERR_PTR_IS_NULL);

    size_t strlen_str1 = ((ptr_line *)first)->len_line - 1;
    size_t strlen_str2 = ((ptr_line *)second)->len_line - 1;
    char * str1 = ((ptr_line *)first)->ptr_line;
    char * str2 = ((ptr_line *)second)->ptr_line;

    while (strlen_str1 >= 0 && strlen_str2 >= 0) {
        if (str1[strlen_str1] != str2[strlen_str2])
            break;
        strlen_str1--;
        strlen_str2--;
    }

    return str1[strlen_str1] - str2[strlen_str2];
}

void print_part_of_sorted_array(ptr_line lines[], FILE * fp, size_t num_of_lines) {
    sassert(lines != NULL, ERR_PTR_IS_NULL);
    sassert(fp != NULL, ERR_PTR_IS_NULL);
    sassert(num_of_lines != NULL, ERR_PTR_IS_NULL);

    for (size_t i = 0; i < num_of_lines; i++) { // print part of sorted array
        fprintf(fp, "(%s)\n", lines[i].ptr_line);
    }
}

void print_unsorted_array(char * onegin_buffer, FILE * fp, size_t num_of_lines) {
    sassert(onegin_buffer != NULL, ERR_PTR_IS_NULL);
    sassert(fp != NULL, ERR_PTR_IS_NULL);
    sassert(num_of_lines != NULL, ERR_PTR_IS_NULL);

    for (size_t i = 0; i < 10; i++) { // print art of unsorted array
        fprintf(fp, "(%s)\n", onegin_buffer + 5); // +5 to skip numbers at the start of each string
        onegin_buffer = strchr(onegin_buffer, '\0') + 1;
    }
}

int is_file_exists(const char * file_name) {
    FILE * fp = fopen(file_name, "r");
    fclose(fp);
    return fp != NULL;
}

void bubble_sort(ptr_line * lines, int(* func)(const void *, const void *), size_t num_of_lines) {
    sassert(lines != NULL, ERR_PTR_IS_NULL);
    sassert(func != NULL, ERR_PTR_IS_NULL);

    for (size_t i = 0; i < num_of_lines - 1; i++) {
        for (size_t j = 0; j < num_of_lines - 1 - i; j++) {
            if (func(lines + j, lines + j + 1) > 0) {
                swap(lines[j], lines[j+1]);
            }
        }
    }
}

void fill_with_different_rand_numbers(int rand_numbers[], int num_of_lines) {
    sassert(rand_numbers != NULL, ERR_PTR_IS_NULL);

    for (int i = 0; i < LINES_IN_STROFA / 2; i++) { // how many different line rhymes in strofa
        while (true) {
            rand_numbers[i] = rand() % num_of_lines;
            int j = 0;
            while (j++ < i)
                if (abs(rand_numbers[j] - rand_numbers[i]) < MINIMUM_RHYME_DISTANCE)
                    break;
            
            if (j >= i)
                break;
        }
    }
}

int fix_out_of_bounds(int line_index, int x, int num_of_lines) {
    if (line_index < 0)
        return line_index + 2 * x;
    if (line_index >= num_of_lines)
        return line_index - 2 * x;
    return line_index;
}

void place_final_random_indexes(size_t final_random_indexes[], const int onegin_strofa_structure[],
                                size_t num_of_rhyme, int rhyme_index, int close_rhyme_index) {
    // placing first rhyme index
    size_t num_line = 0;
    while (num_line < 14) {
        if (onegin_strofa_structure[num_line] == num_of_rhyme) {
            final_random_indexes[num_line] = rhyme_index;
            break;
        }
        num_line++;
    }

    // placing close rhyme index
    while (++num_line < 14) {
        if (onegin_strofa_structure[num_line] == num_of_rhyme) {
            final_random_indexes[num_line] = close_rhyme_index;
            break;
        }
    }
}

void fill_strofa_with_random_indexes(ptr_line * lines, size_t *final_random_indexes, int num_of_lines) {
    sassert(lines != NULL && final_random_indexes != NULL, ERR_PTR_IS_NULL);

    const int onegin_strofa_structure[14] = {0, 1, 0, 1, 2, 2, 3, 3, 4, 5, 5, 4, 6, 6};
    int rand_numbers[LINES_IN_STROFA / 2] = {};

    fill_with_different_rand_numbers(rand_numbers, num_of_lines);

    for (size_t num_of_rhyme = 0; num_of_rhyme < LINES_IN_STROFA / 2; num_of_rhyme++) {
        size_t range = rand() % 3 + 1; // range from 1 to 3
        char * str_now   = lines[rand_numbers[num_of_rhyme]].ptr_last_word_transcription;

        size_t close_rhyme_index = get_close_index(range);
        char * str_close = lines[close_rhyme_index].ptr_last_word_transcription;

        size_t how_many_equal_from_behind = get_num_of_equal_symbols_from_back(str_now, str_close);
        // if bad rhyme => smaller range
        if (how_many_equal_from_behind <= MINIMUM_NUM_FOR_GOOD_RHYME) {
            range  = 1; // small range if rhyme is not so good
            close_rhyme_index = get_close_index(1);

        } // too good rhyme => bigger range
        else if (how_many_equal_from_behind >= MAXIMUM_NUM_FOR_GOOD_RHYME) {
            range += 3; // bigger range if words are practically same
            close_rhyme_index = get_close_index(range); 
        }
        place_final_random_indexes(final_random_indexes, onegin_strofa_structure, num_of_rhyme, rand_numbers[num_of_rhyme], close_rhyme_index);
    }
}