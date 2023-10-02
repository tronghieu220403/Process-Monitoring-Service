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


    void Query::Update()
    {
        PdhCollectQueryData(query_);
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