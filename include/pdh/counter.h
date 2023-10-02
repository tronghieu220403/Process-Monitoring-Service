
#ifndef PROCESSMONITORING_PDH_COUNTER_H_
#define PROCESSMONITORING_PDH_COUNTER_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

namespace pm
{
    class Counter
    {
    private:
        std::string p_name_;
        HQUERY query_;
        std::string type_;
        HCOUNTER h_counter_;
        
    public:

        Counter() = default;
        Counter(std::string& p_name, std::string type, HQUERY query);

        bool AddCounter();

        double GetValue();
    };
}

#endif
