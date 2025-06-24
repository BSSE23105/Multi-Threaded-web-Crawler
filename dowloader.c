
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include "downloader.h"
#include "utils.h"

struct MemoryStruct { char *memory; size_t size; };

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
size_t realsize = size * nmemb;
struct MemoryStruct *mem = userp;
if (mem->size + realsize + 1 > MAX_CONTENT_LEN)
realsize = (mem->size < MAX_CONTENT_LEN)
? (MAX_CONTENT_LEN - mem->size) : 0;
char *ptr = realloc(mem->memory, mem->size + realsize + 1);
if (!ptr) return 0;
mem->memory = ptr;
memcpy(mem->memory + mem->size, contents, realsize);
mem->size += realsize;
mem->memory[mem->size] = '\0';
return realsize;
}

char *download_url(const char *url, long long *download_time_ns) {
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
CURL *curl = curl_easy_init();
struct MemoryStruct chunk = { malloc(1), 0 };
if (!curl) { free(chunk.memory); return NULL; }
curl_easy_setopt(curl, CURLOPT_URL, url);
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
if (curl_easy_perform(curl) != CURLE_OK) {
    free(chunk.memory);
    chunk.memory = NULL;
}
curl_easy_cleanup(curl);

clock_gettime(CLOCK_MONOTONIC, &end);
*download_time_ns = (end.tv_sec - start.tv_sec) * 1000000000LL +
                    (end.tv_nsec - start.tv_nsec);
return chunk.memory;
}