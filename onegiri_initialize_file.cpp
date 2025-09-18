#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "onegiri.h"
#include "onegiri_helpers.h"


void exclude_explanations_and_non_alpha(char str[]) {
    sassert(str != NULL, ERR_PTR_IS_NULL);

    char * ptr_to_str = str;
    char * ptr_to_str_ahead = str; 

    while (*ptr_to_str_ahead != '\n' && *ptr_to_str_ahead != '\0') {
        if (isalpha(*ptr_to_str_ahead) || is_alpha_russian(*ptr_to_str_ahead) || isspace(*ptr_to_str_ahead)) {
            *ptr_to_str++ = *ptr_to_str_ahead;
        }
        ptr_to_str_ahead++;
    }
}

void nullify_anything_extra(char * onegin_buffer, size_t file_size, size_t actually_read) { // todo ass
    sassert(onegin_buffer != NULL, ERR_PTR_IS_NULL);

    while (actually_read < file_size) {
        onegin_buffer[++actually_read] = '\0';
    }
}

void init_onegin_file(const char * file_name_src, const char * file_name_dst, const char * format_string) {
    sassert(file_name_src != NULL, ERR_PTR_IS_NULL);
    sassert(file_name_dst != NULL, ERR_PTR_IS_NULL);
    sassert(format_string != NULL, ERR_PTR_IS_NULL);

    FILE *fp = fopen(file_name_src, "r");
    FILE *fpwrite = fopen(file_name_dst, "w"); // opening files
    sassert(fp != NULL, ERR_PTR_IS_NULL);
    sassert(fpwrite != NULL, ERR_PTR_IS_NULL);

    size_t file_size = get_file_size(fp);
    char * onegin_buffer = (char *) calloc(file_size + 1, sizeof(char));
    char * start_of_onegin_buffer = onegin_buffer;
    size_t actually_read = fread(onegin_buffer, sizeof(char), file_size, fp);
    fclose(fp);

    nullify_anything_extra(onegin_buffer, file_size, actually_read);

    size_t strlen_format_string = strlen(format_string);
    char *next_new_line = NULL;
    size_t num_strofa = 1;

    
    while ((next_new_line = strchr(onegin_buffer, '\n')) != NULL) {
        *next_new_line++ = '\0';
        if (onegin_buffer[0] == '\0' || onegin_buffer[0] == '\n' || onegin_buffer[strlen_format_string] == '\0' || onegin_buffer[strlen_format_string] == '\n') {// if new_line
            num_strofa = 1;
            onegin_buffer = next_new_line;
            continue;
        }

        if (strncmp(onegin_buffer, format_string, strlen_format_string) != 0) {
            num_strofa = 1;
            onegin_buffer = next_new_line;
            continue;
        }

        exclude_explanations_and_non_alpha(onegin_buffer);

        fprintf(fpwrite, "%-3d, (%s)\n", num_strofa++, onegin_buffer + strlen_format_string);
        onegin_buffer = next_new_line;
    }
    free(start_of_onegin_buffer);
    fclose(fpwrite);
}

int main(void) {
    string src = "Пушкин Александр. Евгений Онегин - royallib.ru.txt";
    string dst = "онегин.txt";
    init_onegin_file(src, dst, "\t\t");
    return 0;
}