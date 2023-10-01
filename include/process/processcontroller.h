#ifndef PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_
#define PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_

#include "ulti/everything.h"
#include "process/processinfo.h"
#include "process/process.h"

namespace pm
{
    class ProcessController: public Process
    {
    protected:

		ProcessInfo p_info_;

        bool SetHandle(int pid);
		bool SetHandle(const std::string& name);

		#ifdef _WIN32
			HANDLE process_handle_ = nullptr;
		#elif __linux__

		#endif

    public:

        ProcessController() = default;
		explicit ProcessController(const std::string& name);
		ProcessController(const ProcessController& pc);
		explicit ProcessController(const ProcessController&& pc) = delete;

        ProcessController& operator=(const ProcessController& pc);

		bool IsExists();
        
        int GetPid() override;

		ProcessInfo& GetProcessInfo();

		bool TryFindHandle();

        void Close();

		~ProcessController();
    };
}

#endif