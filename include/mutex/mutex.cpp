#ifdef _VISUAL_STUDIO_WORKSPACE
#include "mutex.h"
#else
#include "include/mutex/mutex.h"
#endif

namespace pm
{
    NamedMutex::NamedMutex(const std::string& mutex_name):
        mutex_name_(mutex_name)
    {
        SetMutex(mutex_name);
    }

    void NamedMutex::SetMutex(const std::string& mutex_name)
    {
        #ifdef _WIN32
            handle_mutex_ = CreateMutexA(
                                NULL, 
                                FALSE, 
                                &mutex_name[0]
                            );
        #elif __linux

        #endif
    }

    std::string NamedMutex::GetMutexName() const
    {
        return mutex_name_;
    }

    void NamedMutex::Lock()
    {
        #ifdef _WIN32
            WaitForSingleObject(handle_mutex_, INFINITY);
        #elif __linux__

        #endif
    }

    void NamedMutex::Unlock()
    {
        #ifdef _WIN32
            ReleaseMutex(handle_mutex_);
        #elif __linux__

        #endif
    }

    void NamedMutex::Close()
    {
        #ifdef _WIN32
            if (handle_mutex_ == nullptr)
            {
                CloseHandle(handle_mutex_);
                handle_mutex_ = 0;
            }
        #elif __linux__

        #endif
    }

    NamedMutex::~NamedMutex()
    {
        Close();
    }
}