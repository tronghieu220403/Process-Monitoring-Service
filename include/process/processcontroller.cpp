
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processcontroller.h"
#else
#include "include/process/processcontroller.h"
#endif

namespace pm
{

	ProcessController::ProcessController(const std::string_view& name) 
	{
        pid_ = Process.FindProcessIdByName(name);
		this.name_ = static_cast<std::string>(name);
		SetHandle(pid_);
	}

	ProcessController::ProcessController(ProcessController& pc)
	{
		this.name_ = pc.name;
		this.p_info_ = pc.p_info_;
		this.ProcessController(pc.name);
	}

	bool ProcessController::SetHandle(int pid)
	{
		#ifdef _WIN32
			process_handle_ = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid_);
			if (process_handle_ == NULL)
			{
				return false;
			}
			p_info_ = ProcessInfo(process_handle_);
			return true;
		#elif __linux__

		#endif
	}

	bool ProcessController::SetHandle(const std::string_view& name){
		pid_ = FindProcessIdByName(name);
		if (pid_ == 0)
		{
			return false;
		}
		return SetHandle(pid_);
	};

	bool ProcessController::IsExists()
	{
		#ifdef _WIN32
			if (GetProcessId(process_handle_) == pid_ && pid_ != 0)
			{
				return true;
			}
			return false;
		#elif __linux__

		#endif
	}

	int ProcessController::GetPid() const
	{
        #ifdef _WIN32
            return GetProcessId(process_handle_);
        #elif __linux__
		    return FindProcessIdByName(name_);
        #endif
	}

	ProcessInfo ProcessController::GetProcessInfo()
	{
		if (IsExists())
		{
			return p_info_;
		}
		return ProcessInfo();
	}

    bool ProcessController::TryFindHandle()
    {
        #ifdef _WIN32
            if (IsExists())
            {
                return true;
            }
            else 
            {
                return SetHandle(name_);
            }
        #elif __linux__

        #endif
    };

	void ProcessController::Close()
	{
		#ifdef _WIN32
		if (process_handle_ != NULL)
		{
			CloseHandle(process_handle_);
			process_handle_ = NULL;
		}
		#elif __linux__

		#endif
	}

	ProcessController::~ProcessController()
	{
		Close();
	}
}