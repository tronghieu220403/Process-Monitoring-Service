#ifndef PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_
#define PROCESSMONITORING_PROCESS_PROCESSCONTROLLER_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processinfo.h"
#include "E:/Code/Github/Process-Monitoring/include/process/process.h"
#else
#include "include/process/process.h"
#include "include/process/processinfo.h"
#endif


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