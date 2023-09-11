#include "src/CTA.h"

namespace pm
{
    CTA::CTA() = default;

    void CTA::UpdateConfig()
    {
        #ifdef _WIN32
        //using mutex here
            if (new_config_ == false)
            {
                return;
            }
            Registry reg("SOFTWARE/CtaProcessMonitoring/ProcsesConf");
            std::vector< std::pair< std::string, std::vector<char> > > info = reg.GetAllBinaryValues();
            process_.clear();
            for (int i = 0; i < info.size(); i++)
            {
                MonitoringComponent mc;
                memcpy(&mc.cpu_usage, &(info[i].second[0 * sizeof(double)]), sizeof(double));
                memcpy(&mc.mem_usage, &(info[i].second[1 * sizeof(double)]), sizeof(double));
                memcpy(&mc.disk_usage, &(info[i].second[2 * sizeof(double)]), sizeof(double));
                memcpy(&mc.network_usage, &(info[i].second[3 * sizeof(double)]), sizeof(double));
                process_.push_back(ProcessSupervision(info[i].first, mc));
            }
        //end mutex here
        #elif __linux__

        #endif
    }

    void CTA::Monitoring()
    {
        while(true)
        {
            Sleep(1000);
            UpdateConfig();
            for (auto ps: process_)
            {
                ps.UpdateProcessStats();
                ps.CheckProcessStats();
                if ((ps.GetProcessLogger()->GetMessage()).size() != 0)
                {
                    // using mutex here
                    log_info_.append(ps.GetProcessLogger()->GetMessage());
                    // using mutex here
                }
            }
        }
    }

    void 
}