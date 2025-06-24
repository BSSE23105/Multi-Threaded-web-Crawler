#ifndef PROCESSOR_H
#define PROCESSOR_H

int count_sentences(const char *text);
int count_words(const char *text);
void update_char_freq(const char *text, int char_freq[26]);

#endif // PROCESSOR_H
