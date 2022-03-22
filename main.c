#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <getopt.h>
#include <errno.h>
#include "library.h"

int main(int argc, char **argv) {
    init();

    int opt;
    long n = 0;
    while ((opt = getopt(argc, argv, "H:n:")) != -1) {
        switch (opt) {
            case 'H':
                if(add_http_header(optarg)) {
                    fprintf(stderr, "error while reading '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n': {
                if(n != 0)  {
                    fprintf(stderr, "'n' value should be set only once\n");
                    exit(EXIT_FAILURE);
                }
                errno = 0;
                char *pEnd;
                n = strtol(optarg, &pEnd, 10);
                if(errno != 0 || optarg == pEnd) {
                    fprintf(stderr, "error while reading '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                }
                if(n <= 0) {
                    fprintf(stderr, "wrong 'n' value '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                }
                }
                break;
            default:
                fprintf(stderr, "usage: %s [-H or -n]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    struct http_request_result * _http_request_result = malloc(sizeof(struct http_request_result));
    struct http_request_result * _http_request_result_median = malloc(sizeof(struct http_request_result));
    memset(_http_request_result_median, 0, sizeof(struct http_request_result));

    for(int i = 0; i < n; ++i) {
        if(perform_http_request(_http_request_result)) {
            _http_request_result_median->namelookup_time += _http_request_result->namelookup_time;
            _http_request_result_median->connect_time += _http_request_result->connect_time;
            _http_request_result_median->starttransfer_time += _http_request_result->starttransfer_time;
            _http_request_result_median->total_time += _http_request_result->total_time;
        }
    }
    if(n > 0)
        printf("\nSKTEST;%s;%ld;%.6f;%.6f;%.6f;%.6f\n",
               _http_request_result->ip,
               _http_request_result->response_code,
                _http_request_result_median->namelookup_time / n,
                _http_request_result_median->connect_time / n,
                _http_request_result_median->starttransfer_time / n,
                _http_request_result_median->total_time / n);

    free(_http_request_result_median);
    free(_http_request_result);
    deinit();

    return 0;
}