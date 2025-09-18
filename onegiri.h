#ifndef ONEGIRI_H
#define ONEGIRI_H


const int MAX_SIZE = 1000;
const int ROWS = 8000;
const int MAX_WORD_SIZE = 100;
const int LINES_IN_STROFA = 14;
const int MINIMUM_RHYME_DISTANCE = 15;
const int MINIMUM_NUM_FOR_GOOD_RHYME = 5;
const int MAXIMUM_NUM_FOR_GOOD_RHYME = 10;
enum error_codes{
    ERROR_CODE = -1, ERR_PTR_IS_NULL = -2
};
typedef const char * const string;

const char * const pairs[2] = {
    "ןפךעס", "בגדהח"
};

struct ptr_line {
    char * ptr_line;
    char * ptr_last_word_transcription;
    size_t len_last_word_transcription;
    size_t len_line;
    bool is_nine_slogov;
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


#endif // ONEGIRI_H