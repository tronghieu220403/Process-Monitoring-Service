#ifdef _WIN32
    #include <Windows.h>
#elif __linux__

#endif

#include <iostream>

#ifdef _WIN32
    #define ZeroMem(data,size) ZeroMemory(data,size)
#elif __linux__
    #define ZeroMem(data,sz) memset(data,size)
#endif
