#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

const int MAX_SIZE = 200;
const int MAX_WORD_SIZE = 100;
const int ROWS = 8000;

char * skip_non_alpha(char * str) {
    int i = 0;
    while (str[i] != '\0' && str[i] != EOF && (str[i] < 'А' || str[i] > 'я'))
        i++;
    return str + i;
}

int is_alpha(char ch) {
    return (ch >= 'А' && ch <= 'я') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

void clear_line(FILE * fp) {
    int ch = 0;
    while ((ch = getc(fp)) != EOF && ch != '\n')
        ;
}

int skip_format_string(const char * format_string, FILE * fp) {
    assert( format_string != NULL);

    int i = 0;
    while (format_string[i] != '\0') {
        if (format_string[i++] != getc(fp))
            return 0;
    }
    return 1;
}

char * strip(char * str) {
    assert(str != NULL);

    char *stripped_str = (char *) calloc(MAX_WORD_SIZE, sizeof(char));
    while (*str != '\0' && !is_alpha(*str))
        str++;

    int i = 0;
    while (str[i] != '\0' && is_alpha(str[i]))
        stripped_str[i] = str[i++];
    return stripped_str;
}

void init_onegin(const char * file_name_src, char onegin_array[][MAX_SIZE], const char * format_string) {
    assert(file_name_src != NULL && onegin_array != NULL && format_string != NULL);

    FILE *fp = fopen(file_name_src, "r"); // opening file
    assert(fp != NULL);

    char str[MAX_SIZE] = {};
    size_t is_trailing_new_lines = 0;
    size_t strlen_format_string = strlen(format_string);
    char num_of_strofa = 1;
    char ch = 0;
    int row = 0;
    
    while (fgets(str, MAX_SIZE, fp) != NULL) {
        int i = 0;
        while (str[i] != '\0') {
            onegin_array[row][i++] = ch;
        }
        row++;
    }

    fclose(fp);
}

int main() {
    const char * const src = "Пушкин Александр. Евгений Онегин - royallib.ru.txt";
    const char * const dst = "онегин.txt";
    char onegin_array[ROWS][MAX_SIZE] = {};
    for (int i = 0; i < ROWS; i++) {
        printf("%d\n", i);
        for (int j = 0; j < MAX_SIZE; j++) {
            printf("%c", onegin_array[i][j]);
        }
    }


    return 0;
}