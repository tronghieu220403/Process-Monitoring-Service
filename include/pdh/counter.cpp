#include "counter.h"

namespace pm
{
    Counter::Counter(std::string& p_name, std::string type, HQUERY query)
        : p_name_(p_name), type_(type), query_(query), h_counter_(0)
    {
    }

    bool Counter::AddCounter()
    {
        std::string cmd = "\\Process V2(" + p_name_ + ")" + "\\" + type_;
        std::wstring w_cmd(cmd.begin(), cmd.end());

        PDH_STATUS status = PdhAddCounter(query_, w_cmd.data(), NULL, &h_counter_);

        if (ERROR_SUCCESS != status)
        {
            return false;
        }
        return true;
    }

    double Counter::GetValue()
    {
        PDH_FMT_COUNTERVALUE pdh_value;
        DWORD dw_value;

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


}