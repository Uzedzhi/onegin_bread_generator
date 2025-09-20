#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "onegiri.h"
#include "onegiri_helpers.h"

const int num_of_existing_flags = 2;
string existing_flags[] = {"-file", "-help"};
enum flags {
    FILE_NAME_FLAG, HELP_FLAG
};

void print_help() {
    printf("type flag --file <your_file_name> <[your_format_string]>\n"
           "to initialize file with your own!. Example: init --flag pushkin.txt [\\t\\t]\n"
           "type flag --help to print this information");
}

void exclude_explanations_and_non_alpha(char str[]) {
    sassert(str != NULL, ERR_PTR_IS_NULL);

    char * ptr_to_str = str;
    char * ptr_to_str_ahead = str; 
    bool is_in_explanation = 0;

    while (*ptr_to_str_ahead != '\n' && *ptr_to_str_ahead != '\0') {
        if (*ptr_to_str_ahead == '[' || *ptr_to_str_ahead == ']')
            is_in_explanation = !is_in_explanation;
        if ((isalpha(*ptr_to_str_ahead) || is_alpha_russian(*ptr_to_str_ahead) || isspace(*ptr_to_str_ahead)) && !is_in_explanation) {
            *ptr_to_str++ = *ptr_to_str_ahead;
        }
        ptr_to_str_ahead++;
    }
    *ptr_to_str = '\0';
}

void nullify_anything_extra(char * onegin_buffer, size_t file_size, size_t actually_read) { // todo ass
    sassert(onegin_buffer != NULL, ERR_PTR_IS_NULL);

    while (actually_read < file_size) {
        onegin_buffer[++actually_read] = '\0';
    }
}

void normalize_format_string(char * format_string) {
    sassert(format_string != NULL, ERR_PTR_IS_NULL);

    char * ptr_format_string = format_string;
    char * ptr_format_string_ahead = format_string;

    while (*ptr_format_string_ahead != '\0') {
        if (*ptr_format_string_ahead == '-')
                *ptr_format_string = ' ';
        else if (*ptr_format_string_ahead == '\\') {
                ptr_format_string_ahead++;
                if (*ptr_format_string_ahead == 'n') 
                    *ptr_format_string = '\n';
                else if (*ptr_format_string_ahead == 't')
                    *ptr_format_string = '\t';
        }

        ptr_format_string++;
        ptr_format_string_ahead++;
    }

    *(ptr_format_string - 1) = ']';
    *ptr_format_string = '\0';
}

void init_onegin_file(const char * file_name_src, const char * file_name_dst, char * format_string) {
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

    normalize_format_string(format_string);
    size_t strlen_format_string = strlen(format_string) - 2; // -2 because user format string is in [] and we ignore them

    char *next_new_line = NULL;
    size_t num_strofa = 1;

    
    while ((next_new_line = strchr(onegin_buffer, '\n')) != NULL) {
        *next_new_line++ = '\0';

        if (strncmp(onegin_buffer, format_string + 1, strlen_format_string - 1) != 0) {
            num_strofa = 1;
            onegin_buffer = next_new_line;
            continue;
        }

        exclude_explanations_and_non_alpha(onegin_buffer);

        if (onegin_buffer[0] == '\0' || onegin_buffer[0] == '\n' || onegin_buffer[strlen_format_string] == '\0' || onegin_buffer[strlen_format_string] == '\n') {// if new_line
            num_strofa = 1;
            onegin_buffer = next_new_line;
            continue;
        }

        fprintf(fpwrite, "%-3d, (%s)\n", num_strofa++, onegin_buffer + strlen_format_string);
        onegin_buffer = next_new_line;
    }
    free(start_of_onegin_buffer);
    fclose(fpwrite);
}

void execute_flag(int num_of_flag, int num_of_current_flag, int argc, char *argv[]) {
    sassert(argv != NULL, ERR_PTR_IS_NULL);

    if (num_of_flag == FILE_NAME_FLAG) {
        if (num_of_current_flag + 2 >= argc)
            push_error(ERROR_CODE, "you need to specify name of your file and format string after --file flag\n"
                                    "type --help for more information");
        const char * user_file_name = argv[num_of_current_flag + 1];
        char * format_string = argv[num_of_current_flag + 2];
        if (format_string[0] != '[' || format_string[strlen(format_string) - 1] != ']')
            push_error(ERROR_CODE, "format string should be in []");
        if (is_file_exists(user_file_name) == 0)
            push_error(ERROR_CODE, "file doesn't exist");
            
        init_onegin_file(user_file_name, "онегин.txt", format_string);
    }
    else if (num_of_flag == HELP_FLAG)
        print_help();
}

void execute_flags(int argc, char * argv[]) {
    int num_of_current_flag = 1;
    while (num_of_current_flag < argc) {
        char * current_flag = argv[num_of_current_flag];
        if (current_flag[0] == '-') {
            for (int i = 0; i < num_of_existing_flags; i++) {
                if (strcmp(current_flag + 1, existing_flags[i]) == 0) {
                    execute_flag(i, num_of_current_flag, argc, argv);
                    break;
                }
            }
        }
        num_of_current_flag++;
    }
}

int main(int argc, char *argv[]) {
    char src[] = "оригинальный-онегин.txt";
    char dst[] = "онегин.txt";
    char format_string[] = "[\t\t]";


    if (argc > 1)
        execute_flags(argc, argv);
    else // init with default name_file
        init_onegin_file(src, dst, format_string);
    return 0;
}