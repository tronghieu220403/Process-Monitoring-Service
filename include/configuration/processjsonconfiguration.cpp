
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processjsonconfiguration.h"
#else
#include "include/configuration/processjsonconfiguration.h"
#endif


namespace pm
{
    ProcessJsonConfiguration::ProcessJsonConfiguration()
    {
        data.clear();
    }
    
    ProcessJsonConfiguration::ProcessJsonConfiguration(std::string contents)
    {
        data.clear();
        SetContent(contents);
    }


    bool ProcessJsonConfiguration::SetContent(std::string contents)
    {
        #ifdef _WIN32
            data.clear();
            using namespace winrt;
            using namespace winrt::Windows::Data::Json;

            auto arr = JsonArray();


            if (std::wstring wcontent(contents.begin(), contents.end()); JsonArray::TryParse(wcontent, arr) != true)
            {
                return false;
            }

            for (auto ele: arr)
            {
                if (ele.ValueType() != JsonValueType::Object)
                {
                    continue;
                }
                JsonObject obj = ele.GetObject();
                if (!obj.HasKey(L"process") || !obj.HasKey(L"cpu") || !obj.HasKey(L"memory") || !obj.HasKey(L"disk") || !obj.HasKey(L"network"))
                {
                    continue;
                }

                auto process = obj.Lookup(L"process");
                auto cpu = obj.Lookup(L"cpu");
                auto memory = obj.Lookup(L"memory");
                auto disk = obj.Lookup(L"disk");
                auto network = obj.Lookup(L"network");

                if (process.ValueType() == JsonValueType::String && cpu.ValueType() == JsonValueType::Number && memory.ValueType() == JsonValueType::Number && disk.ValueType() == JsonValueType::Number && network.ValueType() == JsonValueType::Number)
                {
                    std::string name(process.GetString().begin(), process.GetString().end());
                    MonitoringComponent max_usage{};
                    max_usage.cpu_usage = cpu.GetNumber();
                    max_usage.mem_usage = memory.GetNumber();
                    max_usage.disk_usage = disk.GetNumber();
                    max_usage.network_usage = network.GetNumber();

                    data.push_back(std::make_pair(name, max_usage));
                }
            }
        #elif __linux__

        #endif
        return true;
    }

    std::vector< std::pair< std::string, MonitoringComponent > > ProcessJsonConfiguration::GetData()
    {
        return data;
    }

}