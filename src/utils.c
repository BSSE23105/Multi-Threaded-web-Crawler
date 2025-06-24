#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

char *trim_whitespace(char *s) {
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (*start == '\0') return start;
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return start;
}

void read_urls(const char *filename, char urls[][MAX_URL_LEN], int *url_count) {
    FILE *file = fopen(filename, "r");
    if (!file) { perror("Error opening URL file"); exit(1); }
    char line[MAX_URL_LEN];
    *url_count = 0;
    while (*url_count < MAX_URLS && fgets(line, sizeof(line), file)) {
        char *trimmed = trim_whitespace(line);
        if (*trimmed == '\0' || *trimmed == '#') continue;
        strncpy(urls[*url_count], trimmed, MAX_URL_LEN - 1);
        urls[*url_count][MAX_URL_LEN - 1] = '\0';
        (*url_count)++;
    }
    fclose(file);
    if (*url_count == 0) { fprintf(stderr, "No URLs loaded from %s\n", filename); exit(1); }
}
