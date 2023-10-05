#include "processdisk.h"

namespace pm
{

ProcessDiskStats::ProcessDiskStats()
{

};

ProcessDiskStats::ProcessDiskStats(int pid):
    pid_(pid)
{
    #ifdef __linux__
        if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
        {
            pid_ = 0;
            return;
        }
        last_io_ = GetCurrentCounter();
        last_time_ = clock();
    #endif
}

void ProcessDiskStats::SetPid(int pid)
{
    pid_ = pid;
}

int ProcessDiskStats::GetPid() const
{
    return pid_;
}

#ifdef _WIN32

    void ProcessDiskStats::AddData(FILETIME time, unsigned long long data)
    {
        UsageData io_data;
        SYSTEMTIME last_io_st;
        SYSTEMTIME st;

        if (io_deque_.size() == 0)
        {
            io_data.data = static_cast<double>(data);
            io_data.time = time;
            io_deque_.push_back(io_data);
            return;
        }
        UsageData& last_io_data = io_deque_.back();
        FileTimeToSystemTime(&time, &st);

        unsigned long long data_ym = st.wYear * 12 + st.wMonth;
        unsigned long long data_dhms = st.wDay * 3600 * 12 + st.wHour * 3600 + st.wMinute * 60 + st.wSecond;

        FileTimeToSystemTime(&last_io_data.time, &last_io_st);
        unsigned long long last_ym = last_io_st.wYear * 12 + last_io_st.wMonth;
        unsigned long long last_dhms = last_io_st.wDay * 3600 * 12 + last_io_st.wHour * 3600 + last_io_st.wMinute * 60 + last_io_st.wSecond;

        if (last_ym == data_ym)
        {
            if (last_dhms == data_dhms)
            {
                last_io_data.data += (double)data;
            }
            else if (last_dhms < data_dhms)
            {
                io_data.data = (double)data;
                io_data.time = time;
                io_deque_.push_back(io_data);
            }
        }
        else if (last_ym < data_ym)
        {
            io_data.data = (double)data;
            io_data.time = time;
            io_deque_.push_back(io_data);
        }
    }

    bool ProcessDiskStats::HasData() const
    {
        return io_deque_.size() > 1;
    }

    UsageData ProcessDiskStats::GetFrontIoDataInMb()
    {
        UsageData usage_data;
        usage_data.time = io_deque_.front().time;
        usage_data.data = (double)(io_deque_.front().data) / 1024 / 1024;
        return usage_data;
    }


    void ProcessDiskStats::DeleteFrontIodata()
    {
        io_deque_.pop_front();
    }


#elif __linux__

unsigned long long ProcessDiskStats::GetCurrentCounter()
{
    if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
    {
        return 0;
    }

    std::ifstream file("/proc/" + std::to_string(pid_) + "/io");
    std::string line;

    unsigned long long r;
    unsigned long long w;

    while(getline(file, line))
    {
        if (line.find("read_bytes") == 0)
        {
            std::stringstream s(line);
            std::string name;
            s >> name >> r;
        }
        if (line.find("write_bytes") == 0)
        {
            std::stringstream s(line);
            std::string name;
            s >> name >> w;
            break;
        }
    }
    return r + w;
}

void ProcessDiskStats::UpdateAttributes()
{
    clock_t now_time;

    if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
    {
        last_speed_ = 0;
        last_io_ = 0;
        return;
    }

    now_time = clock();

    double time_range_in_sec = (double)(now_time - last_time_) / CLOCKS_PER_SEC;

    unsigned long long cur_io = GetCurrentCounter();
    
    if (cur_io == 0)
    {
        last_speed_ = 0;
        last_io_ = 0;
        return;
    }

    last_retrieved_time_ = time(0);
    last_speed_ = static_cast<double>(cur_io - last_io_) / time_range_in_sec;
    last_time_ = now_time;
    last_io_ = cur_io;
};

UsageData ProcessDiskStats::GetLastIoSpeedInMb()
{
    UsageData usage_data;
    usage_data.time = last_retrieved_time_;
    usage_data.data = static_cast<double>(last_speed_) / (1024 * 1024);
    return usage_data;
};

#endif


}