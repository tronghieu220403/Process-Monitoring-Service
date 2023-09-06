#pragma once

#ifndef PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_
#define PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_

#define UNICODE
#define _UNICODE

#include <string.h>

#ifdef _WIN32
#include <winrt/windows.data.json.h>
#include <winrt/windows.foundation.collections.h>
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
        std::vector< std::pair< std::wstring, MonitoringComponent > > data;
    public:

    ProcessJsonConfiguration();

    bool SetContent(std::string content);
    //bool SetContentFromFile(std::wstring file_name);

    std::vector< std::pair< std::wstring, MonitoringComponent > > GetData() const;
        
    };
}

#endif