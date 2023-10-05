#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef __linux__

#include "program/cta.h"
#include "program/ctb.h"

#define _CTA

#ifdef _CTA

int main()
{
	auto cta = std::make_shared<pm::CTA>();
	std::jthread connection_thread(&pm::CTA::CommunicateWithCtb, cta);
	cta->Monitoring();
	return 0;
}

#endif

#elif _WIN32

#include "ulti/everything.h"
#include "etw/wmieventclass.h"
#include "etw/event.h"
#include "etw/consumer.h"
#include "etw/provider.h"
#include "program/cta.h"
#include "service/processmonitorservice.h"
#include "service/servicecontrol.h"
#include "service/service.h"

bool provider_oke = false;
bool comsumer_oke = false;

void SetUpProvider()
{
    
    ULONG status;
    pm::KernelProvider kp(
        EVENT_TRACE_FLAG_NO_SYSCONFIG
        | EVENT_TRACE_FLAG_DISK_IO
        | EVENT_TRACE_FLAG_NETWORK_TCPIP
        | EVENT_TRACE_FLAG_THREAD
        );
    status = kp.BeginTrace();
    if (status != ERROR_SUCCESS && status != ERROR_ALREADY_EXISTS)
    {
        return;
    }
    provider_oke = true;
}

void SetUpComsumer()
{
    while (provider_oke == false)
    {
        Sleep(50);
    }
    std::vector<pm::IoInfo> temp;
    pm::KernelConsumer::SetDiskIoSharedVector(temp);
    pm::KernelConsumer::SetNetworkIoSharedVector(temp);
    pm::KernelConsumer kc;
    
    if (kc.Open() != ERROR_SUCCESS)
    {
        return;
    }

    comsumer_oke = true;

    if (kc.Process() != ERROR_SUCCESS)
    {

    }
    
    return;
}

void SetupCta()
{
    while (comsumer_oke == false)
    {
        Sleep(50);
    }
    auto cta = std::make_shared<pm::CTA>();
    std::jthread monitoring_thread(&pm::CTA::Monitoring, cta);
    std::jthread connection_thread(&pm::CTA::CommunicateWithCtb, cta);
}

LPVOID ServiceMainWorker()
{
    std::jthread provider_thread(&SetUpProvider);
    std::jthread comsumer_thread(&SetUpComsumer);
    std::jthread cta_thread(&SetupCta);
    provider_thread.join();
    comsumer_thread.join();
    cta_thread.join();
}

int main()
{

    pm::ServiceControl service_control(L"Process Monitoring Service");

    std::wstring w_path;
    w_path.resize(1000);
    GetModuleFileNameW(nullptr, &w_path[0], 1000);
    w_path.resize(wcslen(&w_path[0]));
    service_control.SetPath(w_path);
    if (service_control.Create() == true)
    {
        service_control.Run();
    }
    else
    {
        return 0;
    }

    pm::ProcessMonitoringService::SetMainWorkerFunction(ServiceMainWorker);
    pm::Service sc;
    sc.SetName(L"Process Monitoring Service");
    sc.SetServiceMainFunc((LPSERVICE_MAIN_FUNCTION)pm::ProcessMonitoringService::ProcessMonitoringServiceMain);
    sc.Start();
    return 0;
}

#endif

#endif

