#ifndef GMARK_TIME_H
#define GMARK_TIME_H

#ifdef _WIN32
    #include <Windows.h>
    int gettimeofday(struct timeval *tp, struct timezone *tzp);
#else // _WIN32
    #include <sys/time.h>
#endif // _WIN32

#endif // GMARK_TIME_H
