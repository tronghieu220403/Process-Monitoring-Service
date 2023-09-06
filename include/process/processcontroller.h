#ifndef PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_
#define PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_

#define UNICODE
#define _UNICODE

#include "include/process/process.h"
#include "include/process/processinfo.h"

namespace pm
{
    class ProcessController: public Process
    {
    protected:

		ProcessInfo p_info_;

        bool SetHandle(int pid);
		bool SetHandle(const std::string_view& name);

		#ifdef _WIN32
			HANDLE process_handle_;
		#elif __linux__

		#endif

    public:

        ProcessController() = default;
		explicit ProcessController(const std::string_view& name);
		explicit ProcessController(ProcessController& pc);
		explicit ProcessController(ProcessController&& pc) = delete;


		bool IsExists();
        
        int GetPid() const override;

		ProcessInfo GetProcessInfo();

		bool TryFindHandle();

        void Close();
    };
}

#endif