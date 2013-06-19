#include "compatibility.hpp"
#include <stdlib.h>
#include <time.h>
#ifndef HAVE_STRTOLL
long long int strtoll(const char *nptr, char **endptr, int base) {
    return strtol(nptr, endptr, base);
}
#endif
#ifndef HAVE_STRTOF
float strtof (const char *nptr, char **endptr) {
#ifdef HAVE_ATOF
    return (float) atof(nptr);
#endif
}
#endif
#ifndef HAVE_LOCALTIME_R
// FIXME: this is not really thread safe, it's just a hack to compile with MSC
// use something like mutex (W32) / pthread_mutex (LINUX)
struct tm *localtime_r(const time_t *clock, struct tm *result) {
    struct tm *tmp;

    tmp = localtime(clock);
    if ( tmp )
    {
        *result = *tmp;
        tmp = result;
    }
    return tmp;
}
#endif
                

