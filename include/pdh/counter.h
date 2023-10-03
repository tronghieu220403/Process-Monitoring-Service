
#ifndef PROCESSMONITORING_PDH_COUNTER_H_
#define PROCESSMONITORING_PDH_COUNTER_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

#include "pdh/query.h"

namespace pm
{
    class Counter
    {
    private:
        inline static Query query_;

        int pid_;

        std::string p_name_;

        std::string type_;

        HCOUNTER h_counter_;
        
    public:

        Counter() = default;
        Counter(std::string& p_name, int pid, std::string type);

        static void UpdateQuery();

        bool AddCounter();

        double GetValue();

        void CloseCounter();

        ~Counter();
    };
}

#endif
