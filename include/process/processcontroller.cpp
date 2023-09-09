
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processcontroller.h"
#else
#include "include/process/processcontroller.h"
#endif

namespace pm
{

	ProcessController::ProcessController(const std::string_view& name)
	{
        SetHandle(name);
		#ifdef _WIN32	
			p_info_ = ProcessInfo(process_handle_);
		#endif // DEBUG
	}

	ProcessController::ProcessController(const ProcessController& pc):p_info_(pc.p_info_)
	{
        SetHandle(pc.Process::GetName());
	}

	ProcessController& ProcessController::operator=(const ProcessController& pc)
	{
		this->p_info_ = pc.p_info_;
		this->SetHandle(pc.Process::GetName());
		return *this;
	}

	bool ProcessController::SetHandle(int pid)
	{
		bool success = true;
		#ifdef _WIN32
			process_handle_ = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
			if (process_handle_ == nullptr)
			{
				success = false;
			}
			else 
			{
				p_info_ = ProcessInfo(process_handle_);
				success = true;
			}
		#elif __linux__

		#endif
		
		return success;

	}

	bool ProcessController::SetHandle(const std::string_view& name){
		Process::SetName(static_cast<std::string>(name));
		Process::SetPid(FindProcessIdByName(GetName()));
		int pid = Process::GetPid();
		if (pid == 0)
		{
			return false;
		}

		return SetHandle(pid);
	};

	bool ProcessController::IsExists()
	{
		#ifdef _WIN32
			if (int pid = GetPid(); GetProcessId(process_handle_) == pid && pid != 0)
			{
				return true;
			}
			return false;
		#elif __linux__
			return false;
		#endif
	}

	int ProcessController::GetPid()
	{
        #ifdef _WIN32
			auto pid = static_cast<int>(GetProcessId(process_handle_));
			Process::SetPid(pid);

        #elif __linux__
		    pid_ FindProcessIdByName(name_);
        #endif
			return pid;

	}

	ProcessInfo& ProcessController::GetProcessInfo()
	{
		return p_info_;
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
                return SetHandle(Process::GetName());
            }
        #elif __linux__
			return false;

        #endif
    };

	void ProcessController::Close()
	{
		#ifdef _WIN32
		if (process_handle_ != nullptr)
		{
			CloseHandle(process_handle_);
			process_handle_ = nullptr;
		}
		#elif __linux__

		#endif
	}

	ProcessController::~ProcessController()
	{
		Close();
	}
}