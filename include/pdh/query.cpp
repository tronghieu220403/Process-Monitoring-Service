#ifdef _WIN32

#include "query.h"

namespace pm
{
    Query::Query()
    {
        Open();
    }

    HQUERY Query::GetHQuery()
    {
        return query_;
    }

    FILETIME Query::GetUpdateTime() const
    {
        return update_time_;
    }

    void Query::Update()
    {
        PdhCollectQueryData(query_);
        GetSystemTimeAsFileTime(&update_time_);
    }

    void Query::Close()
    {
        PdhCloseQuery(query_);
    }


    bool Query::Open()
    {
        PDH_STATUS status = PdhOpenQuery(0, 0, &query_);
        if (ERROR_SUCCESS != status)
        {
            return false;
        }
        return true;
    }

    Query::~Query()
    {
        Close();
    }

}
#endif
