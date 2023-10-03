#include "servicecontrol.h"

namespace pm
{
    std::string ServiceControl::GetName() const {
        return name_;
    }

    void ServiceControl::SetName(std::string name) {
        name_ = name;
    }


    ServiceControl::ServiceControl(const std::string& name)
        : name_(name),
        h_services_control_manager_(OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS))
    {
    }

    ServiceControl::~ServiceControl()
    {
        CloseServiceHandle(h_services_control_manager_);
        h_services_control_manager_ = 0;
    }

    bool ServiceControl::Create()
    {
        std::wstring w_name(name_.begin(), name_.end());
        std::wstring path;
        path.resize(MAX_PATH + 1);
        GetFullPathNameW(w_name.data(), MAX_PATH + 1, &path[0], NULL);
        path.resize(wcslen(path.data()));
        path += (L"\\");

        SC_HANDLE handle_service = CreateServiceW(h_services_control_manager_, w_name.data(), w_name.data(),
            SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
            SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
            path.data(), NULL, NULL, NULL, NULL, NULL);

        if (handle_service == NULL)
        {
            return false;
        }

        CloseServiceHandle(handle_service);
        return true;
    }

    bool ServiceControl::Run()
    {
        SC_HANDLE handle_service;
        std::wstring working_dir;
        working_dir.resize(MAX_PATH + 1);
        std::wstring w_name(name_.begin(), name_.end());

        GetCurrentDirectoryW(MAX_PATH + 1, &working_dir[0]);
        LPCWSTR argv_start[] = { &w_name[0], &working_dir[0] };

        handle_service = OpenServiceW(h_services_control_manager_, w_name.data(), SERVICE_ALL_ACCESS);

        if (handle_service == NULL)
        {
            return false;
        }

        if (!StartServiceW(handle_service, 2, argv_start))
        {
            return false;
        }
        CloseServiceHandle(handle_service);
        return true;
    }

}
