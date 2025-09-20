#ifndef ONEGIRI_H
#define ONEGIRI_H


const int MAX_SIZE = 1000;
const int ROWS = 8000;
const int MAX_WORD_SIZE = 100;
const int LINES_IN_STROFA = 14;
const int MINIMUM_RHYME_DISTANCE = 15;
const int MINIMUM_NUM_FOR_GOOD_RHYME = 4;
const int MAXIMUM_NUM_FOR_GOOD_RHYME = 10;
const int RANGE_TO_SKIP_NUMBERS = 5;
const int NUM_OF_RHYTHM_TYPES = 7;
const int PART_OF_ARRAY = 15;

enum error_codes{
    ERROR_CODE = -1, ERR_PTR_IS_NULL = -2
};
typedef const char * const string;

const char * const pairs[2] = {
    "ןפךעס", "בגדהח"
};
const int onegin_strofa_structure[LINES_IN_STROFA] = {0, 1, 0, 1, 2, 2, 3, 3, 4, 5, 5, 4, 6, 6};
const int onegin_rhythm_types[LINES_IN_STROFA]     = {1, 2, 1, 2, 3, 3, 4, 4, 5, 6, 6, 5, 7, 7};

struct ptr_line {
    char * ptr_line;
    char * ptr_last_word_transcription;
    size_t len_last_word_transcription;
    size_t len_line;
    size_t rhythm_type;
};

#ifndef NDEBUG
#define sassert(condition, ERROR_CODE, ...) {                                                   \
                        if (!(condition)){                                                      \
                            fprintf(stderr, "-->ERROR: in file \"%s\"\n"                        \
                                            "             line %d", __FILE_NAME__, __LINE__);   \
                            fprintf(stderr, "\n" __VA_ARGS__ );                                 \
                            putc('\n', stderr);                                                 \
                            exit(ERROR_CODE);                                                   \
                    }}
#else 
#define sassert(condition, ...) ((void)0)
#endif // sassert

#ifndef NDEBUG
#define push_error(ERROR_CODE, ...) {                                                           \
                            fprintf(stderr, "\n" __VA_ARGS__ );                                  \
                            putc('\n', stderr);                                                  \
                            exit(ERROR_CODE);                                                    \
                    }
#else 
#define sassert(condition, ...) ((void)0)
#endif // print_error


#endif // ONEGIRI_H