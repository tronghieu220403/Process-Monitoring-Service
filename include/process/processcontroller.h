#ifndef PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_
#define PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_

#define UNICODE
#define _UNICODE

#include<process/process.h>

namespace pm
{
    class ProcessController: public Process
    {
    protected:

		ProcessInfo p_info_;

        bool SetHandle(int pid);
		bool SetHandle(const std::string_view& name);
		#ifdef _WIN32
			bool SetHandle(const std::wstring_view& name);
		#elif __linux__

		#endif

		#ifdef _WIN32
			HANDLE process_handle_;
		#elif __linux__

		#endif

    public:
        ProcessController();
		explicit ProcessController(const std::string_view& name);

        #ifdef _WIN32
			explicit ProcessController(const std::wstring_view& wname);
		#endif

		bool IsExists();
        
        int GetPid() const override;

		ProcessInfo GetProcessInfo();

		bool TryFindHandle();

        void Close();
    };
}

#endif