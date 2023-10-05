#ifdef _WIN32

#include "counter.h"

namespace pm
{
    Counter::Counter(std::string& p_name,int pid, std::string type)
        : p_name_(p_name),pid_(pid), type_(type), h_counter_(0)
    {
        size_t idx = p_name_.rfind(".exe");
        if (idx != std::string::npos && idx == p_name_.length() - 4)
        {
            p_name_.erase(idx, 4);
        }
    }

    void Counter::UpdateQuery()
    {
        query_.Update();
    }

    bool Counter::AddCounter()
    {
        std::string cmd = "\\Process V2(" + p_name_ + ":" + std::to_string(pid_) + ")\\" + type_;
        std::wstring w_cmd(cmd.begin(), cmd.end());

        PDH_STATUS status = PdhAddCounter(query_.GetHQuery(), &w_cmd[0], NULL, &h_counter_);

        if (ERROR_SUCCESS != status)
        {
            if (status == PDH_INVALID_ARGUMENT)
            {
                return false;
            }
            else if (status == PDH_INVALID_DATA)
            {
                return false;
            }
            else if (status == PDH_INVALID_HANDLE)
            {
                return false;
            }
            else if (status == PDH_CSTATUS_BAD_COUNTERNAME)
            {
                return false;
            }
            else if (status == PDH_CSTATUS_NO_MACHINE)
            {
                return false;
            }
            else if (status == PDH_CSTATUS_NO_OBJECT)
            {
                return false;
            }
            else if (status == PDH_FUNCTION_NOT_FOUND)
            {
                return false;
            }
            else if (status == PDH_MEMORY_ALLOCATION_FAILURE)
            {
                return false;
            }

        }
        return true;
    }

    double Counter::GetValue()
    {
        PDH_FMT_COUNTERVALUE pdh_value;
        DWORD dw_value;

        if (h_counter_ == nullptr)
        {
            return 0.0;
        }

        PDH_STATUS status = PdhGetFormattedCounterValue(h_counter_, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dw_value, &pdh_value);
        if (status == PDH_INVALID_ARGUMENT)
        {
            return -1;
        }
        else if (status == PDH_INVALID_DATA)
        {
            return -1;
        }
        else if (status == PDH_INVALID_HANDLE)
        {
            return -1;
        }

        return pdh_value.doubleValue;
    }

    FILETIME Counter::GetLastQueryTime()
    {
        return query_.GetUpdateTime();
    }

    void Counter::CloseCounter()
    {
        if (h_counter_ != nullptr && h_counter_ != INVALID_HANDLE_VALUE)
        {
            PdhRemoveCounter(h_counter_);
        }
        return;
    }

    Counter::~Counter()
    {
        CloseCounter();
    }
}

#endif