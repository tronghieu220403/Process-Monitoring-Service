#ifndef PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_
#define PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_

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

    class ProcessJsonConfiguration
    {
    private:
        std::vector< std::pair< std::string, MonitoringComponent > > data_;
    public:

    ProcessJsonConfiguration();
    ProcessJsonConfiguration(std::string content);

    bool SetContent(std::string content);

    std::vector< std::pair< std::string, MonitoringComponent > > GetData();
        
    };
}

#endif