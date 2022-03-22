#ifndef SAMKNOWS_LIBRARY_H
#define SAMKNOWS_LIBRARY_H

struct http_request_result {
    long response_code;
    char *ip;
    double namelookup_time;
    double connect_time;
    double starttransfer_time;
    double total_time;
};

extern void init();
extern void deinit();
extern void clear_http_headers();
extern int add_http_header(const char*);
extern int perform_http_request(struct http_request_result * _http_request_result);

#endif