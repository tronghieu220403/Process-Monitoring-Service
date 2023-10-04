
#ifndef PROCESSMONITORING_PDH_QUERY_H_
#define PROCESSMONITORING_PDH_QUERY_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

namespace pm
{
    class Query
    {
    private:
        HQUERY query_;
        FILETIME update_time_;
    public:

        Query();

        HQUERY GetHQuery();

        explicit Query(Query& q) = delete;
        explicit Query(Query&& q) = delete;
        Query operator=(Query const& q) = delete;

        FILETIME GetUpdateTime() const;

        bool Open();
        void Update();
        void Close();

        ~Query();
    };
}

#endif
