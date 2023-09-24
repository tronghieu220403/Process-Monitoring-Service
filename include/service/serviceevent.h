
#ifndef PROCESSMONITORING_SERVICE_SERVICEEVENT_H_
#define PROCESSMONITORING_SERVICE_SERVICEEVENT_H_

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
    struct ServiceEvent
    {
        public: 
            HANDLE stop_event_ = nullptr;
            HANDLE pause_event_ = nullptr;
            
            HANDLE pause_handled_ = nullptr;
            HANDLE stop_handled_ = nullptr;

            void Close();
    };
}

#endif
