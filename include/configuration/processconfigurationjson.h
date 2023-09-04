#ifndef PROCESSMONITORING_CONFIGURATION_PROCESSCONFIGURATIONJSON_H_
#define PROCESSMONITORING_CONFIGURATION_PROCESSCONFIGURATIONJSON_H_

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

    class ProcessConfigurationJson
    {
    private:
        std::map< std::wstring, std::vector<MonitoringComponent> > mp;
    public:

    ProcessConfigurationJson() = default;

    bool SetContent(std::wstring content_);

    };
}

#endif