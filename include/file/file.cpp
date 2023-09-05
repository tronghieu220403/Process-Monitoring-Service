#include "file/file.h"

// how to ?
namespace pm
{

bool File::Set(std::wstring file_name)
{
    #ifdef _WIN32
        HANDLE file = CreateFile(&file_name[0], GENERIC_READ, 0, NULL, OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL, NULL);
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

        DWORD bytes_read;
        if (!ReadFileEx(file, &buffer[0], file_size_, nullptr, NULL)) {
            CloseHandle(file);
            return L"";
        }

        CloseHandle(file);
        return buffer;

    #elif __linux__

    #endif
}

}