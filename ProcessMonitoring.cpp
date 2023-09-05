#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <iphlpapi.h>
#endif

//#include "include/process/processsupervision.h"

int main()
{
    std::cout << "Hello World!\n";

    //auto p = pm::ProcessSupervision();

    std::vector<unsigned char> buffer;
    DWORD dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
    DWORD dwRetValue = 0;
    // repeat till buffer is big enough
    do
    {
        buffer.resize(dwSize, 0);
        dwRetValue = GetExtendedTcpTable(buffer.data(), &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);

    } while (dwRetValue == ERROR_INSUFFICIENT_BUFFER);

}

