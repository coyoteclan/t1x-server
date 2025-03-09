#ifndef PCH_H
#define PCH_H

#include <unistd.h>     // access
#include <stdint.h>     // uint16_t
#include <cstdio>       // printf, snprintf
#include <cstring>      // strcasecmp, strlen, strcmp, strerror, strncpy...
#include <stdlib.h>     // atoi
#include <stdarg.h>     // va
#include <string>
#include <map>          // make_tuple, get, array
#include <sstream>      // ostringstream
#include <vector>
#include <array>
#include <cmath>        // fabs, sqrt
#include <signal.h>     // signal
#include <arpa/inet.h>  // sockaddr_in, inet_pton
#include <execinfo.h>   // backtrace
#include <dlfcn.h>      // dlsym
#include <sys/time.h>   // gettimeofday

#endif