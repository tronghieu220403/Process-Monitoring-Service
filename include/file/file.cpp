
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
        HANDLE file = CreateFile(&file_name[0], GENERIC_READ, 0, nullptr, OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file == INVALID_HANDLE_VALUE) {
            return false;
        }

        LARGE_INTEGER file_size;
        if (GetFileSizeEx(file, &file_size) == 0){
            CloseHandle(file);
            return false;
        }

        std::wstring buffer;
        buffer.resize(file_size.QuadPart / sizeof(wchar_t));
        file_size_ = file_size.QuadPart;

        LPOVERLAPPED lpOverlapped;
        LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine;
        ZeroMemory(&lpOverlapped, sizeof(lpOverlapped));
        ZeroMemory(&lpCompletionRoutine, sizeof(lpCompletionRoutine));

        if (!ReadFileEx(file, &buffer[0], static_cast<DWORD>(file_size_), lpOverlapped, lpCompletionRoutine)) {
            CloseHandle(file);
            return L"";
        }

        CloseHandle(file);

    #elif __linux__
        return false;

    #endif
}

}