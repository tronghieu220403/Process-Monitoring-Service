#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_
#define PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#include <string.h>

#ifdef _WIN32
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.data.json.h>
#define json_char TCHAR
#elif __linux__

#endif

#include <string>
#include <map>
#include <vector>

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#else
#include "include/ulti/collections.h"
#endif


namespace pm
{

    class ProcessJsonConfiguration
    {
    private:
        std::vector< std::pair< std::string, MonitoringComponent > > data;
    public:

    ProcessJsonConfiguration();
    ProcessJsonConfiguration(std::string content);

    bool SetContent(std::string content);

    std::vector< std::pair< std::string, MonitoringComponent > > GetData();
        
    };
}

#endif