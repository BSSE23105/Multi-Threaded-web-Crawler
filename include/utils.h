#ifndef UTILS_H
#define UTILS_H

#define MAX_URLS 100
#define MAX_URL_LEN 256
#define MAX_CONTENT_LEN (1024 * 1024)

void read_urls(const char *filename, char urls[][MAX_URL_LEN], int *url_count);
char *trim_whitespace(char *s);

#endif // UTILS_H
