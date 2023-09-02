#ifndef PROCESSMONITORING_PROCESS_PROCESSCONTROL_H_
#define PROCESSMONITORING_PROCESS_PROCESSCONTROL_H_

#define UNICODE
#define _UNICODE

#include<process/process.h>

namespace pm
{
    class ProcessControl: public Process
    {
    protected:

		ProcessInfo p_info_;

        bool SetHandle(int pid);
		bool SetHandle(const std::string_view& name);
		#ifdef _WIN32
			bool SetHandle(const std::wstring_view& name);
		#else

		#endif

		#ifdef _WIN32
			HANDLE process_handle_;
		#else

		#endif

    public:
        ProcessControl();
		explicit ProcessControl(const std::string_view& name);

        #ifdef _WIN32
			explicit ProcessControl(const std::wstring_view& wname);
		#endif

		bool IsExists();
        
        int GetPid() const;
        
		ProcessInfo GetProcessInfo();

		bool TryFindHandle();

        ~ProcessControl();
    };
}

#endif