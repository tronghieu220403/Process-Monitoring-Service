
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "file.h"
#else
#include "include/file/file.h"
#endif


// how to ?
namespace pm
{

bool File::Set(std::wstring file_name)
{
    #ifdef _WIN32
        return true;
    #elif __linux__
        return false;
    #endif
}

}