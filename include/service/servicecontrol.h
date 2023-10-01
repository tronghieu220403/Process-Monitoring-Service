
#ifndef PROCESSMONITORING_SERVICE_SERVICECONTROL_H_
#define PROCESSMONITORING_SERVICE_SERVICECONTROL_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

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
