
#ifndef PROCESSMONITORING_SERVICE_PROCESSMONITORSERVICE_H_
#define PROCESSMONITORING_SERVICE_PROCESSMONITORSERVICE_H_

#include "ulti/everything.h"
#include "ulti/collections.h"
#include "service/serviceevent.h"

namespace pm
{

    class ProcessMonitoringService
    {
    private:

        inline static SERVICE_STATUS service_status_ = {0};
        inline static SERVICE_STATUS_HANDLE status_handle_ = nullptr;
        inline static ServiceEvent control_event_ = ServiceEvent();

    public:
        inline static const std::wstring kName = L"ProcessMonitoringService";

        static void ProcessMonitoringServiceCtrlHandler(DWORD ctrl_code);
        static void ProcessMonitoringWorkerFunction();
        static void ProcessMonitoringServiceMain();

        static void ProcessMonitoringServiceStop();
        static void ProcessMonitoringServicePause();
        static void ProcessMonitoringServiceContinue();
        static void ProcessMonitoringServiceShutdown();

        static void Close();

    };
}

#endif
