#include "library.h"

#include <curl/curl.h>
#include <ctype.h>

CURL *curl = NULL;
struct curl_slist *list = NULL;

// __attribute__((constructor))
 void init() {
    curl = curl_easy_init();
}

//__attribute__((destructor))
void deinit() {
    curl_easy_cleanup(curl);
}

void clear_http_headers() {
    curl_slist_free_all(list);
}

int add_http_header(const char* http_header) {
    for(const char* cur = http_header; *cur != '\0'; ++cur)
        if(!isprint(*cur))
            return 1;
    list = curl_slist_append(list, http_header);
    return 0;
}

int perform_http_request(struct http_request_result * _http_request_result) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com/");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    return !curl_easy_perform(curl) &&
    !curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &_http_request_result->response_code) &&
    !curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &_http_request_result->ip) && _http_request_result->ip &&
    !curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &_http_request_result->namelookup_time) &&
    !curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &_http_request_result->connect_time) &&
    !curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &_http_request_result->starttransfer_time) &&
    !curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &_http_request_result->total_time);
}