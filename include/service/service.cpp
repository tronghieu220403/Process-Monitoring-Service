#include "service.h"

namespace pm
{

    LPSERVICE_MAIN_FUNCTION Service::GetServiceMainFunc() const {
        return service_main_func_;
    }

    void Service::SetServiceMainFunc(LPSERVICE_MAIN_FUNCTION main_func) {
        service_main_func_ = main_func;
    }

    std::wstring Service::GetName() const {
        return name_;
    }

    void Service::SetName(std::wstring name) {
        name_ = name;
    }

    void Service::Start()
    {
        WriteDebug("Begin Start");
        SERVICE_TABLE_ENTRY DispatchTable[] =
        {
            { &(name_)[0],				service_main_func_},
            { NULL,						NULL }
        };
        StartServiceCtrlDispatcher(DispatchTable);
    }

    void ServiceEvent::Close()
    {
        if (stop_event_ != INVALID_HANDLE_VALUE && stop_event_ != nullptr)
        {
            CloseHandle(stop_event_);
        }
        if (pause_event_ != INVALID_HANDLE_VALUE && pause_event_ != nullptr)
        {
            CloseHandle(pause_event_);
        }
        if (continue_event_ != INVALID_HANDLE_VALUE && continue_event_ != nullptr)
        {
            CloseHandle(continue_event_);
        }
    }

}
