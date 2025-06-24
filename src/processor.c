#include <ctype.h>
#include <stddef.h>
#include "processor.h"

int count_sentences(const char *text) {
    int count = 0;
    for (size_t i = 0; text[i]; i++) {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
            count++;
    }
    return count;
}

void update_char_freq(const char *text, int char_freq[26]) {
    for (size_t i = 0; text[i]; i++) {
        unsigned char uc = (unsigned char)text[i];
        if (isalpha(uc)) char_freq[tolower(uc) - 'a']++;
    }
}

int count_words(const char *text) {
    int count = 0, in_word = 0;
    for (size_t i = 0; text[i]; i++) {
        if (isspace((unsigned char)text[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
    }
    return count;
}
