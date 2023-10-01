#ifndef PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_
#define PROCESSMONITORING_CONFIGURATION_PROCESSJSONCONFIGURATION_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

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