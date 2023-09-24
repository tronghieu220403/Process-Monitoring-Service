
#ifndef PROCESSMONITORING_SERVICE_SERVICE_H_
#define PROCESSMONITORING_SERVICE_SERVICE_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#else

#include "include/ulti/collections.h"
#endif

namespace pm
{
    class Service
    {
    private:
        std::wstring name_;
        LPSERVICE_MAIN_FUNCTION service_main_func_ = nullptr;

    public:

        LPSERVICE_MAIN_FUNCTION GetServiceMainFunc() const;
        void SetServiceMainFunc(LPSERVICE_MAIN_FUNCTION main_func);
        std::wstring GetName() const;
        void SetName(std::wstring name);

        void Start();
    };
}

#endif
