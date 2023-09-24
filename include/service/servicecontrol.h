
#ifndef PROCESSMONITORING_SERVICE_SERVICECONTROL_H_
#define PROCESSMONITORING_SERVICE_SERVICECONTROL_H_

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
    class ServiceControl
    {
    private:
        std::string name_;
        SC_HANDLE h_services_control_manager_;

    public:

        std::string GetName() const;
        void SetName(std::string name);

        ServiceControl() = default;
        ServiceControl(const std::string& name);
        bool Create();
        bool Run();

        ~ServiceControl();
    };

}

#endif
