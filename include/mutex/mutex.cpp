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
        if (mutex_name.size() != 0)
            handle_mutex_ = CreateMutexA(
                                NULL, 
                                FALSE, 
                                mutex_name.size() != 0 ? &mutex_name[0] : NULL
                            );
        #elif __linux
        if (mutex_name.size() != 0)
        {
            std::string name = "/" + mutex_name;
            sema_ = sem_open(name.data(), O_RDWR | O_CREAT, 0660, 1);
            if (sema_ == SEM_FAILED){
                sema_ = sem_open(name.data(), O_RDWR);
                return;
            }
        }
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
            sem_wait(sema_);
        #endif
    }

    void NamedMutex::Unlock()
    {
        #ifdef _WIN32
            ReleaseMutex(handle_mutex_);
        #elif __linux__
            sem_post(sema_);
        #endif
    }

    void NamedMutex::Close()
    {
        #ifdef _WIN32
            if (handle_mutex_ != nullptr)
            {
                CloseHandle(handle_mutex_);
                handle_mutex_ = 0;
            }
        #elif __linux__
            if (sema_ != nullptr)
            {
                sem_destroy(sema_);
                sema_ = 0;
            }
        #endif
    }

    NamedMutex::~NamedMutex()
    {
        Close();
    }
}