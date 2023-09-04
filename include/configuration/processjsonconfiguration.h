#ifndef PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_
#define PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_

#define UNICODE
#define _UNICODE

#include <string.h>

#ifdef _WIN32
#include <winrt/windows.data.json.h>
#include <winrt/Windows.Foundation.Collections.h>
#define json_char TCHAR
#elif __linux__

#endif

#include <string>
#include <map>
#include <vector>

#include "ulti/collections.h"

namespace pm
{

    class ProcessJsonConfiguration
    {
    private:
        std::vector< std::pair< std::wstring, MonitoringComponent > > data;
    public:

    ProcessJsonConfiguration() = default;

    bool SetContent(std::wstring content);
    bool SetContentFromFile(std::wstring file_name);

    std::vector< std::pair< std::wstring, MonitoringComponent > > GetData();
    
    };
}

#endif