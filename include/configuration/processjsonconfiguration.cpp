
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processjsonconfiguration.h"
#else
#include "include/configuration/processjsonconfiguration.h"
#endif


namespace pm
{
    ProcessJsonConfiguration::ProcessJsonConfiguration()
    {
        data_.clear();
    }
    
    ProcessJsonConfiguration::ProcessJsonConfiguration(std::string contents)
    {
        data_.clear();
        SetContent(contents);
    }


    bool ProcessJsonConfiguration::SetContent(std::string contents)
    {
        #ifdef _WIN32
            data_.clear();
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

                    data_.push_back(std::make_pair(name, max_usage));
                }
            }
        #elif __linux__
                using json = nlohmann::json;

                json json_data;
                try
                {
                    json_data = json::parse(contents);
                }
                catch(const std::exception& e)
                {
                    return false;
                }
                
                if (json_data.is_array())
                {
                    for (json::iterator it = json_data.begin(); it != json_data.end(); ++it) 
                    {
                        //cout << it->type_name() << endl;
                        if (!it->is_object())
                        {
                            continue;
                        }
                        bool check = true;
                        std::string name;
                        MonitoringComponent mc;
                        for (json::iterator ele = it->begin(); ele != it->end(); ++ele) {
                            if (ele->is_string())
                            {
                                name = *ele;
                            }
                            else if (ele->is_number())
                            {
                                if (ele.key() == "cpu")
                                {
                                    mc.cpu_usage = double(*ele);
                                }
                                else if (ele.key() == "memory")
                                {
                                    mc.mem_usage = double(*ele);
                                }
                                else if (ele.key() == "disk")
                                {
                                    mc.disk_usage = double(*ele);
                                }
                                else if (ele.key() == "network")
                                {
                                    mc.network_usage = double(*ele);
                                }
                                else
                                {
                                    check = false;
                                    break;
                                }
                            }
                            else
                            {
                                check = false;
                                break;
                            }
                        }
                        if (check == true)
                        {
                            ProcessJsonConfiguration::data_.push_back(make_pair(name, mc));
                        }
                    }
                }
                else
                {
                    return false;
                }

        #endif
        return true;
    }

    std::vector< std::pair< std::string, MonitoringComponent > > ProcessJsonConfiguration::GetData()
    {
        return data_;
    }

}